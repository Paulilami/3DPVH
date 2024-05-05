#include <string>
#include <vector>
#include <algorithm> // for std::transform
#include <cctype>     // for character classification functions (isalnum, etc.)
#include <unordered_map>  // for efficient character frequency mapping (optional)

// Function to clean data
std::string clean_data(const std::string& data) {
  std::string cleaned_data;
  std::remove_copy_if(data.begin(), data.end(), std::back_inserter(cleaned_data),
                     [](char c) { return !std::isalnum(c); });
  // Optionally convert to lowercase
  // std::transform(cleaned_data.begin(), cleaned_data.end(), cleaned_data.begin(), ::tolower);
  return cleaned_data;
}

// Function to normalize data
std::string normalize_data(const std::string& data) {
  std::string normalized_data = data;
  // Implement normalization logic here (e.g., scaling numerical data)
  // You can replace this with scaling functions from libraries like Eigen
  return normalized_data;
}

// Function to vectorize data using character n-grams
std::vector<double> vectorize_data_ngrams(const std::string& data, size_t n) {
  if (n <= 0) {
    throw std::invalid_argument("Invalid n-gram size (must be positive)");
  }
  std::string cleaned_data = clean_data(data);
  std::vector<double> feature_vector;

  // Option 1: Using nested loops for n-gram generation (less efficient)
  for (size_t i = 0; i < cleaned_data.size() - n + 1; ++i) {
    std::string n_gram = cleaned_data.substr(i, n);
    // Feature extraction logic for each n-gram (e.g., TF-IDF)
    // ...
  }

  // Option 2: Using sliding window approach with a hash table (more efficient)
  std::unordered_map<std::string, double> char_ngram_counts;
  for (size_t i = 0; i < cleaned_data.size(); ++i) {
    if (i + n <= cleaned_data.size()) {
      std::string n_gram = cleaned_data.substr(i, n);
      char_ngram_counts[n_gram]++;
    }
  }

  // Feature vector creation based on n-gram counts (e.g., TF-IDF)
  // ...

  return feature_vector;
}

// Function to vectorize data using word embeddings (requires pre-trained model)
std::vector<double> vectorize_data_word2vec(const std::string& data,
                                            const std::vector<std::string>& vocabulary,
                                            const std::unordered_map<std::string, std::vector<double>>& word_embeddings) {
  std::vector<double> feature_vector(word_embeddings.at(vocabulary[0]).size(), 0.0); // Initialize with embedding size
  std::string token;
  std::stringstream tokenizer(data);

  while (std::getline(tokenizer, token, ' ')) {
    std::transform(token.begin(), token.end(), token.begin(), ::tolower);
    if (word_embeddings.count(token) > 0) {
      // Add word embedding to feature vector (e.g., averaging)
      for (size_t i = 0; i < feature_vector.size(); ++i) {
        feature_vector[i] += word_embeddings.at(token)[i];
      }
    }
  }

  // Normalize feature vector (e.g., averaging or L2 norm)
  double norm = std::sqrt(std::inner_product(feature_vector.begin(), feature_vector.end(), feature_vector.begin()));
  if (norm > 0.0) {
    for (size_t i = 0; i < feature_vector.size(); ++i) {
      feature_vector[i] /= norm;
    }
  }

  return feature_vector;
}

// (Optional) Function to load pre-trained word embeddings from a file
std::unordered_map<std::string, std::vector<double>> load_word_embeddings(const std::string& filename) {
  // Implement logic to read word embeddings from a file format (e.g., word2vec text format)
  // This function is left as an exercise as the file format can vary
  std::unordered_map<std::string, std::vector<double>> word_embeddings;
  // ...
  return word_embeddings;
}

int main() {
  // Example usage
  std::string data = "This is some sample data for hashing.";

  // Option 1: Using character n-grams
  std::vector<double> feature_vector = vectorize_data_ngrams(data, 2);

  // Option 2: Using word2vec (assuming pre-trained model and vocabulary are loaded elsewhere)
  // std::vector<string> vocabulary = {"This", "is", ...}; // Replace with actual vocabulary
  // std::unordered_map<std::string, std::vector<double>> word_embeddings = load_word_embeddings("word2vec.txt");
  // std::vector<double> feature_vector = vectorize_data_word2vec(data, vocabulary, word_embeddings);

  // Use the feature_vector for further processing in the 3DPVH
  // (e.g., pass it to the INS module)
  // ...

  return 0;
}

