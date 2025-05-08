#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <Delaunay_psm.h>

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(delaunay_geogram, m) {

  GEO::initialize();
  GEO::Delaunay::initialize();
  m.doc() = "Geogram binding for delaunay triangulation in 3D \n"
            "https://brunolevy.github.io/geogram/classGEO_1_1Delaunay.html";
  py::class_<GEO::Delaunay, std::unique_ptr<GEO::Delaunay>>(
      m, "ParallelDelaunay3D")
      .def(py::init([]() { return GEO::Delaunay::create(3, "PDEL"); }))
      .def("adjacent_index", &GEO::Delaunay::adjacent_index, "c1"_a, "c2"_a)
      .def("cell_adjacent", &GEO::Delaunay::cell_adjacent, "c"_a, "if"_a)
      .def("cell_is_finite", &GEO::Delaunay::cell_is_finite, "c"_a)
      .def("cell_is_infinite", &GEO::Delaunay::cell_is_infinite, "c"_a)
      .def("cell_size", &GEO::Delaunay::cell_size)
      .def(
          "cell_to_cell",
          [](GEO::Delaunay &delaunay) {
            return py::array_t<geo_signed_index_t>(
                std::vector<ptrdiff_t>{
                    static_cast<py::ssize_t>(delaunay.nb_cells()),
                    static_cast<py::ssize_t>(delaunay.cell_size())},
                delaunay.cell_to_cell());
          },
          py::return_value_policy::reference_internal)
      .def(
          "cell_to_vertices",
          [](GEO::Delaunay &delaunay) {
            return py::array_t<geo_signed_index_t>(
                std::vector<ptrdiff_t>{
                    static_cast<py::ssize_t>(delaunay.nb_cells()),
                    static_cast<py::ssize_t>(delaunay.cell_size())},
                delaunay.cell_to_v());
          },
          py::return_value_policy::reference_internal)
      .def("cell_vertex", &GEO::Delaunay::cell_vertex, "c"_a, "lv"_a)
      .def("cell_adjacent", &GEO::Delaunay::cell_adjacent, "c"_a, "lf"_a)
      //.def("constraints", &GEO::Delaunay::constraints) // TODO pointer
      // handling and Mesh class wrapping
      .def("default_nb_neighbors", &GEO::Delaunay::default_nb_neighbors)
      .def("dimension", &GEO::Delaunay::dimension)
      .def("get_neighbors", &GEO::Delaunay::get_neighbors)
      .def("set_BRIO_levels", &GEO::Delaunay::set_BRIO_levels)
      .def("set_reorder", &GEO::Delaunay::set_reorder)
      .def("set_vertices",
           [](GEO::Delaunay &delaunay,
              py::array_t<double, py::array::c_style | py::array::forcecast>
                  &vertices) {
             if (vertices.ndim() != 2 || vertices.shape(1) != 3) {
               throw std::invalid_argument(
                   "vertices must be a 2D array with shape (n, 3)");
             }
             delaunay.set_vertices(static_cast<geo_index_t>(vertices.shape(0)),
                                   vertices.data());
           })
      .def("nearest_vertex",
           [](const GEO::Delaunay &dt,
              py::array_t<double, py::array::c_style | py::array::forcecast>
                  vertex) {
             if (vertex.ndim() != 1 || vertex.shape(0) != 3) {
               throw std::invalid_argument(
                   "vertex must be a vector of 3 float)");
             }
             return dt.nearest_vertex(
                 vertex.data()); // for now parent Delaunay method is called
           })
      .def("set_stores_neighbors", &GEO::Delaunay::set_stores_neighbors,
           "stores_neighbors"_a)
      .def("set_keeps_infinite", &GEO::Delaunay::set_keeps_infinite,
           "keeps_infinite"_a)
      .def("get_all_neighbors", [](const GEO::Delaunay &dt) {
        std::vector<geo_index_t> edges;
        for (geo_index_t id_vertex = 0; id_vertex < dt.nb_vertices();
             ++id_vertex) {
          GEO::vector<geo_index_t> neighbors;
          dt.get_neighbors(id_vertex, neighbors);
          for (const auto &neighbor : neighbors) {
            if (neighbor > id_vertex) // we only want to add each edge once
            {
              edges.emplace_back(id_vertex);
              edges.emplace_back(neighbor);
            }
          }
        }

        // we use fotran stride to match SPT requirements for the strides of the
        // array.
        return py::array_t<geo_index_t, py::array::f_style>(
            std::vector<ptrdiff_t>{2,
                                   static_cast<py::ssize_t>(edges.size() / 2)},
            &edges[0]);
      });
}