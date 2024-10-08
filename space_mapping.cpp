#include "space_mapping.h"
#include <vector>
#include <cmath>
#include <random>
#include <algorithm> 
#include <limits>     
#include <eigen3/Eigen/Dense>  

struct Point3D {
  double x;
  double y;
  double z;
};

class SpaceMapping {
public:
  Point3D map_to_space(const std::vector<double>& feature_vector) const {
	  std::vector<double> projected_features = project_to_lower_dimension(feature_vector);

	  std::string hashed_features = hash_features(projected_features);

    Point3D point = map_hashed_features(hashed_features);

    return point;
  }

private:
  
  std::vector<double> project_to_lower_dimension(const std::vector<double>& feature_vector) const {
    return feature_vector;
  }

  std::string hash_features(const std::vector<double>& features) const {
    CryptoPP::SHA256 hash;
    std::string message;

    std::vector<unsigned char> byte_array(features.size() * sizeof(double));
    memcpy(byte_array.data(), features.data(), byte_array.size());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> salt_dist(1, 1000);
    int salt = salt_dist(gen);

    message.append(std::to_string(salt));
    message.append("|");
    message.append(std::string(byte_array.begin(), byte_array.end()));

    hash.Update((const unsigned char*)message.c_str(), message.size());
    byte_array.resize(CryptoPP::SHA256::DIGESTSIZE);
    hash.Final(byte_array.data());

    std::string hash_string;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(hash_string));
    encoder.Put(byte_array.data(), byte_array.size());
    encoder.MessageEnd();

    return hash_string;
  }

  Point3D map_hashed_features(const std::string& hashed_features) const {
    Point3D point;

    std::vector<double> hash_values;
    for (char c : hashed_features) {
      hash_values.push_back(static_cast<double>(c) / 255.0);  
    }

    point.x = std::sin(hash_values[0] * 2 * M_PI);
    point.y = std::cos(hash_values[1] * 2 * M_PI);
    point.z = std::sin(hash_values[2] * 2 * M_PI);


double phi = 2 * M_PI * hash_values[1];

point.x = std::sin(theta) * std::cos(phi);
point.y = std::sin(theta) * std::sin(phi);
point.z = std::cos(theta);
return point;
  }
};
