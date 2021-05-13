#include "hash_table.hpp"

#include <stdexcept>

namespace itis {
  int HashTable::hash(int key) const {
    return utils::hash(key, static_cast<int>(buckets_.size()));
    }

  HashTable::HashTable(int capacity, double load_factor) : load_factor_{load_factor} {
    if (capacity <= 0) {
        throw std::logic_error("Capacity must be > 0");
        }

    if (load_factor <= 0.0 || load_factor > 1.0) {
        throw std::logic_error("Load factor must be in [0,1]");
        }

        buckets_ = std::vector<Bucket>{};
        buckets_.resize(capacity);
    }

  std::optional<std::string> HashTable::Search(int key) const {
        auto index = hash(key);
        auto bucket = buckets_[index];
        for (std::pair<int, std::string> pair: bucket){
            if (pair.first == key) {
                return pair.second;
            }
        }
        return std::nullopt;
    }

  void HashTable::Put(int key, const std::string &value) {
     auto index = hash(key);
     for (std::pair<int, std::string> &pair: buckets_[index]){
          if (pair.first == key) {
             pair.second = value;
             return;
            }
        }
     buckets_[index].emplace_back(key, value);
     num_keys_++;
        if (static_cast<double>(num_keys_) / buckets_.size() >= load_factor_) {
            std::vector<Bucket> buckets_vector = std::vector<Bucket>{};
            auto new_size = buckets_.size() * kGrowthCoefficient;
            buckets_vector.resize(new_size);
            for (Bucket &bucket : buckets_){
                for (const auto& pair : bucket){
                    auto new_index = utils::hash(pair.first, new_size);
                    buckets_vector[new_index].push_back(pair);
                }
            }
            buckets_ = buckets_vector;
        }
    }

  std::optional<std::string> HashTable::Remove(int key) {
     auto index = hash(key);
     for(const auto& pair : buckets_[index]){
        if (pair.first == key) {
                std::string to_return = pair.second;
                buckets_[index].remove(pair);
                return to_return;
            }
        }
        return std::nullopt;
    }


  bool HashTable::ContainsKey(int key) const {
        // Note: uses Search(key) which is not initially implemented
        return Search(key).has_value();
    }

  bool HashTable::empty() const {
        return size() == 0;
    }

  int HashTable::size() const {
        return num_keys_;
    }

  int HashTable::capacity() const {
        return static_cast<int>(buckets_.size());
    }

    double HashTable::load_factor() const {
        return load_factor_;
    }

  std::unordered_set<int> HashTable::keys() const {
        std::unordered_set<int> keys(num_keys_);
        for (const auto &bucket : buckets_) {
            for (const auto &[key, _] : bucket) {
                keys.insert(key);
            }
        }
        return keys;
    }

  std::vector<std::string> HashTable::values() const {
      std::vector<std::string> values;
      for (const auto &bucket : buckets_) {
          for (const auto &[_, value] : bucket) {
             values.push_back(value);
            }
        }
        return values;
    }
}  // namespace itis