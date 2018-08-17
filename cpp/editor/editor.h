#ifndef EDITOR_H
#define EDITOR_H

#include <memory>
#include "buffer.h"
#include "cursor.h"

using namespace std;

class Editor {
 public:
  virtual ~Editor();

 private:
  shared_ptr<Buffer> buffer_;
  shared_ptr<Cursor> cursor_;
};

#endif  // EDITOR_H
