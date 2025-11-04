#include "include/int2048.h"
#include <algorithm>

namespace sjtu {

// Use base 1e9 for digit compression
const int BASE = 1000000000;
const int BASE_DIGITS = 9;

void int2048::normalize() {
  while (digits.size() > 1 && digits.back() == 0) {
    digits.pop_back();
  }
  if (digits.empty()) {
    digits.push_back(0);
    sign = false;
  }
  if (digits.size() == 1 && digits[0] == 0) {
    sign = false;
  }
}

// Compare absolute values: returns -1 if |a| < |b|, 0 if equal, 1 if |a| > |b|
int int2048::compareAbs(const int2048 &a, const int2048 &b) {
  if (a.digits.size() != b.digits.size()) {
    return a.digits.size() < b.digits.size() ? -1 : 1;
  }
  for (int i = a.digits.size() - 1; i >= 0; i--) {
    if (a.digits[i] != b.digits[i]) {
      return a.digits[i] < b.digits[i] ? -1 : 1;
    }
  }
  return 0;
}

// Add absolute values
int2048 int2048::addAbs(const int2048 &a, const int2048 &b) {
  int2048 result;
  result.digits.clear();
  long long carry = 0;
  for (size_t i = 0; i < std::max(a.digits.size(), b.digits.size()) || carry; i++) {
    long long sum = carry;
    if (i < a.digits.size()) sum += a.digits[i];
    if (i < b.digits.size()) sum += b.digits[i];
    result.digits.push_back(sum % BASE);
    carry = sum / BASE;
  }
  result.sign = false;
  return result;
}

// Subtract absolute values (assuming |a| >= |b|)
int2048 int2048::subAbs(const int2048 &a, const int2048 &b) {
  int2048 result;
  result.digits.clear();
  long long borrow = 0;
  for (size_t i = 0; i < a.digits.size(); i++) {
    long long diff = a.digits[i] - borrow;
    if (i < b.digits.size()) diff -= b.digits[i];
    if (diff < 0) {
      diff += BASE;
      borrow = 1;
    } else {
      borrow = 0;
    }
    result.digits.push_back(diff);
  }
  result.normalize();
  result.sign = false;
  return result;
}

int2048::int2048() : sign(false) { digits.push_back(0); }

int2048::int2048(long long x) {
  sign = x < 0;
  if (x == 0) {
    digits.push_back(0);
    return;
  }
  if (sign) x = -x;
  while (x > 0) {
    digits.push_back(x % BASE);
    x /= BASE;
  }
}

int2048::int2048(const std::string &s) {
  read(s);
}

int2048::int2048(const int2048 &other) : digits(other.digits), sign(other.sign) {}

void int2048::read(const std::string &s) {
  digits.clear();
  sign = false;
  
  int start = 0;
  if (s[0] == '-') {
    sign = true;
    start = 1;
  } else if (s[0] == '+') {
    start = 1;
  }
  
  // Read in chunks of BASE_DIGITS from right to left
  for (int i = s.length(); i > start; i -= BASE_DIGITS) {
    int len = std::min(BASE_DIGITS, i - start);
    int num = 0;
    for (int j = i - len; j < i; j++) {
      num = num * 10 + (s[j] - '0');
    }
    digits.push_back(num);
  }
  
  normalize();
}

void int2048::print() {
  if (sign) std::cout << '-';
  std::cout << digits.back();
  for (int i = digits.size() - 2; i >= 0; i--) {
    std::cout.width(BASE_DIGITS);
    std::cout.fill('0');
    std::cout << digits[i];
  }
}

// Forward declarations for friend functions
int2048 add(int2048, const int2048 &);
int2048 minus(int2048, const int2048 &);

int2048 &int2048::add(const int2048 &other) {
  int2048 result = ::sjtu::add(*this, other);
  *this = result;
  return *this;
}

int2048 add(int2048 a, const int2048 &b) {
  if (a.sign == b.sign) {
    int2048 result = int2048::addAbs(a, b);
    result.sign = a.sign;
    return result;
  } else {
    int cmp = int2048::compareAbs(a, b);
    if (cmp == 0) {
      return int2048(0);
    } else if (cmp > 0) {
      int2048 result = int2048::subAbs(a, b);
      result.sign = a.sign;
      return result;
    } else {
      int2048 result = int2048::subAbs(b, a);
      result.sign = b.sign;
      return result;
    }
  }
}

int2048 &int2048::minus(const int2048 &other) {
  int2048 result = ::sjtu::minus(*this, other);
  *this = result;
  return *this;
}

int2048 minus(int2048 a, const int2048 &b) {
  int2048 neg_b = b;
  neg_b.sign = !neg_b.sign;
  if (neg_b.digits.size() == 1 && neg_b.digits[0] == 0) {
    neg_b.sign = false;
  }
  return add(a, neg_b);
}

int2048 int2048::operator+() const {
  return *this;
}

int2048 int2048::operator-() const {
  int2048 result = *this;
  if (!(result.digits.size() == 1 && result.digits[0] == 0)) {
    result.sign = !result.sign;
  }
  return result;
}

int2048 &int2048::operator=(const int2048 &other) {
  if (this != &other) {
    digits = other.digits;
    sign = other.sign;
  }
  return *this;
}

int2048 &int2048::operator+=(const int2048 &other) {
  return add(other);
}

int2048 operator+(int2048 a, const int2048 &b) {
  return add(a, b);
}

int2048 &int2048::operator-=(const int2048 &other) {
  return minus(other);
}

int2048 operator-(int2048 a, const int2048 &b) {
  return minus(a, b);
}

int2048 &int2048::operator*=(const int2048 &other) {
  *this = (*this) * other;
  return *this;
}

// Helper for Karatsuba multiplication
int2048 int2048::multiplyAbs(const int2048 &a, const int2048 &b) {
  const size_t KARATSUBA_THRESHOLD = 50;
  
  if (a.digits.size() <= KARATSUBA_THRESHOLD || b.digits.size() <= KARATSUBA_THRESHOLD) {
    // Use standard multiplication for small numbers
    int2048 result;
    result.digits.assign(a.digits.size() + b.digits.size(), 0);
    
    for (size_t i = 0; i < a.digits.size(); i++) {
      long long carry = 0;
      for (size_t j = 0; j < b.digits.size() || carry; j++) {
        long long cur = result.digits[i + j] + carry;
        if (j < b.digits.size()) {
          cur += (long long)a.digits[i] * b.digits[j];
        }
        result.digits[i + j] = cur % BASE;
        carry = cur / BASE;
      }
    }
    
    result.normalize();
    result.sign = false;
    return result;
  }
  
  // Karatsuba algorithm
  size_t mid = std::max(a.digits.size(), b.digits.size()) / 2;
  
  int2048 a0, a1, b0, b1;
  a0.digits.assign(a.digits.begin(), a.digits.begin() + std::min(mid, a.digits.size()));
  if (mid < a.digits.size()) {
    a1.digits.assign(a.digits.begin() + mid, a.digits.end());
  } else {
    a1.digits.push_back(0);
  }
  
  b0.digits.assign(b.digits.begin(), b.digits.begin() + std::min(mid, b.digits.size()));
  if (mid < b.digits.size()) {
    b1.digits.assign(b.digits.begin() + mid, b.digits.end());
  } else {
    b1.digits.push_back(0);
  }
  
  a0.sign = false;
  a1.sign = false;
  b0.sign = false;
  b1.sign = false;
  a0.normalize();
  a1.normalize();
  b0.normalize();
  b1.normalize();
  
  int2048 z0 = multiplyAbs(a0, b0);
  int2048 z2 = multiplyAbs(a1, b1);
  
  int2048 a0_plus_a1 = int2048::addAbs(a0, a1);
  int2048 b0_plus_b1 = int2048::addAbs(b0, b1);
  int2048 z1 = multiplyAbs(a0_plus_a1, b0_plus_b1);
  z1 = int2048::subAbs(z1, z0);
  z1 = int2048::subAbs(z1, z2);
  
  // result = z0 + z1 * BASE^mid + z2 * BASE^(2*mid)
  int2048 result = z0;
  
  // Add z1 * BASE^mid
  if (z1.digits.size() > 1 || z1.digits[0] != 0) {
    for (size_t i = 0; i < mid; i++) {
      z1.digits.insert(z1.digits.begin(), 0);
    }
    result = int2048::addAbs(result, z1);
  }
  
  // Add z2 * BASE^(2*mid)
  if (z2.digits.size() > 1 || z2.digits[0] != 0) {
    for (size_t i = 0; i < 2 * mid; i++) {
      z2.digits.insert(z2.digits.begin(), 0);
    }
    result = int2048::addAbs(result, z2);
  }
  
  result.normalize();
  result.sign = false;
  return result;
}

int2048 operator*(int2048 a, const int2048 &b) {
  int2048 result = int2048::multiplyAbs(a, b);
  result.sign = (a.sign != b.sign);
  if (result.digits.size() == 1 && result.digits[0] == 0) {
    result.sign = false;
  }
  return result;
}

int2048 &int2048::operator/=(const int2048 &other) {
  *this = (*this) / other;
  return *this;
}

int2048 operator/(int2048 a, const int2048 &b) {
  bool result_sign = (a.sign != b.sign);
  
  // For floor division: if signs differ and there's a remainder, adjust
  int2048 a_abs = a;
  a_abs.sign = false;
  int2048 b_abs = b;
  b_abs.sign = false;
  
  if (int2048::compareAbs(a_abs, b_abs) < 0) {
    if (result_sign) {
      return int2048(-1);
    }
    return int2048(0);
  }
  
  int2048 result;
  result.digits.assign(a_abs.digits.size(), 0);
  int2048 current;
  current.digits.clear();
  
  for (int i = a_abs.digits.size() - 1; i >= 0; i--) {
    current.digits.insert(current.digits.begin(), a_abs.digits[i]);
    current.normalize();
    
    int left = 0, right = BASE - 1;
    while (left < right) {
      int mid = (left + right + 1) / 2;
      // Multiply b_abs by mid
      int2048 temp;
      temp.digits.assign(b_abs.digits.size(), 0);
      long long carry = 0;
      for (size_t j = 0; j < b_abs.digits.size() || carry; j++) {
        long long cur = carry;
        if (j < b_abs.digits.size()) {
          cur += (long long)b_abs.digits[j] * mid;
        }
        if (j >= temp.digits.size()) temp.digits.push_back(0);
        temp.digits[j] = cur % BASE;
        carry = cur / BASE;
      }
      temp.normalize();
      
      if (int2048::compareAbs(current, temp) >= 0) {
        left = mid;
      } else {
        right = mid - 1;
      }
    }
    
    result.digits[i] = left;
    
    // Multiply b_abs by left
    int2048 temp;
    temp.digits.assign(b_abs.digits.size(), 0);
    long long carry = 0;
    for (size_t j = 0; j < b_abs.digits.size() || carry; j++) {
      long long cur = carry;
      if (j < b_abs.digits.size()) {
        cur += (long long)b_abs.digits[j] * left;
      }
      if (j >= temp.digits.size()) temp.digits.push_back(0);
      temp.digits[j] = cur % BASE;
      carry = cur / BASE;
    }
    temp.normalize();
    
    current = int2048::subAbs(current, temp);
  }
  
  result.normalize();
  
  // Check if we need to adjust for floor division
  if (result_sign) {
    // Check if there's a remainder
    int2048 check = result * b_abs;
    if (int2048::compareAbs(check, a_abs) != 0) {
      result = result + int2048(1);
    }
    result.sign = true;
  } else {
    result.sign = false;
  }
  
  if (result.digits.size() == 1 && result.digits[0] == 0) {
    result.sign = false;
  }
  
  return result;
}

int2048 &int2048::operator%=(const int2048 &other) {
  *this = (*this) % other;
  return *this;
}

int2048 operator%(int2048 a, const int2048 &b) {
  int2048 quotient = a / b;
  return a - quotient * b;
}

std::istream &operator>>(std::istream &is, int2048 &num) {
  std::string s;
  is >> s;
  num.read(s);
  return is;
}

std::ostream &operator<<(std::ostream &os, const int2048 &num) {
  if (num.sign) os << '-';
  os << num.digits.back();
  for (int i = num.digits.size() - 2; i >= 0; i--) {
    os.width(BASE_DIGITS);
    os.fill('0');
    os << num.digits[i];
  }
  return os;
}

bool operator==(const int2048 &a, const int2048 &b) {
  return a.sign == b.sign && a.digits == b.digits;
}

bool operator!=(const int2048 &a, const int2048 &b) {
  return !(a == b);
}

bool operator<(const int2048 &a, const int2048 &b) {
  if (a.sign != b.sign) return a.sign;
  
  int cmp = int2048::compareAbs(a, b);
  if (a.sign) {
    return cmp > 0;
  } else {
    return cmp < 0;
  }
}

bool operator>(const int2048 &a, const int2048 &b) {
  return b < a;
}

bool operator<=(const int2048 &a, const int2048 &b) {
  return !(a > b);
}

bool operator>=(const int2048 &a, const int2048 &b) {
  return !(a < b);
}

} // namespace sjtu
