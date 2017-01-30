#ifndef UTIL_CODING_H
#define UTIL_CODING_H

#include <stdint.h>
#include <string.h>
#include <string>
#include "leveldb/slice.h"

namespace leveldb {

extern void PutFixed32(std::string *dst, uint32_t value);
extern void PutFixed64(std::string *dst, uint64_t value);
extern void PutVarint32(std::string *dst, uint32_t value);
extern void PutVarint64(std::string *dst, uint64_t value);
extern void PutLengthPrefixedSlice(std::string *dst, const Slice &value);

extern bool GetVarint32(Slice* input, uint32_t* value);
extern bool GetVarint64(Slice* input, uint64_t* value);
extern bool GetLengthPrefixedSlice(Slice* input, Slice* result);

// Pointer-based variants of GetVarint...  These either store a value
// in *v and return a pointer just past the parsed value, or return
// NULL on error.  These routines only look at bytes in the range
// [p..limit-1]
extern const char *GetVarint32Ptr(const char *p, const char *limit, uint32_t *v);
extern const char *GetVarint64Ptr(const char *p, const char *limit, uint64_t *v);

extern int VarintLength(uint64_t v);

extern void EncodeFixed32(char *dst, uint32_t value);
extern void EncodeFixed64(char *dst, uint64_t value);

extern char *EncodeVarint32(char *dst, uint32_t value);
extern char *EncodeVarint64(char *dst, uint64_t value);

// We assume it's always little-endian;
inline uint32_t DecodeFixed32(const char *ptr) {
  uint32_t result;
  memcpy(&result, ptr, sizeof(result));
  return result;
}
// We assume it's always little-endian
inline uint64_t DecodeFixed64(const char *ptr) {
  uint64_t result;
  memcpy(&result, ptr, sizeof(result));
  return result;
}

extern const char *GetVarint32PtrFallback(const char *p,
                                          const char *limit,
                                          uint32_t *value);
inline const char *GetVarint32Ptr(const char *p,
                                  const char *limit,
                                  uint32_t *value) {
  if (p < limit) {
    uint32_t result = *(reinterpret_cast<const unsigned char*>(p));
    if ((result & 128) == 0) {
      *value = result;
      return p + 1;
    }
  }
  return GetVarint32PtrFallback(p, limit, value);
}
}


#endif // UTIL_CODING_H

