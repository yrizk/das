#include "editor.h"

int main(int argc, char** argv) {
  shared_ptr<Editor> editor = make_shared<Editor>();
  editor->Open("./tmp.txt");
  editor->Close();
}
