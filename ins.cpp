#include "ins.h"
#include "data_preprocessing.cpp" 
#include "your_hash_function.h"  
#include "space_mapping.h"       

INS::INS(const std::string& data_preprocessing_config_path,
         const std::string& hash_function_config_path)
    : data_preprocessor_(nullptr), hash_function_(nullptr) {
  // Load configuration for data preprocessor 
  if (!data_preprocessing_config_path.empty()) {
    data_preprocessor_ = data_preprocessing::create_data_preprocessor(data_preprocessing_config_path);
  } else {
    data_preprocessor_ = std::make_unique<data_preprocessing::DataPreprocessing>();  // Default
  }

  // Load config
  hash_function_ = your_hash_function::create_hash_function(hash_function_config_path);
  if (!hash_function_) {
    throw std::runtime_error("Error: Failed to create hash function.");
  }
}

INS::~INS() {
  delete data_preprocessor_;  // Manual deletion 
}

// Generate 3D point
Eigen::Vector3d INS::process(const std::string& data) {
  // Pre-processing step
  std::vector<double> feature_vector;

  // Use data preprocessor 
  if (data_preprocessor_) {
    try {
      feature_vector = data_preprocessor_->vectorize_data(data);
    } catch (const std::exception& e) {
      throw std::runtime_error(std::string("Error during pre-processing: ") + e.what());
    }
  } else {
    // Fallback mec
    feature_vector = data_preprocessing::vectorize_data_ngrams(data, 2); 
  }

  // Empty feature vector
  if (feature_vector.empty()) {
    throw std::runtime_error("Error: Pre-processing failed to generate feature vector.");
  }

  // Hashing
  std::string hash_output;
  try {
    hash_output = hash_function_->hash(feature_vector);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error during hashing: ") + e.what());
  }

  // Empty hash output
  if (hash_output.empty()) {
    throw std::runtime_error("Error: Hash function failed to generate output.");
  }

  // Convert output to 3D endpoint
  Eigen::Vector3d point_in_3d = convert_hash_to_3d(hash_output);
  return point_in_3d;
}

// Function sample2 (might change other components are updated, used to fallback first)
Eigen::Vector3d INS::convert_hash_to_3d(const std::string& hash_output) {
  return space_mapper_->map_to_3d(hash_output);
}

const data_preprocessing::DataPreprocessing* INS::get_data_preprocessor() const {
  return data_preprocessor_.get();
}

const your_hash_function::HashFunction* INS::get_hash_function() const {
  return hash_function_.get();
}
