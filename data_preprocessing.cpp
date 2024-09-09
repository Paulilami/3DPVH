#include <string>
#include <vector>
#include <algorithm> 
#include <cctype>     
#include <unordered_map>  

std::string clean_data(const std::string& data) {
  std::string cleaned_data;
  std::remove_copy_if(data.begin(), data.end(), std::back_inserter(cleaned_data),
                     [](char c) { return !std::isalnum(c); });
  
  return cleaned_data;
}

std::string normalize_data(const std::string& data) {
  std::string normalized_data = data;

  return normalized_data;
}

std::vector<double> vectorize_data_ngrams(const std::string& data, size_t n) {
  if (n <= 0) {
    throw std::invalid_argument("Invalid n-gram size (must be positive)");
  }
  std::string cleaned_data = clean_data(data);
  std::vector<double> feature_vector;

  
  for (size_t i = 0; i < cleaned_data.size() - n + 1; ++i) {
    std::string n_gram = cleaned_data.substr(i, n);
    
  }

  std::unordered_map<std::string, double> char_ngram_counts;
  for (size_t i = 0; i < cleaned_data.size(); ++i) {
    if (i + n <= cleaned_data.size()) {
      std::string n_gram = cleaned_data.substr(i, n);
      char_ngram_counts[n_gram]++;
    }
  }

  return feature_vector;
}

std::vector<double> vectorize_data_word2vec(const std::string& data,
                                            const std::vector<std::string>& vocabulary,
                                            const std::unordered_map<std::string, std::vector<double>>& word_embeddings) {
  std::vector<double> feature_vector(word_embeddings.at(vocabulary[0]).size(), 0.0); // Initialize with embedding size
  std::string token;
  std::stringstream tokenizer(data);

  while (std::getline(tokenizer, token, ' ')) {
    std::transform(token.begin(), token.end(), token.begin(), ::tolower);
    if (word_embeddings.count(token) > 0) {
      for (size_t i = 0; i < feature_vector.size(); ++i) {
        feature_vector[i] += word_embeddings.at(token)[i];
      }
    }
  }
  double norm = std::sqrt(std::inner_product(feature_vector.begin(), feature_vector.end(), feature_vector.begin()));
  if (norm > 0.0) {
    for (size_t i = 0; i < feature_vector.size(); ++i) {
      feature_vector[i] /= norm;
    }
  }

  return feature_vector;
}

std::unordered_map<std::string, std::vector<double>> load_word_embeddings(const std::string& filename) {
 
  std::unordered_map<std::string, std::vector<double>> word_embeddings;
  return word_embeddings;
}

int main() {
 
  std::string data = "This is some sample data for hashing.";

  std::vector<double> feature_vector = vectorize_data_ngrams(data, 2);

  return 0;
}

