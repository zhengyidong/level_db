#ifndef DB_VERSION_SET_H
#define DB_VERSION_SET_H

#include "db/dbformat.h"
#include "db/version_edit.h"

namespace leveldb {

class TableCache;

class VersionSet {
public:
  VersionSet(const std::string &dbname,
             const Options *options,
             TableCache *table,
             const InternalKeyComparator*);
  ~VersionSet();
private:
  Env *const env_;
  const std::string dbname_;
  const Options *const options_;
};
}

#endif // DB_VERSION_SET_H

