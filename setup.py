from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension


def set_cpp_flags():
    import os
    import sys

    # Add cpp flag recommended by upstream
    if sys.platform == "linux":
        cpp_flags = ["-frounding-math", "-fopenmp", "-O3", "-ffp-contract=off"]  
    os.environ["CPPFLAGS"] = os.getenv("CPPFLAGS", "") + " ".join(cpp_flags)


set_cpp_flags()

ext_modules = [
    Pybind11Extension(
        "delaunay_geogram",
        ["geogram.psm.Delaunay/Delaunay_psm.cpp", "delaunay_geogram_py.cpp"],
        include_dirs=["./geogram.psm.Delaunay/"],
    ),
]

setup(name="delaunay_geogram", version="0.0.1", ext_modules=ext_modules)