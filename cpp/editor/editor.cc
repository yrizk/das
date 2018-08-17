#include "editor.h"
#include <iostream>

using namespace std;

Editor::Editor() {
  buffer_ = make_shared<Buffer>();
  cursor_ = make_shared<Cursor>();
}

void Editor::Open(string filepath) {
  cout << "Open Called" << filepath << endl;
}

void Editor::Close() {
  cout << "Empty implementation for close()" << endl;
  delete &buffer_;
  delete &cursor_;
}
