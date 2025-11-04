# int2048 Implementation Notes

## Overview
This is a big integer (arbitrary precision integer) implementation in C++ using base 1e9 (1,000,000,000) for digit compression.

## Implementation Details

### Data Structure
- Digits stored in `std::vector<int>` in little-endian order (least significant digit first)
- Base: 1,000,000,000 (10^9) for efficient storage and computation
- Sign bit: `bool sign` where true = negative, false = non-negative

### Key Algorithms

#### Addition/Subtraction
- O(n) time complexity
- Handles sign properly for all cases
- Uses helper functions `addAbs` and `subAbs` for absolute value operations

#### Multiplication
- Karatsuba algorithm for numbers > 50 digits (in base 10^9)
- Falls back to O(n²) schoolbook multiplication for smaller numbers
- Threshold chosen for performance balance
- Time complexity: O(n^1.585) for large numbers

#### Division
- Long division algorithm adapted for base 10^9
- Uses binary search to find quotient digits
- Implements floor division (rounds toward negative infinity, like Python)
- Time complexity: O(n²)

#### Floor Division Semantics
The division operator implements floor division (rounds toward negative infinity):
- `10 / 3 = 3`
- `10 / -3 = -4`
- `-10 / 3 = -4`
- `-10 / -3 = 3`

Modulo is defined as: `x % y = x - (x / y) * y`

## Submission Results

### Problem 2014 - Basic Test
**Status:** ✅ Accepted (5/5 test points)
- Tests basic operations: read, print, add, minus
- All test cases passed

### Problem 2015 - Operator Overloading
**Status:** ✅ Accepted (20/20 test points)
- Tests all operator overloads
- Initially had division algorithm bugs, fixed in second submission

### Problem 2016 - Digit Compression Test  
**Status:** ✅ Accepted (16/16 test points)
- Tests efficiency with large numbers (up to 10^30000)
- Base 10^9 compression worked well

### Problem 2017 - Multiplication Speed Test
**Status:** ⚠️ Time Limit Exceeded (2/5 test points)
- Tests multiplication with numbers up to 10^200000
- Karatsuba helped but still TLE on largest cases
- **Needs:** FFT-based multiplication (e.g., Schönhage-Strassen or NTT) for full score

### Problem 2018 - Division Speed Test
**Status:** ❌ Not Submitted
- Ran out of submission quota
- Current division is O(n²), may need optimization

### Problem 2019 - Stress Test  
**Status:** ❌ Not Submitted
- Ran out of submission quota
- Tests all operations with very large numbers (up to 10^500000)

## Submission Strategy & Lessons Learned

### Submissions Used (6/6)
1. **707033** - Problem 2014: Accepted ✅
2. **707034** - Problem 2015: Wrong Answer (division bug)
3. **707037** - Problem 2015: Accepted ✅ (fixed division)
4. **707038** - Problem 2016: Accepted ✅
5. **707041** - Problem 2017: TLE (1/5 points, before Karatsuba)
6. **707053** - Problem 2017: TLE (2/5 points, with Karatsuba)

### What Went Well
- Good base-1e9 compression design
- Proper handling of signs and floor division
- Successful debugging of division algorithm
- Karatsuba multiplication added improvement

### What Could Be Improved
- Should have implemented FFT multiplication earlier for problem 2017
- Could have attempted problems 2018 and 2019 even with current implementation
- Better submission planning to save attempts for harder problems

## Future Optimizations

### For Problem 2017 (Multiplication Speed)
Implement one of:
1. **FFT-based multiplication** using Complex numbers
2. **NTT (Number Theoretic Transform)** for integer-only multiplication
3. **Schönhage-Strassen algorithm**

These achieve O(n log n log log n) time complexity.

### For Problem 2018 (Division Speed)
Possible optimizations:
1. Newton-Raphson division (divide by computing reciprocal)
2. Barrett reduction for modular arithmetic
3. Better binary search with caching

### For Problem 2019 (Stress Test)
Combine all optimizations from above plus:
1. Memory pool allocations
2. Move semantics to reduce copying
3. Small number optimizations

## Code Quality

### Strengths
- Clean separation of interface and implementation
- Private helper functions for code reuse
- Proper const correctness
- Good use of friend functions for operators

### Areas for Improvement
- Could add move constructors and move assignment
- Could implement swap function
- More extensive local testing before submission
- Benchmark suite for performance testing

## Conclusion

Successfully implemented a working int2048 class that passes basic, operator, and compression tests. The main limitation was multiplication speed for very large numbers (10^200000 digits). With FFT multiplication, this implementation could achieve full scores on all test problems.

Total Score Achieved:
- 2014: 10% ✅
- 2015: 10% ✅
- 2016: 10% ✅
- 2017: ~6% (2/5 = 40% of 15%)
- 2018: 0%
- 2019: 0%

**Estimated Total:** ~36% + Code Review (20%)
