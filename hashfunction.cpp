#include "hash_function.h"
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <future>  
#include <random>
#include <json.hpp>  
#include <crypto++/cryptlib.h>
#include <crypto++/sha.h>
#include <crypto++/hex.h>

std::mutex hash_function_mutex_;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> salt_dist(1, 1000); 

class SecureHashFunction : public HashFunction {
public:
  std::string hash(const std::vector<double>& feature_vector) const override {
    std::lock_guard<std::mutex> lock(hash_function_mutex_);

    int salt = salt_dist(gen);

    std::vector<double> transformed_features = high_dimensional_transform(feature_vector);

    transformed_features.push_back(salt);

    std::vector<std::future<std::string>> hash_results;
    int num_threads = std::thread::hardware_concurrency();  
    for (int i = 0; i < num_threads; ++i) {
      hash_results.push_back(std::async(std::launch::async, &SecureHashFunction::thread_hash, this, transformed_features, i));
    }

    std::string final_hash;
    for (auto& result : hash_results) {
      final_hash += result.get();
    }

    return final_hash;
  }

private:
  std::vector<double> high_dimensional_transform(const std::vector<double>& feature_vector) const {
    return feature_vector;
  }

  std::string thread_hash(const std::vector<double>& data, int thread_id) const {
    CryptoPP::SHA256 hash;
    std::string message;

    std::vector<unsigned char> byte_array(data.size() * sizeof(double));
    memcpy(byte_array.data(), data.data(), byte_array.size());

    message.append(std::to_string(thread_id));
    message.append("|");

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
};

std::unique_ptr<HashFunction> create_hash_function(const std::string& config_path) {
  nlohmann::json config;
  std::ifstream config_file(config
  nlohmann::json config;
  std::ifstream config_file(config_path);
  if (!config_file.is_open()) {
    throw std::runtime_error("Error: Failed to open configuration file: " + config_path);
  }

  try {
    config_file >> config;
  } catch (const nlohmann::json::parse_error& e) {
    config_file.close();
    throw std::runtime_error("Error: Failed to parse configuration file: " + std::string(e.what()));
  }

  config_file.close();

  std::string reduction_technique;
  if (config.contains("reduction_technique")) {
    reduction_technique = config["reduction_technique"];
  } else {
    reduction_technique = "PCA";  
  }
  return std::make_unique<SecureHashFunction>();
}
