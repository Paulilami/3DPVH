#include "validation.h"
#include <vector>
#include <limits>  
#include <cmath>

// Function to define the expected 3D space range for valid data points
std::vector<double> define_valid_range(const std::string& ins_type) {

  // Example: 
  if (ins_type == "INS_A") {
    double radius = 1.0;  
    double uncertainty = 0.1;  
    return {-radius + uncertainty, radius - uncertainty,
            -radius + uncertainty, radius - uncertainty,
            -radius + uncertainty, radius - uncertainty};
  } else {
    // Handle unknown INS types or return default range
    return {};
  }
}

bool validate_data(const std::vector<INSData>& ins_data) {

  std::string ins_type = ins_data.front().// ...sam2...
  std::vector<double> valid_range = define_valid_range(ins_type);
  if (valid_range.empty()) {
    return false; // No valid range
  }

  for (const INSData& data : ins_data) {
    if (data.point3d.x < valid_range[0] || data.point3d.x > valid_range[1] ||
        data.point3d.y < valid_range[2] || data.point3d.y > valid_range[3] ||
        data.point3d.z < valid_range[4] || data.point3d.z > valid_range[5]) {
      return false; // Data point outside valid range (excluding uncertainty)
    }

   
    double mean_x = data.point3d.x;  
    double std_dev_x = uncertainty;   
    double chebyshev_bound_x = mean_x + 2.0 * std_dev_x;  // 97.7% confidence interval
    if (data.point3d.x < chebyshev_bound_x || data.point3d.x > valid_range[1]) {
      return false; // Outside valid range considering Chebyshev's bound --> https://en.wikipedia.org/wiki/Chebyshev%27s_inequality
    }
    // Repeat checks for y and z coordinates.

  }

double calculate_pairwise_distance(const Point3D& point1, const Point3D& point2) {
  return sqrt(pow(point1.x - point2.x, 2.0) + pow(point1.y - point2.y, 2.0) + pow(point1.z - point2.z, 2.0));
}

bool perform_spatial_consistency_check(const std::vector<INSData>& ins_data, double max_allowed_distance) {
  for (size_t i = 0; i < ins_data.size(); ++i) {
    for (size_t j = i + 1; j < ins_data.size(); ++j) {  // Avoid redundant comparisons
      double distance = calculate_pairwise_distance(ins_data[i].point3d, ins_data[j].point3d);
      if (distance > max_allowed_distance) {
        // Flag data points as suspicious if distance exceeds the threshold
        return false;
      }
    }
  }
  return true; 

bool perform_temporal_coherence_analysis(const std::vector<INSData>& ins_data) {
  // Placeholder till sam2
  } else {
    return true;
  }
}


return true; // All data points valid
