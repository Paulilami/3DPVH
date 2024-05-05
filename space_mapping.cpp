#include "space_mapping.h"
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>  // for std::min
#include <limits>     // for numeric_limits
#include <eigen3/Eigen/Dense>  // Assuming using Eigen library for linear algebra

// Assuming a 3D space for point representation (adjust dimensions as needed)
struct Point3D {
  double x;
  double y;
  double z;
};

class SpaceMapping {
public:
  Point3D map_to_space(const std::vector<double>& feature_vector) const {
    // 1. High-Dimensional Feature Space Projection (replace with your choice)
    std::vector<double> projected_features = project_to_lower_dimension(feature_vector);

    // 2. Hashing with Salting (optional)
    // This step can be used for additional security if desired
    std::string hashed_features = hash_features(projected_features);

    // 3. Mapping to 3D Space
    Point3D point = map_hashed_features(hashed_features);

    return point;
  }

private:
  // Choose and implement your desired dimensionality reduction technique here
  // Options include:
  // - Kernel Principal Component Analysis (KPCA)
  // - Random Projections
  // - Locality Sensitive Hashing (LSH)
  std::vector<double> project_to_lower_dimension(const std::vector<double>& feature_vector) const {
    // Placeholder for now, return original features without dimensionality reduction
    // You can replace this with your chosen technique's implementation
    return feature_vector;
  }

  // Hashing function with salting (optional)
  std::string hash_features(const std::vector<double>& features) const {
    // Crypto++ library for secure hashing
    CryptoPP::SHA256 hash;
    std::string message;

    // Convert features to byte array
    std::vector<unsigned char> byte_array(features.size() * sizeof(double));
    memcpy(byte_array.data(), features.data(), byte_array.size());

    // Generate random salt (optional for additional security)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> salt_dist(1, 1000);
    int salt = salt_dist(gen);

    message.append(std::to_string(salt));
    message.append("|");
    message.append(std::string(byte_array.begin(), byte_array.end()));

    // Hash the message
    hash.Update((const unsigned char*)message.c_str(), message.size());
    byte_array.resize(CryptoPP::SHA256::DIGESTSIZE);
    hash.Final(byte_array.data());

    // Convert hash output to hex string
    std::string hash_string;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(hash_string));
    encoder.Put(byte_array.data(), byte_array.size());
    encoder.MessageEnd();

    return hash_string;
  }

  // Mapping hashed features to a 3D point (replace with your custom mapping logic)
  // This example uses a simple trigonometric mapping for demonstration (can be replaced)
  Point3D map_hashed_features(const std::string& hashed_features) const {
    Point3D point;

    // Convert each character in the hash string to a double value (example)
    std::vector<double> hash_values;
    for (char c : hashed_features) {
      hash_values.push_back(static_cast<double>(c) / 255.0);  // Normalize to [0, 1]
    }

    // **Example 1: Simple Trigonometric Mapping**
    point.x = std::sin(hash_values[0] * 2 * M_PI);
    point.y = std::cos(hash_values[1] * 2 * M_PI);
    point.z = std::sin(hash_values[2] * 2 * M_PI);

    // **Example 2: Spherical Mapping (consider using for distance-based queries)**
		// double theta = acos(1 - 2 * hash_values[0]);  // Incorrect normalization
double theta = acos(2 * hash_values[0] - 1);  // Corrected normalization for [0, 1] range

// phi (longitude) can be calculated from another hash value
double phi = 2 * M_PI * hash_values[1];

// Convert spherical coordinates to Cartesian coordinates
point.x = std::sin(theta) * std::cos(phi);
point.y = std::sin(theta) * std::sin(phi);
point.z = std::cos(theta);

// **Placeholder for Complex Mapping (using Eigen library for example)**
// Eigen::VectorXd projected_features = Eigen::Map<const Eigen::VectorXd>(feature_vector.data(), feature_vector.size());
// // Implement your complex mapping using Eigen for linear algebra operations (e.g., rotations, projections)
// // ...
// Point3D point = convert_to_3d(projected_features);  // Replace with your conversion function

return point;
  }
};
