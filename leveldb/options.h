#ifndef LEVELDB_OPTIONS_H
#define LEVELDB_OPTIONS_H

#include <stddef.h>

namespace leveldb {

class Cache;
class Comparator;
class Env;
class Logger;
class Snapshot;

enum CompressionType {
  kNoCompression = 0x0,
  kSnappyCompression = 0x1
};

struct Options {
  const Comparator *comparator;
  bool create_if_missing;
  bool error_if_exists;
  bool paranoid_checks;
  Env *env;
  Logger *info_log;
  size_t write_buffer_size;
  int max_open_files;
  Cache *block_cache;
  CompressionType compression;
  const FilterPolicy *filter_policy;
  Options();
};

struct ReadOption {
  bool verify_checksums;
  bool fill_cache;
  const Snapshot *snapshot;

  ReadOption()
    : verify_checksums(false),
      fill_cache(true),
      snapshot(NULL) {}
};

struct WriteOption {
  bool sync;

  WriteOption()
    : sync(false) {}
};

}


#endif // LEVELDB_OPTIONS_H
