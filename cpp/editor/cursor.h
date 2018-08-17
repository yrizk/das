#ifndef CURSOR_H
#define CURSOR_H

class Cursor {
 public:
  Cursor(int r = 0, int c = 0);
  virtual ~Cursor();

 private:
  int row_;
  int col_;
};
#endif  // CURSOR_H
