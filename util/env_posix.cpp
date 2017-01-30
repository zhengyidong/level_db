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

class PosixEnv : public Env {
public:
  virtual Status NewSequentialFile(const std::string &fname, SequentialFile **result) {
    FILE *f = fopen(fname.c_str(), "r");
    if (f == NULL) {
      *result = NULL;
      return IOError(fname, errno);
    } else {
      *result = new PosixSequentialFile(fname, f);
      return Status::OK();
    }
  }

  virtual Status NewWritableFile(const std::string &fname, WritableFile **result) {
    return Status::NotFound("");
  }

  virtual Status DeleteFile(const std::string &fname) {
    Status result;
    if (unlink(fname.c_str()) != 0) {
      result = IOError(fname, errno);
    }
    return result;
  }

  virtual Status RenameFile(const std::string &src, const std::string &target) {
    Status result;
    if (rename(src.c_str(), target.c_str()) != 0) {
      result = IOError(src, errno);
    }
    return result;
  }

};
}

static pthread_once_t once = PTHREAD_ONCE_INIT;
static Env* default_env;
static void InitDefaultEnv() { default_env = new PosixEnv; }

Env* Env::Default() {
  pthread_once(&once, InitDefaultEnv);
  return default_env;
}

}
