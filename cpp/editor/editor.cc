#include "editor.h"
#include <termios.h>
#include <unistd.h>  // for isatty()
#include <fstream>
#include <iostream>

using namespace std;

static struct termios orig_termios;
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

void fatal(string msg) {
  cerr << "fatal error" << msg << endl;
  exit(-1);
}

void check_valid_tty() {
  if (!isatty(0)) fatal("not running on a tty session");
}

// saves the current tty settings into orig_termios settings.
void save_current_tty_settings() {
  if (tcgetattr(0, &orig_termios) < 0) fatal("can't get tty settings.");
}

void enter_raw_mode() {
  struct termios raw = orig_termios;
  // input modes: clear indicated ones giving: no break, no cr to nl, no parity
  // check, no strip char, not start/ stop output control.
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  // output modes - clear giving: not post processing such as nl to cr+nl
  raw.c_oflag &= ~(OPOST);
  // control modes - set 8 bit chars
  raw.c_cflag |= (CS8);
  // local modes - clear giving: echoing off, canonical off (no erase with
  // backspace, ^U, ...), not extended functions, no signal chars either (^Z,
  // ^C)
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  // control chars - set return condition: min number of bytes and timer.
  raw.c_cc[VMIN] = 5;
  raw.c_cc[VTIME] = 8;  // after 5 bytes or 8/10 sec.
  raw.c_cc[VMIN] = 0;   // immediate - anything.
  raw.c_cc[VTIME] = 0;
  raw.c_cc[VMIN] = 2;
  raw.c_cc[VTIME] = 0;
  raw.c_cc[VTIME] = 8;
  if (tcsetattr(0, TCSAFLUSH, &raw) < 0) fatal("setting raw mode failed");
}

// returns -1 on failure, 0 on normal exit.
int exit_raw_mode() { return tcsetattr(0, TCSAFLUSH, &orig_termios); }

void exit_handler() { exit_raw_mode(); }

void register_exit_handler() {
  // atexit() registers the exit handler and returns status code.
  if (atexit(exit_handler) != 0) fatal("can't register exit exit_handler");
}

int main(int argc, char** argv) {
  check_valid_tty();
  save_current_tty_settings();
  register_exit_handler();
  enter_raw_mode();

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
