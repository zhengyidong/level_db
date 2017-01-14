#ifndef UTIL_HASH_H
#define UTIL_HASH_H

#include <stddef.h>
#include <stdint.h>

namespace leveldb {
extern uint32_t Hash(const char *data, size_t n, uint32_t seed);
}

#endif // UTIL_HASH_H


