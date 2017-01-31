#ifndef LEVELDB_CACHE_H
#define LEVELDB_CACHE_H

#include <stdint.h>
#include "leveldb/slice.h"

namespace leveldb {

class Cache;

extern Cache *NewLRUCache(size_t capacity);
class Cache {
public:
  Cache() {}
  virtual ~Cache();
  struct Handle {};
  virtual Handle *Insert(const Slice &key, void *value, size_t charge,
                         void (*deleter)(const Slice &key, void *value)) = 0;
  virtual Handle *Lookup(const Slice &key) = 0;

  // Release a mapping returned by a previous Lookup().
  // REQUIRES: handle must not have been released yet.
  // REQUIRES: handle must have been returned by a method on *this.
  virtual void Release(Handle *handle) = 0;
  virtual void *Value(Handle *handle) = 0;
  virtual void Erase(const Slice &key) = 0;
  virtual uint64_t NewId() = 0;
private:
  void LRURemove(Handle *e);
  void LRUAppend(Handle *e);
  void Unref(Handle *e);

  struct Rep;
  Rep *rep_;

  // No copying allowed
  Cache(const Cache&);
  void operator=(const Cache&);
};
}

#endif // LEVELDB_CACHE_H

