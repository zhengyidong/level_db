#ifndef PORT_PORT_H
#define PORT_PORT_H

#include <pthread.h>
#include "port/atomic_pointer.h"

namespace leveldb {
namespace port {

typedef pthread_once_t OnceType;
#define LEVELDB_ONCE_INIT PTHREAD_ONCE_INIT
extern void InitOnce(OnceType *once, void (*initializer)());

}
}

#endif // PORT_PORT_H

