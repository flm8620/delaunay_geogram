#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <Delaunay_psm.h>

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(delaunay_geogram, m)
{
    GEO::initialize();
    m.doc() = "Geogram binding for delaunay tetrahedrization"; // optional module docstring
    py::class_<GEO::Delaunay, std::unique_ptr<GEO::Delaunay, py::nodelete>>(m, "ParallelDelaunay3D")
        .def(py::init([]()
                      { return GEO::Delaunay::create(3, "PDEL"); }))
        .def("set_vertices", [](GEO::Delaunay &delaunay, py::array_t<double, py::array::c_style | py::array::forcecast> vertices)
             { 
                if (vertices.ndim() != 2 or vertices.shape(1) != 3) {
                        throw std::invalid_argument("vertices must be a 2D array with shape (n, 3)");
                    }
                delaunay.set_vertices(vertices.shape(0), vertices.data()); })
        .def("set_stores_neighbors", &GEO::Delaunay::set_stores_neighbors, "stores_neighbors"_a)
        .def("set_keeps_infinite", &GEO::Delaunay::set_keeps_infinite, "keeps_infinite"_a)
        .def(
            "get_all_neighbors", [](const GEO::Delaunay &dt)
            {
                std::vector<geo_index_t> edges;
                for (geo_index_t id_vertex = 0; id_vertex < dt.nb_vertices(); ++id_vertex)
                {
                    GEO::vector<geo_index_t> neighbors;
                    dt.get_neighbors(id_vertex, neighbors);
                    for (const auto &neighbor : neighbors)
                    {
                        if (neighbor > id_vertex) // we only want to add each edge once
                        {
                            edges.emplace_back(id_vertex);
                            edges.emplace_back(neighbor);
                        }
                    }
                }

                // we use fotran stride to match SPT requirements for the strides of the array.
                return py::array_t<geo_index_t, py::array::f_style>(
                    std::vector<ptrdiff_t>{2, static_cast<uint32_t>(edges.size() / 2)},
                    &edges[0]); });
}