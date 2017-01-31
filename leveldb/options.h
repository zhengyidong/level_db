#ifndef LEVELDB_OPTIONS_H
#define LEVELDB_OPTIONS_H

#include <stddef.h>

namespace leveldb {

class Cache;
class Comparator;
class Env;
class FilterPolicy;
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

  // Approximate size of user data packed per block.  Note that the
  // block size specified here corresponds to uncompressed data.  The
  // actual size of the unit read from disk may be smaller if
  // compression is enabled.  This parameter can be changed dynamically.
  //
  // Default: 4K
  size_t block_size;

  // Number of keys between restart points for delta encoding of keys.
  // This parameter can be changed dynamically.  Most clients should
  // leave this parameter alone.
  //
  // Default: 16
  int block_restart_interval;

  CompressionType compression;

  // If non-NULL, use the specified filter policy to reduce disk reads.
  // Many applications will benefit from passing the result of
  // NewBloomFilterPolicy() here.
  //
  // Default: NULL
  const FilterPolicy *filter_policy;
  Options();

};

struct ReadOptions {
  // If true, all data read from underlying storage will be
  // verified against corresponding checksums.
  // Default: false
  bool verify_checksums;

  // Should the data read for this iteration be cached in memory?
  // Callers may wish to set this field to false for bulk scans.
  // Default: true
  bool fill_cache;
  const Snapshot *snapshot;

  ReadOptions()
    : verify_checksums(false),
      fill_cache(true),
      snapshot(NULL) {}
};

struct WriteOptions {
  bool sync;

  WriteOptions()
    : sync(false) {}
};

}


#endif // LEVELDB_OPTIONS_H
