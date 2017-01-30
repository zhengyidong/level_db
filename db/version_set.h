#ifndef DB_VERSION_SET_H
#define DB_VERSION_SET_H

namespace leveldb {
class VersionSet {
private:
  Env *const env_;
  const std::string dbname_;
  const Options *const options_;
};
}

#endif // DB_VERSION_SET_H

