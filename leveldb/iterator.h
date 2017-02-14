#ifndef LEVELDB_ITERATOR_H
#define LEVELDB_ITERATOR_H

#include "leveldb/status.h"

namespace leveldb {

class Iterator {
public:
  Iterator();
  virtual ~Iterator();
  virtual bool Valid() const = 0;
  virtual void SeekToFirst() = 0;
  virtual void SeekToLast() = 0;

  // Position at the first key in the source that at or past target
  // The iterator is Valid() after this call iff the source contains
  // an entry that comes at or past target.
  virtual void Seek(const Slice &target) = 0;

  virtual void Next() = 0;
  virtual void Prev() = 0;
  virtual Slice key() const = 0;
  virtual Slice value() const = 0;
  virtual Status status() const = 0;
  typedef void(*CleanupFunction)(void *arg1, void *arg2);
  void RegisterCleanup(CleanupFunction function, void *arg1, void *arg2);

private:
  struct Cleanup {
    CleanupFunction function;
    void *arg1;
    void *arg2;
    Cleanup *next;
  };
  Cleanup cleanup_;

  // No copying allowed
  Iterator(const Iterator&);
  void operator=(const Iterator&);
};

extern Iterator *NewEmptyIterator();
extern Iterator *NewErrorIterator(const Status &status);
}

#endif // LEVELDB_ITERATOR_H

