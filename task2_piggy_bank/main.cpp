#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>

using Operation = bool (*)(int& balance, int amount);

const int kMaxBalance = std::numeric_limits<int>::max();

bool Deposit(int& balance, int amount) {
  if (amount <= 0 || amount > kMaxBalance - balance) {
    return false;
  }
  balance += amount;
  return true;
}

bool Withdraw(int& balance, int amount) {
  if (amount <= 0 || amount > balance) {
    return false;
  }
  balance -= amount;
  return true;
}

bool PrintBalance(int& balance, int) {
  std::cout << balance << '\n';
  return true;
}

bool Reset(int& balance, int) {
  balance = 0;
  return true;
}

bool DoubleBalance(int& balance, int) {
  if (balance > kMaxBalance / 2) {
    return false;
  }
  balance *= 2;
  return true;
}

struct Command {
  const char* name;
  Operation action;
  bool needs_amount;
};

const Command kCommands[] = {
    {"deposit", Deposit, true},       {"withdraw", Withdraw, true},
    {"balance", PrintBalance, false}, {"reset", Reset, false},
    {"double", DoubleBalance, false},
};
const int kCommandCount = sizeof(kCommands) / sizeof(kCommands[0]);

const Command* FindCommand(const char* name) {
  for (int i = 0; i < kCommandCount; ++i) {
    if (std::strcmp(kCommands[i].name, name) == 0) {
      return &kCommands[i];
    }
  }
  return nullptr;
}

void SkipLine() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
  int balance = 0;
  char name[32];

  while (std::cin >> std::setw(sizeof(name)) >> name) {
    if (std::strcmp(name, "stop") == 0) {
      break;
    }

    const Command* command = FindCommand(name);
    if (command == nullptr) {
      std::cout << "error\n";
      SkipLine();
      continue;
    }

    int amount = 0;
    if (command->needs_amount && !(std::cin >> amount)) {
      std::cin.clear();
      SkipLine();
      std::cout << "error\n";
      continue;
    }

    if (!command->action(balance, amount)) {
      std::cout << "error\n";
    }
  }
}
