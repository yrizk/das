#ifndef EDITOR_H
#define EDITOR_H

#include <memory>
#include "buffer.h"
#include "cursor.h"

using namespace std;

class Editor {
 public:
  Editor();
  virtual ~Editor();
  // pure virtual: opens the content of the file.
  void Open(string filepath);
  // pure virtual: just closes the open buffer, and tears down the member
  // variables.
  void Close();

 private:
  shared_ptr<Buffer> buffer_;
  shared_ptr<Cursor> cursor_;
};

#endif  // EDITOR_H
