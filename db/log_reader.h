#ifndef DB_LOG_READER_H
#define DB_LOG_READER_H

#include <stdint.h>

#include "leveldb/status.h"

namespace leveldb {

class SequentialFile;

namespace log {

class Reader {
public:
  class Reporter {
  public:
    virtual ~Reporter();

    // Some corruption was detected.  "size" is the approximate number
    // of bytes dropped due to the corruption.
    virtual void Corruption(size_t bytes, const Status &status) = 0;
  };

  // Create a reader that will return log records from "*file".
  // "*file" must remain live while this Reader is in use.
  //
  // If "reporter" is non-NULL, it is notified whenever some data is
  // dropped due to a detected corruption.  "*reporter" must remain
  // live while this Reader is in use.
  //
  // If "checksum" is true, verify checksums if available.
  //
  // The Reader will start reading at the first record located at physical
  // position >= initial_offset within the file.
  Reader(SequentialFile *file, Reporter *reporter, bool checksum,
         uint64_t initial_offset);

  // Read the next record into *record.  Returns true if read
  // successfully, false if we hit end of the input.  May use
  // "*scratch" as temporary storage.  The contents filled in *record
  // will only be valid until the next mutating operation on this
  // reader or the next mutation to *scratch.
  bool ReadRecord(Slice *record, std::string *scratch);
  // TODO
};
}
}

#endif // DB_LOG_READER_H

