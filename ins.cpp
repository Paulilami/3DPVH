#include "ins.h"
#include "data_preprocessing.cpp" // Assuming data_preprocessing.cpp is in the same directory
#include "your_hash_function.h"  // Include header for your chosen cryptographic hash function
#include "space_mapping.h"       // Include header for space mapping logic

INS::INS(const std::string& data_preprocessing_config_path,
         const std::string& hash_function_config_path)
    : data_preprocessor_(nullptr), hash_function_(nullptr) {
  // Load configuration for data preprocessor (optional)
  if (!data_preprocessing_config_path.empty()) {
    data_preprocessor_ = data_preprocessing::create_data_preprocessor(data_preprocessing_config_path);
  } else {
    data_preprocessor_ = std::make_unique<data_preprocessing::DataPreprocessing>();  // Default preprocessor
  }

  // Load configuration for hash function
  hash_function_ = your_hash_function::create_hash_function(hash_function_config_path);
  if (!hash_function_) {
    throw std::runtime_error("Error: Failed to create hash function.");
  }
}

INS::~INS() {
  delete data_preprocessor_;  // Manual deletion for custom preprocessor objects (if applicable)
}

// Process data and generate 3D point
Eigen::Vector3d INS::process(const std::string& data) {
  // Optional pre-processing step
  std::vector<double> feature_vector;

  // Use data preprocessor if available
  if (data_preprocessor_) {
    try {
      feature_vector = data_preprocessor_->vectorize_data(data);
    } catch (const std::exception& e) {
      throw std::runtime_error(std::string("Error during pre-processing: ") + e.what());
    }
  } else {
    // Fallback to basic vectorization if no preprocessor configured
    feature_vector = data_preprocessing::vectorize_data_ngrams(data, 2);  // Example (replace with desired method)
  }

  // Error handling for empty feature vector
  if (feature_vector.empty()) {
    throw std::runtime_error("Error: Pre-processing failed to generate feature vector.");
  }

  // Hashing step
  std::string hash_output;
  try {
    hash_output = hash_function_->hash(feature_vector);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error during hashing: ") + e.what());
  }

  // Error handling for empty hash output
  if (hash_output.empty()) {
    throw std::runtime_error("Error: Hash function failed to generate output.");
  }

  // Convert hash output to 3D point
  Eigen::Vector3d point_in_3d = convert_hash_to_3d(hash_output);
  return point_in_3d;
}

// Function to convert hash output to a 3D point (implementation depends on space_mapping.cpp)
Eigen::Vector3d INS::convert_hash_to_3d(const std::string& hash_output) {
  return space_mapper_->map_to_3d(hash_output);
}

// (Optional) Function to get the used data preprocessor (if any)
const data_preprocessing::DataPreprocessing* INS::get_data_preprocessor() const {
  return data_preprocessor_.get();
}

// (Optional) Function to get the used hash function
const your_hash_function::HashFunction* INS::get_hash_function() const {
  return hash_function_.get();
}
