#include <iostream>
#include <string>
#include <vector>

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

std::string &&TrimInt(std::string &&fig) {
  size_t first_non_zero{fig.find_first_not_of('0')};
  if (first_non_zero == std::string::npos) {
    fig.clear();
  } else {
    fig.erase(0, first_non_zero);
  }
  return std::move(fig);
}

std::string Add(const std::string &fig1, const std::string &fig2) {
  if (fig1.empty()) {
    return fig2;
  }
  if (fig2.empty()) {
    return fig1;
  }
  size_t length{fig1.size() > fig2.size() ? fig1.size() : fig2.size()};
  int higher_digit{0};
  std::vector<int> result_vector;
  for (size_t i = 0; i < length; ++i) {
    int sum{higher_digit};
    if (i >= fig1.size()) {
      sum += fig2[fig2.size() - i - 1] - '0';
    } else if (i >= fig2.size()) {
      sum += fig1[fig1.size() - i - 1] - '0';
    } else {
      sum += fig1[fig1.size() - i - 1] - '0' + fig2[fig2.size() - i - 1] - '0';
    }
    result_vector.push_back(sum % 10);
    higher_digit = sum / 10;
  }
  if (higher_digit > 0) {
    result_vector.push_back(higher_digit);
  }
  std::string result_string;
  for (auto i = result_vector.rbegin(); i < result_vector.rend(); ++i) {
    result_string += std::to_string(*i);
  }
  return result_string;
}

std::string Sub(const std::string &fig1, const std::string &fig2) {
  if (fig1.empty() || fig1 <= fig2) {
    return "";
  }
  if (fig2.empty()) {
    return fig1;
  }
  int higher_digit{0};
  std::vector<int> result_vector;
  for (size_t i = 0; i < fig1.size(); ++i) {
    int sub{higher_digit};
    if (i >= fig2.size()) {
      sub += fig1[fig1.size() - i - 1] - '0';
    } else {
      sub +=
          fig1[fig1.size() - i - 1] - '0' - (fig2[fig2.size() - i - 1] - '0');
    }
    if (sub < 0) {
      result_vector.push_back(sub + 10);
      higher_digit = -1;
    } else {
      result_vector.push_back(sub);
      higher_digit = 0;
    }
  }
  std::string result_string;
  for (auto i = result_vector.rbegin(); i < result_vector.rend(); ++i) {
    result_string += std::to_string(*i);
  }
  return result_string;
}

std::string Multi(const std::string &fig1, const std::string &fig2) {
  if (fig1.empty()) {
    return "";
  }
  if (fig2.empty()) {
    return "";
  }
  std::vector<int> result_vector(fig1.size() + fig2.size(), 0);
  for (size_t j = 0; j < fig1.size(); ++j) {
    int higher_digit{0};
    for (size_t i = 0; i < fig2.size(); ++i) {
      int temp{result_vector[j + i] + higher_digit +
               (fig1[fig1.size() - j - 1] - '0') *
                   (fig2[fig2.size() - i - 1] - '0')};
      higher_digit = temp / 10;
      result_vector[j + i] = temp % 10;
    }
    result_vector[j + fig2.size()] += higher_digit;
  }
  std::string result_string;
  for (auto i = result_vector.rbegin(); i < result_vector.rend(); ++i) {
    result_string += std::to_string(*i);
  }
  return result_string;
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
  std::cout << "Sum is " << Add(fig1, fig2) << std::endl;
  std::cout << "Sub is " << TrimInt(Sub(fig1, fig2)) << std::endl;
  std::cout << "Multi is " << TrimInt(Multi(fig1, fig2)) << std::endl;
  return 0;
}