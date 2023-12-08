import pytest
import numpy as np

from delaunay_geogram import ParallelDelaunay3D

def generate_random_dt(stores_neighbors = True):
    point_cloud = np.random.rand(100, 3)
    dt = ParallelDelaunay3D()
    dt.set_stores_neighbors(stores_neighbors)
    dt.set_vertices(point_cloud)
    return dt

def test_delaunay():
    dt = generate_random_dt(True)
    edges = dt.get_all_neighbors()
    assert edges.shape[0] > 1

def test_delaunay_cell_indexing():
    dt = generate_random_dt(False)
    assert dt.cell_vertex(1,3) == dt.cell_to_vertices()[1][3]
    assert dt.cell_adjacent(2,0) == dt.cell_to_cell()[2][0]

def test_fail_delaunay():
    point_cloud = np.random.rand(100, 1)
    dt = ParallelDelaunay3D()
    with pytest.raises(ValueError) as exception_info:
        dt.set_vertices(point_cloud)
    assert str(exception_info.value) == "vertices must be a 2D array with shape (n, 3)"
