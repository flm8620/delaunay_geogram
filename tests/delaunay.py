import pytest
import numpy as np

from delaunay_geogram import ParallelDelaunay3D


def test_delaunay():
    point_cloud = np.random.rand(100, 3)
    dt = ParallelDelaunay3D()
    dt.set_stores_neighbors(True)
    dt.set_vertices(point_cloud)
    edges = dt.get_all_neighbors()
    assert edges.shape[0] > 1


def test_fail_delaunay():
    point_cloud = np.random.rand(100, 1)
    dt = ParallelDelaunay3D()
    with pytest.raises(ValueError) as exception_info:
        dt.set_vertices(point_cloud)
    assert str(exception_info.value) == "vertices must be a 2D array with shape (n, 3)"
