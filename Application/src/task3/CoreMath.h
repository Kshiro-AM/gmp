#pragma once

#define MATRIX_POINT_INDEX(x, y, z, nx, ny) ((x) + (y) * (nx) + (z) * (nx) * (ny) )
#define WPOINT_INDEX(x, y, z, nx, ny) ((x) + (nx) * ((y) + (z) * (ny)))