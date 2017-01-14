#include "leveldb/comparator.h"
#include "leveldb/slice.h"
#include "port/port.h"

namespace leveldb {
Comparator::~Comparator() {}

namespace {
class BytewiseComparatorImpl : public Comparator {
public:
  BytewiseComparatorImpl() {}

  virtual const char *Name() const {
    return "leveldb.BytewiseComparator";
  }

  virtual int Compare(const Slice &a, const Slice &b) const {
    return a.compare(b);
  }
};

static port::OnceType once = LEVELDB_ONCE_INIT;
static const Comparator *bytewise;

static void InitModule() {
  bytewise = new BytewiseComparatorImpl;
}

const Comparator *BytewiseComparator() {
  port::InitOnce(&once, InitModule);
  return bytewise;
}
}
}
