#include "util/coding.h"

namespace leveldb {

void EncodeFixed32(char *dist, uint32_t value) {
  memcpy(dist, &value, sizeof(value));
}
void EncodeFixed64(char *dist, uint64_t value) {
  memcpy(dist, &value, sizeof(value));
}



void PutFixed32(std::string *dst, uint32_t value) {
  char buf[sizeof(value)];
  EncodeFixed32(buf, value);
  dst->append(buf, sizeof(buf));
}

void PutFixed64(std::string *dst, uint64_t value) {
  char buf[sizeof(value)];
  EncodeFixed64(buf, value);
  dst->append(buf, sizeof(buf));
}

char *EncodeVarint32(char *dst, uint32_t v) {
  unsigned char *ptr = reinterpret_cast<unsigned char*>(dst);
  static const int B = 128;
  if (v < 1<<7) {
    *(ptr++) = v;
  } else if (v < (1 << 14)) {
    *(ptr++) = v | B;
    *(ptr++) = (v >> 7);
  } else if (v < (1 << 21)) {
    *(ptr++) = v | B;
    *(ptr++) = (v >> 7) | B;
    *(ptr++) = (v >> 14);
  } else if (v < (1 << 28)) {
    *(ptr++) = v | B;
    *(ptr++) = (v >> 7) | B;
    *(ptr++) = (v >> 14) | B;
    *(ptr++) = (v >> 21);
  } else {
    *(ptr++) = v | B;
    *(ptr++) = (v >> 7) | B;
    *(ptr++) = (v >> 14) | B;
    *(ptr++) = (v >> 21) | B;
    *(ptr++) = (v >> 28);
  }
  return reinterpret_cast<char*>(ptr);
}

char *EncodeVarint64(char *dst, uint64_t v) {
  static const int B = 128;
  unsigned char *ptr = reinterpret_cast<unsigned char*>(dst);
  while (v >= B) {
    *(ptr++) = (v & (B - 1)) | B;
    v >>= 7;
  }
  *(ptr++) = static_cast<unsigned char>(v);
  return reinterpret_cast<char*>(ptr);
}

int VarintLength(uint64_t v) {
  int len = 1;
  while (v >= 128) {
    v >>= 7;
    ++len;
  }
  return len;
}

const char *GetVarint32PtrFallback(const char *p,
                                          const char *limit,
                                          uint32_t *value) {
  uint32_t result = 0;
  for (uint32_t shift = 0; shift <= 28 && p < limit; shift += 7) {
    uint32_t byte = *(reinterpret_cast<const unsigned char*>(p));
    p++;
    if (byte & 128) {
      result |= ((byte & 127) << shift);
    } else {
      result |= (byte << shift);
      *value = result;
      return (reinterpret_cast<const char *>(p));
    }
  }
}

}
