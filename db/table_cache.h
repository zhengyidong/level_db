#ifndef DB_TABLE_CACHE_H
#define DB_TABLE_CACHE_H

#include <string>
#include "db/dbformat.h"
#include "leveldb/cache.h"

namespace leveldb {

class TableCache {
public:
  TableCache(const std::string &dbname, const Options *options, int entries);
  ~TableCache();

  Status Get(const ReadOptions &options,
             uint64_t file_number,
             uint64_t file_size,
             const Slice &k,
             void *arg,
             void (*handle_result)(void*, const Slice&, const Slice&));
private:
  Env *const env_;
  const std::string dbname_;
  const Options *options_;
  Cache *cache_;

  Status FindTable(uint64_t file_number, uint64_t file_size, Cache::Handle**);
};
}
#endif // DB_TABLE_CACHE_H

