#include "port/port.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace leveldb {
namespace port {
static void PthreadCall(const char *label, int result) {
  if (result != 0) {
    fprintf(stderr, "pthread %s: %s\n", label, strerror(result));
    abort();
  }
}

void InitOnce(OnceType *once, void (*initializer)()) {
  PthreadCall("once", pthread_once(once, initializer));
}
}
}
