#ifndef LEVELDB_ENV_H
#define LEVELDB_ENV_H

#include <cstdarg>
#include <stdint.h>
#include "leveldb/status.h"

namespace leveldb {
class FileLock;
class Logger;
class RandomAccessFile;
class SequentialFile;
class WritableFile;

class SequentialFile;

class Env {
public:
  Env() {}
  virtual ~Env();
  static Env *Default();
  virtual Status NewSequentialFile(const std::string& fname,
                                   SequentialFile** result) = 0;
  virtual Status NewWritableFile(const std::string& fname,
                                 WritableFile** result) = 0;
  virtual Status DeleteFile(const std::string& fname) = 0;
};

class SequentialFile {
public:
  SequentialFile() {}
  virtual ~SequentialFile();
  virtual Status Read(size_t n, Slice *result, char *scrach) = 0;
  virtual Status Skip(uint64_t n) = 0;

private:
  // No copying allowed
  SequentialFile(const SequentialFile&);
  void operator=(const SequentialFile&);
};

class RandomAccessFile {
public:
  RandomAccessFile() {}
  virtual ~RandomAccessFile();
  virtual Status Read(uint64_t offset, size_t n, Slice *result, char *scratch) const = 0;

private:
  // No copying allowed
  RandomAccessFile(const RandomAccessFile&);
  void operator=(const RandomAccessFile&);
};

class WritableFile {
 public:
  WritableFile() { }
  virtual ~WritableFile();

  virtual Status Append(const Slice &data) = 0;
  virtual Status Close() = 0;
  virtual Status Flush() = 0;
  virtual Status Sync() = 0;

 private:
  // No copying allowed
  WritableFile(const WritableFile&);
  void operator=(const WritableFile&);
};

class Logger {
 public:
  Logger() {}
  virtual ~Logger();

  // Write an entry to the log file with the specified format.
  virtual void Logv(const char *format, va_list ap) = 0;

 private:
  // No copying allowed
  Logger(const Logger&);
  void operator=(const Logger&);
};

// Identifies a locked file.
class FileLock {
 public:
  FileLock() { }
  virtual ~FileLock();
 private:
  // No copying allowed
  FileLock(const FileLock&);
  void operator=(const FileLock&);
};

extern void Log(Logger *info_log, const char *format, ...)
#  if defined(__GNUC__) || defined(__clang__)
  __attribute__((__format__(__printf__, 2, 3)))
#  endif
;

extern Status WriteStringToFile(Env *v, const Slice &data, const std::string &fname);
extern Status ReadFileTOString(Env *v, const std::string &fname, std::string *data);


}


#endif // LEVELDB_ENV_H

