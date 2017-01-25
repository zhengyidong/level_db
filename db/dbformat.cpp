#include "db/dbformat.h"
#include "util/coding.h"

namespace leveldb {
static uint64_t PackSequenceAndType(uint64_t seq, ValueType t) {
  assert(seq <= kMaxSequenceNumber);
  assert(t <= kValueTypeForSeek);
  return (seq << 8) | t;
}

void AppendInternalKey(std::string *result, const ParsedInternalKey &key) {
  result->append(key.user_key.data(), key.user_key.size());
  PutFixed64(result, PackSequenceAndType(key.sequence, key.type));
}

LookupKey::LookupKey(const Slice &user_key, SequenceNumber s) {
  size_t usize = user_key.size();
  size_t needed = usize + 13;
  char *dst;
  if (needed <= sizeof(space_)) {
    dst = space_;
  } else {
    dst = new char[needed];
  }
  start_ = dst;
  dst = EncodeVarint32(dst, usize + 8);
  kstart_ = dst;
  memcpy(dst, user_key.data(), usize);
  dst += usize;
  EncodeFixed64(dst, PackSequenceAndType(s, kValueTypeForSeek));
  dst += 8;
  end_ = dst;
}

const char *InternalKeyComparator::Name() const {
  return "leveldb.InternalKeyComparator";
}

int InternalKeyComparator::Compare(const Slice &a, const Slice &b) const {
  int r = user_comparator_->Compare(ExtractUserKey(a), ExtractUserKey(b));
  if (r == 0) {
    const uint64_t anum = DecodeFixed64(a.size() + a.data() - 8);
    const uint64_t bnum = DecodeFixed64(b.size() + b.data() - 8);
    if (anum > bnum) {
      r = -1;
    } else if (anum < bnum) {
      r = +1;
    }
  }
  return r;
}

}
