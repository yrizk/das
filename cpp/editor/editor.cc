#include "editor.h"
#include <fstream>
#include <iostream>

using namespace std;
Editor::Editor() {
  buffer_ = Buffer();
  cursor_ = make_shared<Cursor>();
}

void Editor::Close() { cout << "Empty implementation for close()" << endl; }

// creates a buffer with the minimum size.
string read_file(string filepath) {
  ifstream file(filepath);
  if (!file) {
    return NULL;  // safe since our return type is not a pointer.
  }
  string content((istreambuf_iterator<char>(file)),
                 (istreambuf_iterator<char>()));
  return content;
}

void Editor::Open(string filepath) {
  string content = read_file(filepath);
  cout << "Content of file: " << content << endl;
}

Editor::~Editor() {}

int main(int argc, char** argv) {
  shared_ptr<Editor> editor = make_shared<Editor>();
  editor->Open("./tmp.txt");
  while (1) {
    char c;
    cin >> c;
    switch (c) {
      case 'q':
        editor->Close();
        return 0;
    }
  }
}
