#ifndef DB_DB_IMPL_H
#define DB_DB_IMPL_H

#include "leveldb/db.h"

namespace leveldb {
class DBImpl : public DB {
public:
  DBImpl(const Options& options, const std::string& dbname);
  virtual ~DBImpl();
  virtual Status Put(const WriteOptions&, const Slice& key, const Slice& value);
};
}

#endif // DB_DB_IMPL_H

