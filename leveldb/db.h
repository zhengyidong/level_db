#ifndef LEVELDB_DB_H
#define LEVELDB_DB_H

#include "leveldb/iterator.h"

namespace leveldb {
// Update Makefile if you change these
static const int kMajorVersion = 1;
static const int kMinorVersion = 7;

struct Options;
struct ReadOptions;
struct WriteOptions;
class WriteBatch;

class Snapshot {
protected:
  virtual ~Snapshot();
};

struct Range {
  Slice start;
  Slice limit;
  Range() {}
  Range(const Slice &s, const Slice &l) : start(s), limit(l) {}
};

class DB {
public:
  static Status Open(const Options &options, const std::string &name, DB **ptr);
  DB() {}
  virtual ~DB();
  virtual Status Put(const WriteOptions &options,
                     const Slice &key,
                     const Slice &value) = 0;
  virtual Status Delete(const WriteOptions &options, const Slice &key) = 0;
  virtual Status Write(const WriteOptions &options, WriteBatch *updates) = 0;
  virtual Status Get(const ReadOptions& options,
                     const Slice& key, std::string* value) = 0;
private:
  // No copying allowed
  DB(const DB&);
  void operator=(const DB&);
};

Status DestroyDB(const std::string& name, const Options& options);
}

#endif // LEVELDB_DB_H

