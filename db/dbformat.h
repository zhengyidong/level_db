#ifndef DB_DBFORMAT_H
#define DB_DBFORMAT_H

#include "leveldb/comparator.h"
#include "leveldb/slice.h"
#include "util/coding.h"

namespace leveldb {

class InternalKey;

enum ValueType {
  kTypeDeletion = 0x0,
  kTypeValue = 0x1
};

static const ValueType kValueTypeForSeek = kTypeValue;
typedef uint64_t SequenceNumber;
static const SequenceNumber kMaxSequenceNumber = ((0x1ull << 56) - 1);

struct ParsedInternalKey {
  Slice user_key;
  SequenceNumber sequence;
  ValueType type;

  ParsedInternalKey() {}
  ParsedInternalKey(const Slice &u, const SequenceNumber &seq, ValueType t)
    :user_key(u), sequence(seq), type(t) {}
};

inline Slice ExtractUserKey(const Slice &internal_key) {
  assert(internal_key.size() >= 8);
  return Slice(internal_key.data(), internal_key.size() - 8);
}

extern void AppendInternalKey(std::string *result, const ParsedInternalKey &key);


class InternalKeyComparator : public Comparator {
private:
  const Comparator *user_comparator_;
public:
  explicit InternalKeyComparator(const Comparator *c) : user_comparator_(c) {}
  virtual const char *Name() const;
  virtual int Compare(const Slice &a, const Slice &b) const;
  const Comparator *user_comparator() const { return user_comparator_; }
};

class InternalKey {
private:
  std::string rep_;
public:
  InternalKey() {}
  InternalKey(const Slice &user_key, SequenceNumber s, ValueType t) {
    AppendInternalKey(&rep_, ParsedInternalKey(user_key, s, t));
  }

  void DecodeFrom(const Slice &s) { rep_.assign(s.data(), s.size()); }
  Slice encode() const {
    assert(!rep_.empty());
    return rep_;
  }

  Slice user_key() const { return ExtractUserKey(rep_); }

  void SetFrom(const ParsedInternalKey &p) {
    rep_.clear();
    AppendInternalKey(&rep_, p);
  }

  void Clear() { rep_.clear(); }
};

class LookupKey {
public:
  LookupKey(const Slice &user_key, SequenceNumber sequence);
  ~LookupKey();
  Slice memtable_key() const { return Slice(start_, end_ - start_); }
  Slice internal_key() const { return Slice(kstart_, end_ - kstart_); }
  Slice user_key() const { return Slice(kstart_, end_ - kstart_ - 8); }
private:
  const char *start_;
  const char *kstart_;
  const char *end_;
  char space_[200];

  LookupKey(const LookupKey&);
  void operator=(const LookupKey&);
};

inline LookupKey::~LookupKey() {
  if (start_ != space_) delete[] start_;
}

}

#endif // DB_DBFORMAT_H

