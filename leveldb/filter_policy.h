#ifndef LEVELDB_FILTER_POLICY_H
#define LEVELDB_FILTER_POLICY_H

#include <string>

namespace leveldb {

class Slice;

class FilterPolicy {
public:
  ~FilterPolicy();
  virtual const char *Name() const = 0;
  virtual void CreateFilter(const Slice *key, int n, std::string *dst) const = 0;
  virtual bool KeyMayMatch(const Slice &key, const Slice &filter) const = 0;
};

extern const FilterPolicy *NewBloomFilterPolicy(int bits_per_key);
}

#endif // LEVELDB_FILTER_POLICY_H

