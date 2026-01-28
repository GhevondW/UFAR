# Algorithms and Complexity Analysis Guide

A comprehensive guide to understanding algorithm complexity, asymptotic notations, and performance analysis with practical C++ examples.

## Table of Contents

1. [What is Algorithm Analysis?](#1-what-is-algorithm-analysis)
2. [Why Complexity Matters](#2-why-complexity-matters)
3. [Asymptotic Notations](#3-asymptotic-notations)
4. [Big O Notation (Upper Bound)](#4-big-o-notation-upper-bound)
5. [Big Omega Notation (Lower Bound)](#5-big-omega-notation-lower-bound)
6. [Big Theta Notation (Tight Bound)](#6-big-theta-notation-tight-bound)
7. [Comparing Notations](#7-comparing-notations)
8. [Common Time Complexities](#8-common-time-complexities)
9. [Space Complexity](#9-space-complexity)
10. [Recursive vs Iterative Analysis](#10-recursive-vs-iterative-analysis)
11. [Best, Average, and Worst Case](#11-best-average-and-worst-case)
12. [Practical Examples](#12-practical-examples)
13. [Common Pitfalls](#13-common-pitfalls)
14. [Quick Reference](#14-quick-reference)

---

## 1) What is Algorithm Analysis?

**Algorithm analysis** is the process of determining how much time and memory an algorithm needs to run. Instead of measuring actual runtime (which depends on the computer), we count the number of operations.

### Why Not Just Measure Execution Time?

**Problems with measuring time**:
- Depends on computer speed
- Depends on programming language
- Depends on compiler optimization
- Depends on other running programs

**Better approach**: Count operations relative to input size (n)

---

## 2) Why Complexity Matters

### The Power of Efficient Algorithms

Consider searching for a name in a phone book with 1 million entries:

| Algorithm | Operations | Time (at 1ms/op) |
|-----------|-----------|------------------|
| Linear Search | 1,000,000 | 16.7 minutes |
| Binary Search | 20 | 0.02 seconds |

**That's 50,000x faster!**

### Growth Rates Comparison

```
Input size (n):           10        100       1,000      10,000
─────────────────────────────────────────────────────────────────
O(1)      Constant:       1         1         1          1
O(log n)  Logarithmic:    3         7         10         13
O(n)      Linear:         10        100       1,000      10,000
O(n log n) Linearithmic:  30        664       9,966      132,877
O(n²)     Quadratic:      100       10,000    1,000,000  100,000,000
O(2ⁿ)     Exponential:    1,024     1.27×10³⁰ [too large] [too large]
```

---

## 3) Asymptotic Notations

Asymptotic notations describe algorithm performance as input size grows **very large** (approaching infinity).

### The Three Main Notations

```
┌─────────────────────────────────────────────────────────┐
│                                                         │
│  Big O (O)       : Upper bound (worst case)             │
│                    "At most this fast"                  │
│                                                         │
│  Big Omega (Ω)   : Lower bound (best case)              │
│                    "At least this fast"                 │
│                                                         │
│  Big Theta (Θ)   : Tight bound (average case)           │
│                    "Exactly this fast"                  │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

---

## 4) Big O Notation (Upper Bound)

**Big O** describes the **worst-case scenario** - the maximum time/space an algorithm could take.

### Formal Definition

f(n) = O(g(n)) if there exist constants c > 0 and n₀ such that:

```
0 ≤ f(n) ≤ c × g(n)  for all n ≥ n₀
```

**In simple terms**: f(n) grows no faster than g(n)

### Example 1: Constant Time - O(1)

Algorithm takes the same time regardless of input size.

```cpp
// O(1) - Constant time
int getFirstElement(int arr[], int size) {
    if (size > 0) {
        return arr[0];  // Single operation, doesn't matter how big array is
    }
    return -1;
}

// Another O(1) example
int sumTwoNumbers(int a, int b) {
    return a + b;  // Always one operation
}

// Still O(1) even with multiple operations
void printFirstThree(int arr[], int size) {
    cout << arr[0] << endl;  // Operation 1
    cout << arr[1] << endl;  // Operation 2
    cout << arr[2] << endl;  // Operation 3
    // 3 operations, but not dependent on n → O(1)
}
```

### Example 2: Linear Time - O(n)

Algorithm time grows proportionally with input size.

```cpp
// O(n) - Linear time
int findMax(int arr[], int n) {
    int max = arr[0];           // 1 operation
    
    for (int i = 1; i < n; i++) {  // n-1 iterations
        if (arr[i] > max) {     // 1 comparison per iteration
            max = arr[i];       // 1 assignment (sometimes)
        }
    }
    
    return max;                 // 1 operation
}
// Total: 1 + (n-1) + (n-1) + 1 ≈ 2n operations → O(n)
```

### Example 3: Quadratic Time - O(n²)

Algorithm time grows with the square of input size.

```cpp
// O(n²) - Quadratic time
void printAllPairs(int arr[], int n) {
    for (int i = 0; i < n; i++) {        // n iterations
        for (int j = 0; j < n; j++) {    // n iterations for each i
            cout << arr[i] << ", " << arr[j] << endl;
        }
    }
}
// Total operations: n × n = n² → O(n²)
```

### Example 4: Logarithmic Time - O(log n)

Algorithm divides the problem in half each time.

```cpp
// O(log n) - Logarithmic time (Binary Search)
int binarySearch(int arr[], int n, int target) {
    int left = 0, right = n - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;  // Found
        }
        
        if (arr[mid] < target) {
            left = mid + 1;   // Search right half
        } else {
            right = mid - 1;  // Search left half
        }
    }
    
    return -1;  // Not found
}
// Each iteration cuts search space in half
// n → n/2 → n/4 → n/8 → ... → 1
// Number of iterations: log₂(n) → O(log n)
```

### Example 5: Linearithmic Time - O(n log n)

Common in efficient sorting algorithms.

```cpp
// O(n log n) - Merge Sort
void merge(int arr[], int left, int mid, int right) {
    // Merging takes O(n) time
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    int L[n1], R[n2];
    
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        mergeSort(arr, left, mid);      // Divide: log n levels
        mergeSort(arr, mid + 1, right); // Divide: log n levels
        merge(arr, left, mid, right);   // Conquer: O(n) work
    }
}
// Total: O(n) work at each of O(log n) levels → O(n log n)
```

---

## 5) Big Omega Notation (Lower Bound)

**Big Omega (Ω)** describes the **best-case scenario** - the minimum time an algorithm will take.

### Formal Definition

f(n) = Ω(g(n)) if there exist constants c > 0 and n₀ such that:

```
0 ≤ c × g(n) ≤ f(n)  for all n ≥ n₀
```

**In simple terms**: f(n) grows at least as fast as g(n)

### Example: Linear Search

```cpp
int linearSearch(int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;  // Found!
        }
    }
    return -1;  // Not found
}

/*
 * Best case (Ω):  Target is first element → Ω(1)
 * Worst case (O): Target is last or not present → O(n)
 * 
 * The algorithm is Ω(1) because it might find the element immediately
 */
```

### Example: Finding Minimum

```cpp
int findMin(int arr[], int n) {
    int min = arr[0];
    
    for (int i = 1; i < n; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    
    return min;
}

/*
 * Best case (Ω):  Must check all elements → Ω(n)
 * Worst case (O): Must check all elements → O(n)
 * 
 * No matter what, we have to look at every element
 * So Ω(n) and O(n) are the same → Θ(n)
 */
```

---

## 6) Big Theta Notation (Tight Bound)

**Big Theta (Θ)** describes the **average case** when upper and lower bounds match.

### Formal Definition

f(n) = Θ(g(n)) if and only if:
- f(n) = O(g(n))  AND
- f(n) = Ω(g(n))

**In simple terms**: f(n) grows exactly at the rate of g(n)

### Example 1: Array Sum

```cpp
int arraySum(int arr[], int n) {
    int sum = 0;
    
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    
    return sum;
}

/*
 * Best case:  Must visit all n elements → Ω(n)
 * Worst case: Must visit all n elements → O(n)
 * Average:    Must visit all n elements → Θ(n)
 * 
 * Since Ω(n) = O(n), we can say Θ(n)
 */
```

### Example 2: Bubble Sort

```cpp
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {           // n iterations
        for (int j = 0; j < n - i - 1; j++) {   // decreasing iterations
            if (arr[j] > arr[j + 1]) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

/*
 * Best case:  Array already sorted → Ω(n) with optimization
 * Worst case: Array reverse sorted → O(n²)
 * Average:    Random array → Θ(n²)
 * 
 * Without optimization, even best case is Θ(n²)
 */
```

### Example 3: Merge Sort

```cpp
// (Using merge and mergeSort from earlier)

/*
 * Merge Sort always divides array in half:
 * 
 * Best case:  Ω(n log n)
 * Worst case: O(n log n)
 * Average:    Θ(n log n)
 * 
 * All cases are the same → Θ(n log n)
 */
```

---

## 7) Comparing Notations

### When to Use Each Notation

| Notation | Usage | Example |
|----------|-------|---------|
| **O** | Upper bound (guarantee) | "This algorithm will take **at most** O(n²)" |
| **Ω** | Lower bound (minimum) | "This algorithm will take **at least** Ω(n)" |
| **Θ** | Tight bound (exact) | "This algorithm **always** takes Θ(n log n)" |

### Real-World Analogy

Imagine traveling from City A to City B:

- **Big O**: "It will take **at most** 3 hours" (worst traffic)
- **Big Omega**: "It will take **at least** 1 hour" (perfect conditions)
- **Big Theta**: "It typically takes **exactly** 2 hours" (normal conditions)

### Visual Comparison

```cpp
// Algorithm 1: Linear Search
int linearSearch(int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) return i;
    }
    return -1;
}
// Best case: Ω(1) - found immediately
// Worst case: O(n) - found at end or not found
// NOT Θ because best ≠ worst

// Algorithm 2: Print All Elements
void printAll(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
}
// Best case: Ω(n)
// Worst case: O(n)
// Average case: Θ(n) - always visits all elements
```

---

## 8) Common Time Complexities

### Ranking from Fastest to Slowest

```
Fast  ↑
      │
O(1)        │ Constant       │ Array access
O(log n)    │ Logarithmic    │ Binary search
O(n)        │ Linear         │ Loop through array
O(n log n)  │ Linearithmic   │ Merge sort, Quick sort
O(n²)       │ Quadratic      │ Nested loops, Bubble sort
O(n³)       │ Cubic          │ Triple nested loops
O(2ⁿ)       │ Exponential    │ Recursive fibonacci
O(n!)       │ Factorial      │ Generate all permutations
      │
Slow  ↓
```

### Practical Examples

```cpp
// O(1) - Constant
int getElement(int arr[], int index) {
    return arr[index];
}

// O(log n) - Logarithmic
// (Binary search shown earlier)

// O(n) - Linear
int countOccurrences(int arr[], int n, int value) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] == value) count++;
    }
    return count;
}

// O(n log n) - Linearithmic
// (Merge sort shown earlier)

// O(n²) - Quadratic
bool hasDuplicates(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) return true;
        }
    }
    return false;
}

// O(2ⁿ) - Exponential
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// O(n!) - Factorial
void permutations(string str, int l, int r) {
    if (l == r) {
        cout << str << endl;
    } else {
        for (int i = l; i <= r; i++) {
            swap(str[l], str[i]);
            permutations(str, l + 1, r);
            swap(str[l], str[i]);
        }
    }
}
```

---

## 9) Space Complexity

Space complexity measures how much **memory** an algorithm uses.

### Components of Space Complexity

```
Total Space = Fixed Space + Variable Space

Fixed Space:
  - Code (instructions)
  - Constants
  - Simple variables

Variable Space:
  - Dynamic allocations
  - Recursion call stack
  - Data structures
```

### Example 1: O(1) Space - Constant

```cpp
// O(1) space - only uses fixed variables
int sumArray(int arr[], int n) {
    int sum = 0;      // 4 bytes
    int i;            // 4 bytes
    
    for (i = 0; i < n; i++) {
        sum += arr[i];
    }
    
    return sum;
}
// Space used doesn't grow with input size → O(1)
```

### Example 2: O(n) Space - Linear

```cpp
// O(n) space - creates array of size n
int* doubleArray(int arr[], int n) {
    int* result = new int[n];  // n × 4 bytes
    
    for (int i = 0; i < n; i++) {
        result[i] = arr[i] * 2;
    }
    
    return result;
}
// Space used grows linearly with n → O(n)
```

### Example 3: O(n²) Space

```cpp
// O(n²) space - creates 2D array
int** createMatrix(int n) {
    int** matrix = new int*[n];
    
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n];  // n rows × n columns
    }
    
    return matrix;
}
// Space used: n × n = n² → O(n²)
```

---

## 10) Recursive vs Iterative Analysis

### Time Complexity Comparison

```cpp
// ITERATIVE - O(n) time, O(1) space
int factorialIterative(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// RECURSIVE - O(n) time, O(n) space
int factorialRecursive(int n) {
    if (n <= 1) return 1;
    return n * factorialRecursive(n - 1);
}
```

### Why Does Recursive Use More Space?

**Call Stack Visualization**:

```
factorialRecursive(4)
│
├── factorialRecursive(3)
│   │
│   ├── factorialRecursive(2)
│   │   │
│   │   ├── factorialRecursive(1)
│   │   │   └── returns 1
│   │   │
│   │   └── returns 2 × 1 = 2
│   │
│   └── returns 3 × 2 = 6
│
└── returns 4 × 6 = 24

Each call takes space on the stack: 4 calls → O(n) space
```

### Example 1: Fibonacci

```cpp
// BAD: Exponential time and linear space
int fibRecursive(int n) {
    if (n <= 1) return n;
    return fibRecursive(n - 1) + fibRecursive(n - 2);
}
// Time: O(2ⁿ) - exponential growth
// Space: O(n) - maximum depth of recursion tree

// GOOD: Linear time, constant space
int fibIterative(int n) {
    if (n <= 1) return n;
    
    int prev = 0, curr = 1;
    
    for (int i = 2; i <= n; i++) {
        int next = prev + curr;
        prev = curr;
        curr = next;
    }
    
    return curr;
}
// Time: O(n) - single loop
// Space: O(1) - only 3 variables
```

### Example 2: Array Sum

```cpp
// Recursive - O(n) time, O(n) space
int sumRecursive(int arr[], int n) {
    if (n == 0) return 0;
    return arr[n - 1] + sumRecursive(arr, n - 1);
}
// Each call adds to stack: n calls → O(n) space

// Iterative - O(n) time, O(1) space
int sumIterative(int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}
// Only uses fixed variables → O(1) space
```

### Example 3: Binary Search

```cpp
// Recursive - O(log n) time, O(log n) space
int binarySearchRecursive(int arr[], int left, int right, int target) {
    if (left > right) return -1;
    
    int mid = left + (right - left) / 2;
    
    if (arr[mid] == target) return mid;
    
    if (arr[mid] > target) {
        return binarySearchRecursive(arr, left, mid - 1, target);
    } else {
        return binarySearchRecursive(arr, mid + 1, right, target);
    }
}
// Divides in half: log n calls → O(log n) space

// Iterative - O(log n) time, O(1) space
int binarySearchIterative(int arr[], int n, int target) {
    int left = 0, right = n - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) return mid;
        
        if (arr[mid] > target) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    return -1;
}
// No recursion: O(1) space
```

### Recursion Call Stack Formula

```
Space Complexity = O(maximum depth of recursion)

Linear recursion:     O(n)       - factorial(n)
Logarithmic:          O(log n)   - binary search
Tree recursion:       O(n)       - fibonacci (height of tree)
```

---

## 11) Best, Average, and Worst Case

### Understanding Different Cases

```cpp
int linearSearch(int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
}

/*
 * Best Case (Ω):    Target at index 0 → Ω(1)
 * Average Case (Θ): Target in middle → Θ(n/2) → Θ(n)
 * Worst Case (O):   Target at end or absent → O(n)
 */
```

### Analyzing Different Scenarios

```cpp
// Insertion Sort
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/*
 * Best Case (already sorted):
 *   Array: [1, 2, 3, 4, 5]
 *   Inner while loop never executes
 *   Time: Ω(n)
 * 
 * Worst Case (reverse sorted):
 *   Array: [5, 4, 3, 2, 1]
 *   Inner while loop executes i times for each i
 *   Time: O(n²)
 * 
 * Average Case (random):
 *   Inner loop executes about i/2 times
 *   Time: Θ(n²)
 */
```

---

## 12) Practical Examples

### Example 1: Finding Duplicates

```cpp
// Approach 1: Brute Force - O(n²) time, O(1) space
bool hasDuplicates_v1(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) {
                return true;
            }
        }
    }
    return false;
}

// Approach 2: Sorting - O(n log n) time, O(1) space
#include <algorithm>
bool hasDuplicates_v2(int arr[], int n) {
    sort(arr, arr + n);  // O(n log n)
    
    for (int i = 0; i < n - 1; i++) {  // O(n)
        if (arr[i] == arr[i + 1]) {
            return true;
        }
    }
    return false;
}
// Total: O(n log n) + O(n) = O(n log n)

// Approach 3: Hash Set - O(n) time, O(n) space
#include <unordered_set>
bool hasDuplicates_v3(int arr[], int n) {
    unordered_set<int> seen;
    
    for (int i = 0; i < n; i++) {
        if (seen.find(arr[i]) != seen.end()) {
            return true;  // Already seen
        }
        seen.insert(arr[i]);
    }
    return false;
}

/*
 * Comparison:
 * ┌──────────┬─────────────┬─────────────┐
 * │ Approach │ Time        │ Space       │
 * ├──────────┼─────────────┼─────────────┤
 * │ Brute    │ O(n²)       │ O(1)        │
 * │ Sort     │ O(n log n)  │ O(1)        │
 * │ Hash     │ O(n)        │ O(n)        │
 * └──────────┴─────────────┴─────────────┘
 * 
 * Trade-off: Time vs Space
 */
```

### Example 2: Two Sum Problem

Find two numbers that add up to a target.

```cpp
// Approach 1: Brute Force - O(n²)
#include <vector>
#include <iostream>
using namespace std;

vector<int> twoSum_v1(int arr[], int n, int target) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] + arr[j] == target) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

// Approach 2: Hash Map - O(n)
#include <unordered_map>
vector<int> twoSum_v2(int arr[], int n, int target) {
    unordered_map<int, int> map;  // value → index
    
    for (int i = 0; i < n; i++) {
        int complement = target - arr[i];
        
        if (map.find(complement) != map.end()) {
            return {map[complement], i};
        }
        
        map[arr[i]] = i;
    }
    
    return {-1, -1};
}

/*
 * Analysis:
 * v1: O(n²) time, O(1) space - Check all pairs
 * v2: O(n) time, O(n) space - Use hash map for O(1) lookup
 */
```

### Example 3: Matrix Multiplication

```cpp
// O(n³) time, O(1) extra space
void matrixMultiply(int A[][100], int B[][100], int C[][100], int n) {
    // Initialize result matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
        }
    }
    
    // Multiply
    for (int i = 0; i < n; i++) {           // n iterations
        for (int j = 0; j < n; j++) {       // n iterations
            for (int k = 0; k < n; k++) {   // n iterations
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
// Three nested loops: n × n × n = n³ → O(n³)
```

---

## 13) Common Pitfalls

### Pitfall 1: Confusing Big O with Exact Count

```cpp
// This is O(n), NOT O(2n)
void printTwice(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
}
// Total: 2n operations, but we drop constants → O(n)
```

### Pitfall 2: Ignoring Dominant Terms

```cpp
// This is O(n²), NOT O(n² + n)
void example(int arr[], int n) {
    // First loop: O(n)
    for (int i = 0; i < n; i++) {
        cout << arr[i];
    }
    
    // Nested loops: O(n²)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << arr[i] + arr[j];
        }
    }
}
// Total: O(n + n²) → Drop lower term → O(n²)
```

### Pitfall 3: Hidden Complexity in Library Functions

```cpp
#include <string>
#include <algorithm>

// This looks O(n) but is actually O(n²)
void reverseWords(string words[], int n) {
    for (int i = 0; i < n; i++) {
        reverse(words[i].begin(), words[i].end());  // O(m) where m = word length
    }
}
// If average word length is m: O(n × m)
// If m ≈ n: O(n²)
```

### Pitfall 4: Not Considering Best/Worst Cases

```cpp
// Quick Sort
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/*
 * Best/Average Case: O(n log n) - balanced partitions
 * Worst Case: O(n²) - already sorted with bad pivot
 * 
 * Don't just say "Quick Sort is O(n log n)"!
 */
```

---

## 14) Quick Reference

### Time Complexity Cheat Sheet

```
┌────────────────────────────────────────────────────────────┐
│ Operation          │ Array  │ Linked │ Hash    │ Binary    │
│                    │        │ List   │ Table   │ Search    │
│                    │        │        │         │ Tree      │
├────────────────────┼────────┼────────┼─────────┼───────────┤
│ Access             │ O(1)   │ O(n)   │ N/A     │ O(log n)  │
│ Search             │ O(n)   │ O(n)   │ O(1)    │ O(log n)  │
│ Insert             │ O(n)   │ O(1)*  │ O(1)    │ O(log n)  │
│ Delete             │ O(n)   │ O(1)*  │ O(1)    │ O(log n)  │
└────────────────────────────────────────────────────────────┘
* At known position
```

### Sorting Algorithm Complexities

```
┌────────────────┬──────────┬──────────┬──────────┬────────┐
│ Algorithm      │ Best     │ Average  │ Worst    │ Space  │
├────────────────┼──────────┼──────────┼──────────┼────────┤
│ Bubble Sort    │ O(n)     │ O(n²)    │ O(n²)    │ O(1)   │
│ Selection Sort │ O(n²)    │ O(n²)    │ O(n²)    │ O(1)   │
│ Insertion Sort │ O(n)     │ O(n²)    │ O(n²)    │ O(1)   │
│ Merge Sort     │ O(nlogn) │ O(nlogn) │ O(nlogn) │ O(n)   │
│ Quick Sort     │ O(nlogn) │ O(nlogn) │ O(n²)    │ O(logn)│
│ Heap Sort      │ O(nlogn) │ O(nlogn) │ O(nlogn) │ O(1)   │
└────────────────┴──────────┴──────────┴──────────┴────────┘
```

### Rules of Thumb

1. **Drop constants**: O(2n) → O(n)
2. **Drop lower terms**: O(n² + n) → O(n²)
3. **Different inputs, different variables**: O(a + b) not O(n)
4. **Nested loops usually multiply**: O(n × m)
5. **Sequential operations add**: O(a) + O(b) → O(a + b)

### How to Analyze an Algorithm

```
Step 1: Identify the basic operation(s)
Step 2: Count how many times it executes
Step 3: Express as a function of input size
Step 4: Find the dominant term
Step 5: Express using Big O notation
```

### Example Analysis Template

```cpp
void example(int arr[], int n) {
    // Step 1: Basic operation = print
    // Step 2: Loop executes n times
    // Step 3: T(n) = n
    // Step 4: Dominant term = n
    // Step 5: O(n)
    
    for (int i = 0; i < n; i++) {
        cout << arr[i] << endl;
    }
}
```

---

## Practice Problems

### Problem 1: Analyze This Code

```cpp
void mystery(int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            sum++;
        }
    }
    cout << sum << endl;
}
```

**Answer**: 
- Outer loop: n iterations
- Inner loop: 0 + 1 + 2 + ... + (n-1) = n(n-1)/2
- Total: O(n²)

### Problem 2: Space Complexity

```cpp
int sumRecursive(int n) {
    if (n == 0) return 0;
    return n + sumRecursive(n - 1);
}
```

**Answer**:
- Time: O(n) - n recursive calls
- Space: O(n) - call stack grows to depth n

### Problem 3: Compare Approaches

Which is better for finding the maximum in an unsorted array?

a) Sort then return last element  
b) Loop through once to find max

**Answer**: 
- (a) O(n log n) - sorting
- (b) O(n) - single pass
- **Better: (b)** - linear is faster than linearithmic

---

## Additional Resources

**Books**:
- *Introduction to Algorithms* by CLRS (Chapters 3-4)
- *Algorithms* by Robert Sedgewick (Chapters 1.4)

**Online**:
- Big-O Cheat Sheet: https://www.bigocheatsheet.com/
- VisuAlgo: https://visualgo.net/
- Algorithm Visualizer: https://algorithm-visualizer.org/

**Practice**:
- LeetCode: https://leetcode.com/
- HackerRank: https://www.hackerrank.com/domains/algorithms

---

## Summary

### Key Takeaways

**Big O (O)**: Upper bound - worst case performance  
**Big Omega (Ω)**: Lower bound - best case performance  
**Big Theta (Θ)**: Tight bound - average case (when best = worst)  
**Drop constants and lower terms**: O(2n² + 3n) → O(n²)  
**Recursive algorithms use stack space**: Usually O(depth)  
**Time vs Space trade-off**: Faster algorithms often use more memory  
**Practice analyzing code**: Count operations relative to input size  

**Remember**: Understanding complexity helps you write efficient code and choose the right algorithm for the problem!
