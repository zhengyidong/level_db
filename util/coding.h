#ifndef UTIL_CODING_H
#define UTIL_CODING_H

#include <stdint.h>
#include <string.h>
#include <string>

namespace leveldb {

extern void PutFixed32(std::string *dst, uint32_t value);
extern void PutFixed64(std::string *dst, uint64_t value);
extern void PutVarInt32(std::string *dst, uint32_t value);
extern void PutVarInt64(std::string *dst, uint64_t value);

extern int VarintLength(uint64_t v);
extern void EncodeFixed32(char *dst, uint32_t value);
extern void EncodeFixed64(char *dst, uint64_t value);
extern char *EncodeVarint32(char *dst, uint32_t value);
extern char *EncodeVarint64(char *dst, uint64_t value);

// We assume it's always little-endian
inline uint32_t DecodeFixed32(const char *ptr) {
  uint32_t result;
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

