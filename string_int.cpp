#include <iostream>
#include <string>

std::string Usage() { return "Usage: <executable> fig1 fig2\n"; }

bool IsInt(const std::string &fig) {
  for (size_t i = 0; i < fig.size(); ++i) {
    if (fig[i] - '0' < 0 || fig[i] - '0' > 9) {
      return false;
    }
  }
  return true;
}

// Removes leading zeroes if have
void TrimInt(std::string &fig) {
  size_t first_non_zero{fig.find_first_not_of('0')};
  if (first_non_zero == std::string::npos) {
    fig.clear();
  } else {
    fig.erase(0, first_non_zero);
  }
}

std::string Add(const std::string &fig1, const std::string &fig2) {
  if (fig1.empty()) {
    return fig2;
  }
  if (fig2.empty()) {
    return fig1;
  }
  std::vector<unsigned short int> result;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << Usage() << std::endl;
    return 1;
  }
  std::string fig1{argv[1]};
  std::string fig2{argv[2]};
  if (!IsInt(fig1) || !IsInt(fig2)) {
    std::cerr << "Arguments are not integer" << std::endl;
    return 1;
  }
  TrimInt(fig1);
  TrimInt(fig2);
  std::cout << '"' << fig1 << "\" and \"" << fig2 << '"' << std::endl;
  if (fig1 == fig2) {
    std::cout << "They are equal!" << std::endl;
  } else {
    std::cout << "They are not equal" << std::endl;
  }
  return 0;
}