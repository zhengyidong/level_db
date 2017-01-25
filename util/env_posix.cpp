#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "unistd.h"
#include "leveldb/env.h"
#include "leveldb/status.h"
#include "port/port.h"

namespace leveldb {
namespace {
static Status IOError(const std::string &context, int err_number) {
  return Status::IOError(context, strerror(err_number));
}

class PosixSequentialFile : public SequentialFile {
private:
  std::string filename_;
  FILE *file_;

public:
  PosixSequentialFile(const std::string &fname, FILE *f)
    : filename_(fname), file_(f) {}
  virtual ~PosixSequentialFile() { fclose(file_); }

  virtual Status Read(size_t n, Slice *result, char *scratch) {
    Status s;
    size_t r = fread_unlocked(scratch, 1, n, file_);
    *result = Slice(scratch, r);
    if (r < n) {
      if (feof(file_)) {
      } else {
        s = IOError(filename_, errno);
      }
    }
    return s;
  }

  virtual Status Skip(uint64_t n) {
    if (fseek(file_, n, SEEK_CUR)) {
      return IOError(filename_, errno);
    }
    return Status::OK();
  }
};

class PosixRandomAccessFile : RandomAccessFile {
private:
  std::string filename_;
  int fd_;

public:
  PosixRandomAccessFile(const std::string &fname, int fd)
    : filename_(fname), fd_(fd) {}
  virtual ~PosixRandomAccessFile() { close(fd_); }
  virtual Status Read(uint64_t offset, size_t n, Slice *result, char *scratch) const {
    Status s;
    ssize_t r = pread(fd_, scratch, n, static_cast<off_t>(offset));
    *result = Slice(scratch, (r < 0) ? 0 : r);
    if (r < 0) {
      s = IOError(filename_, errno);
    }
    return s;
  }
};

class MmapLimiter {
public:
  MmapLimiter() {
  }
private:
  port::AtomicPointer allowed_;
};

class PosixMmapReadableFile : public RandomAccessFile {
};
}
}
