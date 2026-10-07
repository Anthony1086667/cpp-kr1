#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>

const int kRows = 5;
const int kCols = 10;

bool IsInside(int x, int y) {
  return x >= 0 && x < kCols && y >= 0 && y < kRows;
}

void Clear(char board[kRows][kCols]) {
  for (int y = 0; y < kRows; ++y) {
    for (int x = 0; x < kCols; ++x) {
      board[y][x] = ' ';
    }
  }
}

bool Put(char board[kRows][kCols], int x, int y, char c) {
  if (!IsInside(x, y)) {
    return false;
  }
  board[y][x] = c;
  return true;
}

bool Text(char board[kRows][kCols], int x, int y, const char* word) {
  int length = static_cast<int>(std::strlen(word));

  if (length == 0 || !IsInside(x, y) || !IsInside(x + length - 1, y)) {
    return false;
  }
  for (int i = 0; i < length; ++i) {
    Put(board, x + i, y, word[i]);
  }
  return true;
}

void Print(const char board[kRows][kCols]) {
  for (int y = 0; y < kRows; ++y) {
    std::cout.write(board[y], kCols);
    std::cout << '\n';
  }
}

void SkipLine() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
  char board[kRows][kCols];
  Clear(board);

  char command[16];
  while (std::cin >> std::setw(sizeof(command)) >> command) {
    if (std::strcmp(command, "quit") == 0) {
      break;
    }

    bool ok = true;
    if (std::strcmp(command, "clear") == 0) {
      Clear(board);
    } else if (std::strcmp(command, "print") == 0) {
      Print(board);
    } else if (std::strcmp(command, "put") == 0) {
      int x = 0;
      int y = 0;
      char c = ' ';
      ok = (std::cin >> x >> y >> c) && Put(board, x, y, c);
    } else if (std::strcmp(command, "text") == 0) {
      int x = 0;
      int y = 0;
      char word[64];
      ok = (std::cin >> x >> y >> std::setw(sizeof(word)) >> word) &&
           Text(board, x, y, word);
    } else {
      ok = false;
    }

    if (!ok) {
      std::cout << "error\n";
      std::cin.clear();
      SkipLine();
    }
  }
}
