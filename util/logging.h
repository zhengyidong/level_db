#ifndef UTIL_LOGGING_H
#define UTIL_LOGGING_H

#include <stdint.h>

namespace leveldb {

class Slice;

extern bool ConsumeDecimalNumber(Slice *in, uint64_t *val);
}

#endif // UTIL_LOGGING_H

