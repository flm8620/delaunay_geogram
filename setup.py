import os
import sys
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

# Add cpp flag recommended by upstream
if sys.platform == "linux":
    extra_compile_args = ["-frounding-math", "-fopenmp", "-O3", "-ffp-contract=off", "-DGEO_STATIC_LIBS"]  
    extra_link_args = []
if sys.platform == "win32":
    extra_compile_args = ["/openmp", "/DGEO_STATIC_LIBS"]
    extra_link_args = ["advapi32.lib", "Shell32.lib"] # Why this is needed?
    
ext_modules = [
    Pybind11Extension(
        "delaunay_geogram",
        ["delaunay_geogram_py.cpp", "./geogram.psm.Delaunay/Delaunay_psm.cpp"],
        include_dirs=["./geogram.psm.Delaunay/"],
        extra_link_args=extra_link_args,
        extra_compile_args = extra_compile_args
    ),
]

setup(name="delaunay_geogram", version="0.0.1", ext_modules=ext_modules, cmdclass={"build_ext": build_ext})