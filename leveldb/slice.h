#ifndef LEVELDB_SLICE_H
#define LEVELDB_SLICE_H

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <string>

namespace leveldb {

class Slice {
public:
  Slice() : data_(""), size_(0) {}
  Slice(const char *d, size_t n) : data_(d), size_(n) {}
  Slice(const std::string &s) : data_(s.data()), size_(s.size()) {}
  Slice(const char *s) : data_(s), size_(strlen(s)) {}

  const char *data() const { return data_; }
  size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }
  char operator[](size_t n) {
    assert(n < size());
    return data_[n];
  }
  void clear() { data_ = ""; size_ = 0; }

  // Drop the first "n" bytes from this slice.
  void remove_prefix(size_t n) {
    assert(n <= size());
    data_ += n;
    size_ -= n;
  }

  std::string ToString() const { return std::string(data_, size_); }

  int compare(const Slice &b) const;
private:
  const char *data_;
  size_t size_;
};

inline bool operator==(const Slice &a, const Slice &b) {
  return ((a.size() == b.size()) &&
          (memcmp(a.data(), b.data(), a.size()) == 0));
}

inline bool operator!=(const Slice &a, const Slice &b) {
  return !(a == b);
}

inline int Slice::compare(const Slice &b) const {
  size_t min_size = (size_ < b.size_) ? size_ : b.size_;
  int r = memcmp(data_, b.data_, min_size);
  if (r == 0) {
    if (size_ < b.size_) r = -1;
    else if (size_ > b.size_) r = 1;
  }
  return r;
}
}

#endif // LEVELDB_SLICE_H

