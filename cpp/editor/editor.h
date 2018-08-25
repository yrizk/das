#ifndef EDITOR_H
#define EDITOR_H

#include <memory>
#include "buffer.h"
#include "cursor.h"

using namespace std;

class Editor {
 public:
  Editor();
  ~Editor();
  // pure virtual: opens the content of the file.
  void Open(std::string filepath);
  // pure virtual: just closes the open buffer, and tears down the member
  // variables.
  void Close();

 private:
  Buffer buffer_;
  std::shared_ptr<Cursor> cursor_;
};

#endif  // EDITOR_H
