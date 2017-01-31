// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef LEVELDB_DB_H
#define LEVELDB_DB_H

#include "leveldb/iterator.h"
#include "leveldb/options.h"

namespace leveldb {
// Update Makefile if you change these
static const int kMajorVersion = 1;
static const int kMinorVersion = 7;

struct Options;
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

// A DB is a persistent ordered map from keys to values.
// A DB is safe for concurrent access from multiple threads without
// any external synchronization.
class DB {
public:
  DB() {}
  virtual ~DB();

  // Open the database with the specified "name".
  // Stores a pointer to a heap-allocated database in *dbptr and returns
  // OK on success.
  // Stores NULL in *dbptr and returns a non-OK status on error.
  // Caller should delete *dbptr when it is no longer needed.
  static Status Open(const Options &options,
                     const std::string &name,
                     DB **ptr);
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

