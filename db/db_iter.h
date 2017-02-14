#ifndef DB_DB_ITER_H
#define DB_DB_ITER_H

#include <stdint.h>
#include "leveldb/db.h"
#include "db/dbformat.h"

namespace leveldb {

// Return a new iterator that converts internal keys (yielded by
// "*internal_iter") that were live at the specified "sequence" number
// into appropriate user keys.
extern Iterator *NewDBIterator (
    const std::string *dbname,
    Env *env,
    const Comparator *user_key_comparator,
    Iterator *internal_iter,
    const SequenceNumber &sequence);

} // end of namespace leveldb

#endif // DB_DB_ITER_H

