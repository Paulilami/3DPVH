#include "validation.h"

// Data structures (replace with your actual data types)
struct Point3D {
  double x;
  double y;
  double z;
};

struct INSData {
  // ... data fields specific to your Information Source (INS)
  Point3D point3d;  // The 3D point representing the retrieved information
};

bool validate_data(const std::vector<INSData>& ins_data);
