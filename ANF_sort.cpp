#include <iostream>
#include <string>
#include <bitset>
#include <cmath>
#include <algorithm>
#include <windows.h>

struct Arr {
  int special_num{};
  int original_value{};
};

std::string build_line(int l, int l0, const std::string& vec) {
  if (l == 0) {
    return "";
  }
  std::string line;
  for (int i = 0; i < l - 1; i++) {
    line += std::to_string((vec[i] - '0' + vec[i + 1] - '0') % 2);
  }
  std::string v = line;
  std::string spaced_line(l0 - l + 1, ' ');
  for (size_t i = 0; i < line.size(); ++i) {
    spaced_line += line[i];
    if (i != line.size() - 1)
      spaced_line += ' ';
  }
  spaced_line += std::string(l0 - l + 1, ' ') + '\n';
  return spaced_line + build_line(l - 1, l0, v);
}

std::string build_pascal_triangle(int l0, const std::string& f_vec) {
  std::string pascal_triangle = f_vec + '\n' + build_line(l0, l0, f_vec);
  std::string target;
  size_t pos = 0, end;
  while ((end = pascal_triangle.find('\n', pos)) != std::string::npos) {
    std::string line = pascal_triangle.substr(pos, end - pos);
    pos = end + 1;
    size_t first_non_space = line.find_first_not_of(' ');
    if (first_non_space != std::string::npos) {
      target += line[first_non_space];
    }
  }
  return target;
}

int ANF_special_number(const std::string& f_vec) {
  int k = 3;
  std::string anf;
  int degree = 0;
  int l0 = static_cast<int>(std::pow(2, k));
  std::string target = build_pascal_triangle(l0, f_vec);
  for (size_t i = 0; i < target.size(); i++) {
    if (target[i] == '1') {
      if (!anf.empty()) {
        anf += " ⊕ ";
      }
      if (i == 0) {
        anf += "1";
      } else {
        std::string args_input = std::bitset<32>(i).to_string().substr(32 - k, k);
        std::string conjunction;
        for (int j = 0; j < k; j++) {
          if (args_input[j] == '1') {
            conjunction += "x" + std::to_string(j + 1);
          }
        }
        anf += conjunction;
        degree = std::max(degree, static_cast<int>(conjunction.length()));
      }
    }
  }
  if (anf.empty()) {
    anf = "0";
  }
  std::string::difference_type n = std::count(anf.begin(), anf.end(), ' ');
  n = n / 2 + 1;
  n += degree / 2;
  // std::cout << anf << ' ' << n << '\n';
  return static_cast<int>(n);
}

void Merge(Arr* arr, int n, int len, Arr* result) {
  int right = 0;
  int left = 0;
  auto a = arr;
  auto b = arr + len;
  while (left < len && right < n - len) {
    if (a[left].special_num <= b[right].special_num) {
      result[left + right] = a[left];
      left++;
    } else {
      result[right + left] = b[right];
      right++;
    }
  }
  while (left < len) {
    result[left + right] = a[left];
    left++;
  }
  while (right < n - len) {
    result[left + right] = b[right];
    right++;
  }
  for (int i = 0; i < n; i++) {
    arr[i] = result[i];
  }
}

Arr* RecursiveMergeSort(Arr* arr, int n, Arr* buffer) {
  if (n <= 1) {
    return arr;
  }
  int mid = n / 2;
  RecursiveMergeSort(arr, mid, buffer);
  RecursiveMergeSort(arr + mid, n - mid, buffer);
  Merge(arr, n, mid, buffer);
  return arr;
}

void MergeSort(Arr* arr, int n) {
  auto* buffer = new Arr[n];
  RecursiveMergeSort(arr, n, buffer);
  delete[] buffer;
}

int main() {
  std::locale::global(std::locale(""));
 #ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
 #endif

  int n = 0;
  int num = 0;
  std::cin >> n;
  auto* arr = new Arr[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> num;
    std::string v = std::bitset<128>(num).to_string().substr(120, 8);
    arr[i].original_value = num;
    arr[i].special_num = ANF_special_number(v);
  }
  MergeSort(arr, n);
  std::cout << '\n';
  for (int i = 0; i < n; i++) {
    std::cout << arr[i].original_value << " ";
  }
  delete[] arr;
}