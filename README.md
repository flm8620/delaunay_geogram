# Delaunay Geogram

Fast and Robust 3D Delaunay Triangulation for Python / Python bindings for [Geogram Delaunay PSM](https://github.com/BrunoLevy/geogram.psm.Delaunay)

For now, it offers only very minimalistic bindings. 
It is mainly intended to be plugged into [SuperPoint Transformer](https://github.com/drprojects/superpoint_transformer) 
in replacement of [https://docs.scipy.org/doc/scipy/reference/generated/scipy.spatial.Delaunay.html](scipy.spatial.Delaunay) which is based on QHull.
In this context, Geogram parallel and optimized architecture dramatically improve speed execution over Qhull based Delaunay.
The self contained  

Bindings are not complete and API could evolve and break, we will try to adhere to semantic versioning (see changelog)

# Installation


```console
git clone --recurse-submodules https://github.com/3DFin/delaunay_geogram
pip install .
```

# Usage

```python
import numpy as np
from delaunay_geogram import ParallelDelaunay3D
point_cloud = np.random.rand(100, 3)
dt = ParallelDelaunay3D()
dt.set_vertices(point_cloud)
```

# License

(c) 2023 Swansea University for the bindings

Geogram is licensed under the BSD 3 clause, see [here](https://github.com/BrunoLevy/geogram.psm.Delaunay/blob/main/LICENSE)
