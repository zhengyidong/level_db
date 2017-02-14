#ifndef LEVELDB_ENV_H
#define LEVELDB_ENV_H

#include <cstdarg>
#include <vector>
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

  // Create a brand new random access read-only file with the
  // specified name.  On success, stores a pointer to the new file in
  // *result and returns OK.  On failure stores NULL in *result and
  // returns non-OK.  If the file does not exist, returns a non-OK
  // status.
  //
  // The returned file may be concurrently accessed by multiple threads.
  virtual Status NewRandomAccessFile(const std::string& fname,
                                     RandomAccessFile** result) = 0;

  virtual Status NewWritableFile(const std::string& fname,
                                 WritableFile** result) = 0;

  // Returns true iff the named file exists.
  virtual bool FileExists(const std::string &fname) = 0;

  // Store in *result the names of the children of the specified directory.
  // The names are relative to "dir".
  // Original contents of *results are dropped.
  virtual Status GetChildren(const std::string& dir,
                             std::vector<std::string>* result) = 0;

  // Delete the named file.
  virtual Status DeleteFile(const std::string &fname) = 0;

  // Create the specified directory.
  virtual Status CreateDir(const std::string &dirname) = 0;

  // Delete the specified directory.
  virtual Status DeleteDir(const std::string &dirname) = 0;

  // Store the size of fname in *file_size.
  virtual Status GetFileSize(const std::string& fname, uint64_t* file_size) = 0;

  // Rename file src to target.
  virtual Status RenameFile(const std::string& src,
                            const std::string& target) = 0;

  // Lock the specified file.  Used to prevent concurrent access to
  // the same db by multiple processes.  On failure, stores NULL in
  // *lock and returns non-OK.
  //
  // On success, stores a pointer to the object that represents the
  // acquired lock in *lock and returns OK.  The caller should call
  // UnlockFile(*lock) to release the lock.  If the process exits,
  // the lock will be automatically released.
  //
  // If somebody else already holds the lock, finishes immediately
  // with a failure.  I.e., this call does not wait for existing locks
  // to go away.
  //
  // May create the named file if it does not already exist.
  virtual Status LockFile(const std::string& fname, FileLock** lock) = 0;

  // Release the lock acquired by a previous successful call to LockFile.
  // REQUIRES: lock was returned by a successful LockFile() call
  // REQUIRES: lock has not already been unlocked.
  virtual Status UnlockFile(FileLock* lock) = 0;

  // Arrange to run "(*function)(arg)" once in a background thread.
  //
  // "function" may run in an unspecified thread.  Multiple functions
  // added to the same Env may run concurrently in different threads.
  // I.e., the caller may not assume that background work items are
  // serialized.
  virtual void Schedule(
      void (*function)(void* arg),
      void* arg) = 0;

  // Create and return a log file for storing informational messages.
  virtual Status NewLogger(const std::string& fname, Logger** result) = 0;

  // Returns the number of micro-seconds since some fixed point in time. Only
  // useful for computing deltas of time.
  virtual uint64_t NowMicros() = 0;

  // Sleep/delay the thread for the perscribed number of micro-seconds.
  virtual void SleepForMicroseconds(int micros) = 0;

 private:
  // No copying allowed
  Env(const Env&);
  void operator=(const Env&);
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
  FileLock() {}
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

// A utility routine: read contents of named file into *data
extern Status ReadFileToString(Env *v, const std::string &fname,
                               std::string *data);


}


#endif // LEVELDB_ENV_H

