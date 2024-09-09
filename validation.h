#include "validation.h"

struct Point3D {
  double x;
  double y;
  double z;
};

struct INSData { Point3D point3d; }

bool validate_data(const std::vector<INSData>& ins_data);
