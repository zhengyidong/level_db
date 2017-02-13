#ifndef DB_LOG_READER_H
#define DB_LOG_READER_H

namespace leveldb {
namespace log {

class Reader {
public:
  class Reporter {
  public:
    virtual ~Reporter();

    // Some corruption was detected.  "size" is the approximate number
    // of bytes dropped due to the corruption.
    virtual void Corruption(size_t bytes, const Status& status) = 0;
  };
  // TODO
};
}
}

#endif // DB_LOG_READER_H

