# Advanced Hashing: A Deep Dive into Open Addressing vs. Chaining

## Table of Contents
1. [Introduction: The Birth of Hashing](#1-introduction)
2. [The Problem: Why We Need Hashing](#2-the-problem)
3. [Building Hashing from First Principles](#3-building-hashing)
4. [Collision Resolution: The Core Challenge](#4-collision-resolution)
5. [Chaining: The Linked Approach](#5-chaining)
6. [Open Addressing: The Probing Approach](#6-open-addressing)
7. [Deep Comparison: Open Addressing vs. Chaining](#7-comparison)
8. [Advanced Topics and Optimizations](#8-advanced-topics)
9. [Performance Analysis](#9-performance-analysis)
10. [Choosing the Right Strategy](#10-choosing-strategy)

---

## 1. Introduction: The Birth of Hashing

Imagine you're a librarian in a massive library with millions of books. A patron asks for a specific book. How do you find it quickly? You could search linearly through every shelf (O(n) time), or use a sorted system with binary search (O(log n) time). But what if you could go directly to the exact shelf where the book is located in O(1) time?

This is the promise of hashing: **constant-time average access** to data.

### What is Hashing?

Hashing is one of the most elegant ideas in computer science. It transforms a potentially large or complex key into a small integer index that points directly to the data's location in an array. However, this elegance comes with a challenge: **collisions**. When two different keys map to the same index, we need a strategy to handle them.

This guide explores two fundamental collision resolution strategies:
- **Chaining**: Store multiple elements at each index using linked structures
- **Open Addressing**: Find alternative locations within the same array

Let's build this from the ground up.

---

## 2. The Problem: Why We Need Hashing

### The Fundamental Data Storage Problem

Consider implementing a dictionary that maps student IDs to student records:

```cpp
struct Student {
    int id;
    string name;
    double gpa;
    
    Student(int i, const string& n, double g) 
        : id(i), name(n), gpa(g) {}
};
```

**Approach 1: Unsorted Array**
```cpp
class NaiveDictionary {
private:
    vector<Student> students;
    
public:
    void insert(const Student& s) {
        students.push_back(s);  // O(1)
    }
    
    Student* find(int id) {
        for (auto& s : students) {  // O(n) - Linear search
            if (s.id == id) return &s;
        }
        return nullptr;
    }
    
    bool remove(int id) {
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->id == id) {
                students.erase(it);  // O(n)
                return true;
            }
        }
        return false;
    }
};
```

**Performance:**
- Insert: O(1)
- Search: O(n)
- Delete: O(n)

**Approach 2: Sorted Array with Binary Search**
```cpp
class SortedDictionary {
private:
    vector<Student> students;
    
    // Keep array sorted by ID
    void insertSorted(const Student& s) {
        auto pos = lower_bound(students.begin(), students.end(), s,
            [](const Student& a, const Student& b) { return a.id < b.id; });
        students.insert(pos, s);  // O(n) due to shifting
    }
    
public:
    void insert(const Student& s) {
        insertSorted(s);
    }
    
    Student* find(int id) {
        int left = 0, right = students.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (students[mid].id == id)
                return &students[mid];
            else if (students[mid].id < id)
                left = mid + 1;
            else
                right = mid - 1;
        }
        return nullptr;  // O(log n)
    }
};
```

**Performance:**
- Insert: O(n)
- Search: O(log n)
- Delete: O(n)

**The Question**: Can we do better? Can we achieve O(1) for all operations?

### The Insight: Direct Addressing

What if we used the student ID directly as an array index?

```cpp
class DirectAddressing {
private:
    static const int MAX_ID = 1000000;
    Student* table[MAX_ID];  // Massive waste of space!
    
public:
    DirectAddressing() {
        for (int i = 0; i < MAX_ID; i++)
            table[i] = nullptr;
    }
    
    void insert(const Student& s) {
        if (s.id >= 0 && s.id < MAX_ID)
            table[s.id] = new Student(s);  // O(1)
    }
    
    Student* find(int id) {
        if (id >= 0 && id < MAX_ID)
            return table[id];  // O(1)
        return nullptr;
    }
    
    bool remove(int id) {
        if (id >= 0 && id < MAX_ID && table[id] != nullptr) {
            delete table[id];
            table[id] = nullptr;
            return true;
        }
        return false;
    }
};
```

**Problems with Direct Addressing:**
1. **Space Waste**: If we only have 1,000 students but IDs go up to 1,000,000, we waste 99.9% of memory
2. **Limited Key Types**: Only works for integer keys in a known range
3. **Sparse Data**: Most applications have sparse key spaces
4. **Memory Cost**: For large ID ranges, memory requirements are prohibitive

### Enter Hashing

Hashing solves these problems by:
1. **Mapping large key spaces to smaller table sizes** using a hash function
2. **Supporting arbitrary key types** (strings, objects, etc.)
3. **Achieving O(1) average-case performance** with reasonable space usage

The trade-off: **Collisions are now inevitable**.

---

## 3. Building Hashing from First Principles

### The Hash Function

A hash function `h(key)` maps a key to an index in our table:

```
h: Universe of Keys → {0, 1, 2, ..., m-1}
```

where `m` is the table size.

**Properties of a Good Hash Function:**
1. **Deterministic**: Same key always produces same hash
2. **Uniform Distribution**: Keys spread evenly across table
3. **Fast Computation**: O(1) time to compute
4. **Minimize Collisions**: Different keys should rarely hash to same index
5. **Avalanche Effect**: Small change in key causes large change in hash

### Basic Hash Functions

**For Integer Keys:**

```cpp
class IntHashFunctions {
public:
    // Division Method: Simple but can have patterns
    static size_t division(int key, size_t tableSize) {
        return abs(key) % tableSize;
    }
    
    // Multiplication Method: Better distribution
    // Uses Knuth's multiplicative constant
    static size_t multiplication(int key, size_t tableSize) {
        const double A = 0.6180339887;  // (sqrt(5) - 1) / 2
        double temp = abs(key) * A;
        temp = temp - floor(temp);  // Extract fractional part
        return static_cast<size_t>(floor(tableSize * temp));
    }
    
    // Universal Hashing: Randomized, proven good distribution
    // From family of hash functions: h(k) = ((a*k + b) mod p) mod m
    static size_t universal(int key, size_t tableSize, int a, int b, int p) {
        // p is prime > max key value
        // a is random in [1, p-1]
        // b is random in [0, p-1]
        long long hash = (static_cast<long long>(a) * key + b) % p;
        return hash % tableSize;
    }
    
    // MurmurHash-inspired integer hash
    static size_t murmur(int key, size_t tableSize) {
        unsigned int h = static_cast<unsigned int>(key);
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;
        return h % tableSize;
    }
};
```

**For String Keys:**

```cpp
class StringHashFunctions {
public:
    // Polynomial Rolling Hash
    // Treats string as polynomial: s[0]*B^0 + s[1]*B^1 + ... + s[n-1]*B^(n-1)
    static size_t polynomialHash(const string& key, size_t tableSize) {
        const int BASE = 31;  // Prime number as base
        size_t hash = 0;
        size_t power = 1;
        
        for (char c : key) {
            hash = (hash + (c - 'a' + 1) * power) % tableSize;
            power = (power * BASE) % tableSize;
        }
        return hash;
    }
    
    // DJB2 Hash: Simple and effective
    // Created by Daniel J. Bernstein
    static size_t djb2(const string& key, size_t tableSize) {
        size_t hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c;  // hash * 33 + c
        }
        return hash % tableSize;
    }
    
    // SDBM Hash: Used in Berkeley DB
    static size_t sdbm(const string& key, size_t tableSize) {
        size_t hash = 0;
        for (char c : key) {
            hash = c + (hash << 6) + (hash << 16) - hash;
        }
        return hash % tableSize;
    }
    
    // FNV-1a: Fast, good distribution
    // Fowler-Noll-Vo hash function
    static size_t fnv1a(const string& key, size_t tableSize) {
        const size_t FNV_OFFSET = 2166136261u;
        const size_t FNV_PRIME = 16777619u;
        
        size_t hash = FNV_OFFSET;
        for (char c : key) {
            hash ^= static_cast<size_t>(c);
            hash *= FNV_PRIME;
        }
        return hash % tableSize;
    }
};
```

### Understanding Hash Distribution

Let's visualize how different hash functions distribute keys:

```cpp
void analyzeHashDistribution() {
    const int TABLE_SIZE = 100;
    const int NUM_KEYS = 1000;
    
    vector<int> divisionDist(TABLE_SIZE, 0);
    vector<int> multiplicationDist(TABLE_SIZE, 0);
    
    // Generate sequential keys
    for (int key = 1; key <= NUM_KEYS; key++) {
        divisionDist[IntHashFunctions::division(key, TABLE_SIZE)]++;
        multiplicationDist[IntHashFunctions::multiplication(key, TABLE_SIZE)]++;
    }
    
    // Calculate variance (lower is better)
    auto calculateVariance = [](const vector<int>& dist) {
        double mean = 0;
        for (int count : dist) mean += count;
        mean /= dist.size();
        
        double variance = 0;
        for (int count : dist) {
            variance += (count - mean) * (count - mean);
        }
        return variance / dist.size();
    };
    
    cout << "Division method variance: " << calculateVariance(divisionDist) << endl;
    cout << "Multiplication method variance: " << calculateVariance(multiplicationDist) << endl;
}
```

### The Collision Problem

Even with a perfect hash function, collisions are inevitable due to the **Pigeonhole Principle**:

> If you have n items and m slots, and n > m, then at least one slot must contain more than one item.

**Birthday Paradox Application**: With just √m items, the probability of a collision approaches 50%.

For a table of size 365 (days in a year), you only need 23 people for a 50% chance of two sharing a birthday!

```cpp
// Calculate collision probability
double collisionProbability(int tableSize, int numKeys) {
    if (numKeys > tableSize) return 1.0;
    
    double prob = 1.0;
    for (int i = 0; i < numKeys; i++) {
        prob *= (tableSize - i) / static_cast<double>(tableSize);
    }
    return 1.0 - prob;  // Probability of at least one collision
}

// Example: 
// collisionProbability(365, 23) ≈ 0.507 (50.7%)
// collisionProbability(100, 12) ≈ 0.555 (55.5%)
```

This means collisions happen much sooner than intuition suggests, making collision resolution critical.

---

## 4. Collision Resolution: The Core Challenge

When two keys hash to the same index, we need a strategy to handle them. The two fundamental approaches are:

### Collision Resolution Strategies

1. **Chaining (Separate Chaining/Closed Addressing)**
   - Each table slot contains a data structure (linked list, array, tree)
   - Multiple elements can coexist at the same index
   - Table size can be smaller than number of elements

2. **Open Addressing (Closed Hashing)**
   - All elements stored directly in the table
   - On collision, probe for next available slot
   - Table size must be ≥ number of elements

Let's explore each in depth with complete implementations.

---

## 5. Chaining: The Linked Approach

### The Core Idea

With chaining, each table slot contains a linked list (or other data structure) of all elements that hash to that index.

**Visual Representation:**
```
Table:
[0] → NULL
[1] → [Key=12, Val="Alice"] → [Key=45, Val="Bob"] → NULL
[2] → [Key=23, Val="Charlie"] → NULL
[3] → NULL
[4] → [Key=9, Val="David"] → [Key=34, Val="Eve"] → [Key=71, Val="Frank"] → NULL
[5] → [Key=5, Val="Grace"] → NULL
...
```

### Basic Implementation with Linked Lists

```cpp
template<typename K, typename V>
class HashTableChaining {
private:
    struct Node {
        K key;
        V value;
        Node* next;
        
        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };
    
    vector<Node*> table;
    size_t tableSize;
    size_t numElements;
    
    // Hash function
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % tableSize;
    }
    
public:
    HashTableChaining(size_t size = 101) 
        : tableSize(size), numElements(0) {
        table.resize(tableSize, nullptr);
    }
    
    // Insert or update
    void insert(const K& key, const V& value) {
        size_t index = hash(key);
        Node* current = table[index];
        
        // Search if key already exists (update case)
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;  // Update existing value
                return;
            }
            current = current->next;
        }
        
        // Key not found, insert new node at head of list
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        numElements++;
    }
    
    // Search
    V* find(const K& key) {
        size_t index = hash(key);
        Node* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;  // Not found
    }
    
    // Remove
    bool remove(const K& key) {
        size_t index = hash(key);
        Node* current = table[index];
        Node* prev = nullptr;
        
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    // Removing head of list
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                numElements--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;  // Key not found
    }
    
    // Get load factor (α = n/m)
    double loadFactor() const {
        return static_cast<double>(numElements) / tableSize;
    }
    
    size_t size() const {
        return numElements;
    }
    
    // Destructor - free all memory
    ~HashTableChaining() {
        for (Node* head : table) {
            while (head != nullptr) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }
    }
};
```

### Advanced Chaining with Dynamic Resizing

A key optimization is to resize the table when the load factor becomes too high:

```cpp
template<typename K, typename V>
class AdvancedHashChaining {
private:
    struct Node {
        K key;
        V value;
        Node* next;
        
        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };
    
    vector<Node*> table;
    size_t tableSize;
    size_t numElements;
    
    // Threshold for resizing
    static constexpr double MAX_LOAD_FACTOR = 1.0;
    static constexpr double MIN_LOAD_FACTOR = 0.25;
    
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % tableSize;
    }
    
    // Find next prime number >= n (for better distribution)
    size_t nextPrime(size_t n) const {
        if (n <= 2) return 2;
        if (n % 2 == 0) n++;
        
        while (true) {
            bool isPrime = true;
            for (size_t i = 3; i * i <= n; i += 2) {
                if (n % i == 0) {
                    isPrime = false;
                    break;
                }
            }
            if (isPrime) return n;
            n += 2;
        }
    }
    
    // Resize and rehash all elements
    void resize(size_t newSize) {
        size_t oldSize = tableSize;
        tableSize = newSize;
        
        // Save old table
        vector<Node*> oldTable = move(table);
        table.resize(tableSize, nullptr);
        numElements = 0;
        
        // Rehash all elements into new table
        for (Node* head : oldTable) {
            while (head != nullptr) {
                insert(head->key, head->value);
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }
    }
    
public:
    AdvancedHashChaining(size_t size = 101) 
        : tableSize(nextPrime(size)), numElements(0) {
        table.resize(tableSize, nullptr);
    }
    
    void insert(const K& key, const V& value) {
        // Check if we need to resize before inserting
        if (loadFactor() > MAX_LOAD_FACTOR) {
            resize(nextPrime(tableSize * 2));
        }
        
        size_t index = hash(key);
        Node* current = table[index];
        
        // Check if key exists
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        
        // Insert at head
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        numElements++;
    }
    
    V* find(const K& key) {
        size_t index = hash(key);
        Node* current = table[index];
        
        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }
    
    bool remove(const K& key) {
        size_t index = hash(key);
        Node* current = table[index];
        Node* prev = nullptr;
        
        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                numElements--;
                
                // Shrink table if load factor too low
                if (tableSize > 101 && loadFactor() < MIN_LOAD_FACTOR) {
                    resize(nextPrime(tableSize / 2));
                }
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }
    
    double loadFactor() const {
        return static_cast<double>(numElements) / tableSize;
    }
    
    // Statistics for analysis
    struct Statistics {
        int maxChainLength;
        int emptySlots;
        double avgChainLength;
        double loadFactor;
        vector<int> chainLengthDistribution;
    };
    
    Statistics getStatistics() const {
        Statistics stats;
        stats.maxChainLength = 0;
        stats.emptySlots = 0;
        stats.loadFactor = loadFactor();
        vector<int> chainLengths;
        
        for (Node* head : table) {
            int length = 0;
            Node* current = head;
            while (current != nullptr) {
                length++;
                current = current->next;
            }
            
            if (length == 0) {
                stats.emptySlots++;
            } else {
                chainLengths.push_back(length);
            }
            stats.maxChainLength = max(stats.maxChainLength, length);
        }
        
        stats.avgChainLength = chainLengths.empty() ? 0.0 :
            accumulate(chainLengths.begin(), chainLengths.end(), 0.0) / chainLengths.size();
        
        stats.chainLengthDistribution = chainLengths;
        
        return stats;
    }
    
    void printStatistics() const {
        auto stats = getStatistics();
        
        cout << "\n=== Hash Table Statistics (Chaining) ===" << endl;
        cout << "Table Size: " << tableSize << endl;
        cout << "Number of Elements: " << numElements << endl;
        cout << "Load Factor: " << fixed << setprecision(3) << stats.loadFactor << endl;
        cout << "Empty Slots: " << stats.emptySlots << " (" 
             << (100.0 * stats.emptySlots / tableSize) << "%)" << endl;
        cout << "Max Chain Length: " << stats.maxChainLength << endl;
        cout << "Avg Chain Length (non-empty): " << fixed << setprecision(2) 
             << stats.avgChainLength << endl;
    }
    
    ~AdvancedHashChaining() {
        for (Node* head : table) {
            while (head != nullptr) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }
    }
};
```

### Chaining Variants

**1. Chaining with Dynamic Arrays (Better Cache Performance)**

Instead of linked lists, use vectors for better spatial locality:

```cpp
template<typename K, typename V>
class VectorChaining {
private:
    struct Entry {
        K key;
        V value;
        Entry(const K& k, const V& v) : key(k), value(v) {}
    };
    
    vector<vector<Entry>> table;
    size_t tableSize;
    size_t numElements;
    
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % tableSize;
    }
    
public:
    VectorChaining(size_t size = 101) : tableSize(size), numElements(0) {
        table.resize(tableSize);
    }
    
    void insert(const K& key, const V& value) {
        size_t index = hash(key);
        
        // Check if key exists
        for (auto& entry : table[index]) {
            if (entry.key == key) {
                entry.value = value;
                return;
            }
        }
        
        // Insert new entry
        table[index].emplace_back(key, value);
        numElements++;
    }
    
    V* find(const K& key) {
        size_t index = hash(key);
        
        for (auto& entry : table[index]) {
            if (entry.key == key) {
                return &entry.value;
            }
        }
        return nullptr;
    }
    
    bool remove(const K& key) {
        size_t index = hash(key);
        auto& bucket = table[index];
        
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                numElements--;
                return true;
            }
        }
        return false;
    }
};
```

**Advantages of Vector Chaining:**
- Better cache locality (sequential memory)
- Fewer memory allocations
- No pointer overhead

**Disadvantages:**
- Slower insertions if vector needs reallocation
- Slower deletions (requires shifting)

**2. Chaining with Self-Balancing Trees (For Worst-Case Guarantees)**

Java's HashMap switches to trees when chains get long:

```cpp
template<typename K, typename V>
class TreeChaining {
private:
    struct Entry {
        K key;
        V value;
        Entry(const K& k, const V& v) : key(k), value(v) {}
        
        bool operator<(const Entry& other) const {
            return key < other.key;
        }
    };
    
    vector<set<Entry>> table;  // Using std::set (Red-Black Tree)
    size_t tableSize;
    size_t numElements;
    
public:
    void insert(const K& key, const V& value) {
        size_t index = hash(key);
        // Set automatically maintains sorted order
        // This gives O(log n) worst-case for long chains
        table[index].insert(Entry(key, value));
        numElements++;
    }
    
    // Find: O(log chain_length) worst-case
    // vs O(chain_length) for linked list
};
```

### Performance Analysis of Chaining

**Time Complexity:**

| Operation | Average Case | Worst Case |
|-----------|--------------|------------|
| Insert    | O(1)         | O(n)       |
| Search    | O(1 + α)     | O(n)       |
| Delete    | O(1 + α)     | O(n)       |

where α (alpha) is the load factor (n/m)

**Expected Chain Length:**

With uniform hashing, the expected length of a chain is α = n/m.

For α = 1 (n = m), average chain length is 1.
For α = 2 (n = 2m), average chain length is 2.

**Space Complexity:** O(n + m)
- n: number of elements
- m: table size
- Plus pointer overhead for links

---

## 6. Open Addressing: The Probing Approach

### The Core Idea

In open addressing, all elements are stored in the hash table itself. When a collision occurs, we **probe** for the next available slot using a systematic sequence.

**Key Difference from Chaining:**
- No auxiliary data structures
- Table can become full (load factor α ≤ 1)
- Must handle deletions carefully

### Probing Sequence

For a key k, we try positions:
```
h(k, 0), h(k, 1), h(k, 2), ..., h(k, m-1)
```

where h(k, i) determines the i-th position to try.

### Three Main Probing Strategies

#### 1. Linear Probing

**Formula:** `h(k, i) = (h(k) + i) mod m`

Simply try the next slot sequentially.

```cpp
template<typename K, typename V>
class LinearProbingHashTable {
private:
    enum EntryStatus { EMPTY, OCCUPIED, DELETED };
    
    struct Entry {
        K key;
        V value;
        EntryStatus status;
        
        Entry() : status(EMPTY) {}
        Entry(const K& k, const V& v) : key(k), value(v), status(OCCUPIED) {}
    };
    
    vector<Entry> table;
    size_t tableSize;
    size_t numElements;
    size_t numDeleted;
    
    static constexpr double MAX_LOAD_FACTOR = 0.5;
    
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % tableSize;
    }
    
    size_t nextPrime(size_t n) const {
        if (n <= 2) return 2;
        if (n % 2 == 0) n++;
        
        while (true) {
            bool isPrime = true;
            for (size_t i = 3; i * i <= n; i += 2) {
                if (n % i == 0) {
                    isPrime = false;
                    break;
                }
            }
            if (isPrime) return n;
            n += 2;
        }
    }
    
    void resize() {
        size_t oldSize = tableSize;
        tableSize = nextPrime(tableSize * 2);
        
        vector<Entry> oldTable = move(table);
        table.resize(tableSize);
        numElements = 0;
        numDeleted = 0;
        
        // Rehash all occupied entries
        for (const auto& entry : oldTable) {
            if (entry.status == OCCUPIED) {
                insert(entry.key, entry.value);
            }
        }
    }
    
public:
    LinearProbingHashTable(size_t size = 101) 
        : tableSize(nextPrime(size)), numElements(0), numDeleted(0) {
        table.resize(tableSize);
    }
    
    void insert(const K& key, const V& value) {
        // Resize if load factor too high
        if (static_cast<double>(numElements) / tableSize > MAX_LOAD_FACTOR) {
            resize();
        }
        
        size_t index = hash(key);
        size_t i = 0;
        
        // Linear probing: try index, index+1, index+2, ...
        while (i < tableSize) {
            size_t pos = (index + i) % tableSize;
            
            if (table[pos].status == EMPTY || table[pos].status == DELETED) {
                // Found empty or deleted slot
                table[pos] = Entry(key, value);
                if (table[pos].status == DELETED) {
                    numDeleted--;
                }
                numElements++;
                return;
            } else if (table[pos].key == key) {
                // Key already exists, update value
                table[pos].value = value;
                return;
            }
            
            i++;  // Try next position
        }
        
        // Table is full (shouldn't happen with proper load factor management)
        throw runtime_error("Hash table is full");
    }
    
    V* find(const K& key) {
        size_t index = hash(key);
        size_t i = 0;
        
        while (i < tableSize) {
            size_t pos = (index + i) % tableSize;
            
            if (table[pos].status == EMPTY) {
                // Key not found (hit empty slot)
                return nullptr;
            } else if (table[pos].status == OCCUPIED && table[pos].key == key) {
                // Found the key
                return &table[pos].value;
            }
            
            i++;  // Continue probing
        }
        
        return nullptr;  // Not found after checking entire table
    }
    
    bool remove(const K& key) {
        size_t index = hash(key);
        size_t i = 0;
        
        while (i < tableSize) {
            size_t pos = (index + i) % tableSize;
            
            if (table[pos].status == EMPTY) {
                return false;  // Key not found
            } else if (table[pos].status == OCCUPIED && table[pos].key == key) {
                // Mark as deleted (lazy deletion)
                table[pos].status = DELETED;
                numElements--;
                numDeleted++;
                
                // Rehash if too many deleted entries
                if (numDeleted > tableSize / 4) {
                    resize();
                }
                return true;
            }
            
            i++;
        }
        
        return false;
    }
    
    double loadFactor() const {
        return static_cast<double>(numElements) / tableSize;
    }
    
    void printStatistics() const {
        int longestProbe = 0;
        int totalProbes = 0;
        int clusters = 0;
        int currentClusterSize = 0;
        
        for (size_t i = 0; i < tableSize; i++) {
            if (table[i].status == OCCUPIED) {
                // Calculate probe length for this element
                size_t originalPos = hash(table[i].key);
                int probeLength = 0;
                size_t pos = originalPos;
                
                while (pos != i) {
                    probeLength++;
                    pos = (pos + 1) % tableSize;
                }
                
                longestProbe = max(longestProbe, probeLength);
                totalProbes += probeLength;
                currentClusterSize++;
            } else {
                if (currentClusterSize > 0) {
                    clusters++;
                    currentClusterSize = 0;
                }
            }
        }
        
        cout << "\n=== Hash Table Statistics (Linear Probing) ===" << endl;
        cout << "Table Size: " << tableSize << endl;
        cout << "Elements: " << numElements << endl;
        cout << "Deleted Markers: " << numDeleted << endl;
        cout << "Load Factor: " << fixed << setprecision(3) << loadFactor() << endl;
        cout << "Longest Probe: " << longestProbe << endl;
        cout << "Avg Probe Length: " << fixed << setprecision(2) 
             << (numElements > 0 ? static_cast<double>(totalProbes) / numElements : 0) << endl;
        cout << "Number of Clusters: " << clusters << endl;
    }
};
```

**Primary Clustering Problem:**

Linear probing suffers from **primary clustering**: long runs of occupied slots form, and they tend to get longer.

```
Example:
Hash values: h(A)=3, h(B)=3, h(C)=5, h(D)=3

Table after insertions:
[0] EMPTY
[1] EMPTY
[2] EMPTY
[3] A (h(A)=3, no collision)
[4] B (h(B)=3, collided, probed to 4)
[5] C (h(C)=5, no collision)
[6] D (h(D)=3, collided at 3,4,5, probed to 6)
[7] EMPTY

Cluster from index 3-6 has formed!
Next insertion with h(E)=4 will extend cluster to index 7.
```

#### 2. Quadratic Probing

**Formula:** `h(k, i) = (h(k) + c₁·i + c₂·i²) mod m`

Common choice: `c₁ = 1, c₂ = 1`, giving `h(k, i) = (h(k) + i + i²) mod m`

```cpp
template<typename K, typename V>
class QuadraticProbingHashTable {
private:
    enum EntryStatus { EMPTY, OCCUPIED, DELETED };
    
    struct Entry {
        K key;
        V value;
        EntryStatus status;
        
        Entry() : status(EMPTY) {}
        Entry(const K& k, const V& v) : key(k), value(v), status(OCCUPIED) {}
    };
    
    vector<Entry> table;
    size_t tableSize;
    size_t numElements;
    
    static constexpr double MAX_LOAD_FACTOR = 0.5;
    
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % tableSize;
    }
    
    size_t nextPrime(size_t n) const {
        if (n <= 2) return 2;
        if (n % 2 == 0) n++;
        
        while (true) {
            bool isPrime = true;
            for (size_t i = 3; i * i <= n; i += 2) {
                if (n % i == 0) {
                    isPrime = false;
                    break;
                }
            }
            if (isPrime) return n;
            n += 2;
        }
    }
    
    void resize() {
        size_t oldSize = tableSize;
        tableSize = nextPrime(tableSize * 2);
        
        vector<Entry> oldTable = move(table);
        table.resize(tableSize);
        numElements = 0;
        
        for (const auto& entry : oldTable) {
            if (entry.status == OCCUPIED) {
                insert(entry.key, entry.value);
            }
        }
    }
    
public:
    QuadraticProbingHashTable(size_t size = 101) 
        : tableSize(nextPrime(size)), numElements(0) {
        table.resize(tableSize);
    }
    
    void insert(const K& key, const V& value) {
        if (static_cast<double>(numElements) / tableSize > MAX_LOAD_FACTOR) {
            resize();
        }
        
        size_t index = hash(key);
        size_t i = 0;
        
        // Quadratic probing: try h(k), h(k)+1, h(k)+4, h(k)+9, h(k)+16, ...
        while (i < tableSize) {
            size_t pos = (index + i * i) % tableSize;
            
            if (table[pos].status == EMPTY || table[pos].status == DELETED) {
                table[pos] = Entry(key, value);
                numElements++;
                return;
            } else if (table[pos].key == key) {
                table[pos].value = value;
                return;
            }
            
            i++;
        }
        
        throw runtime_error("Hash table is full");
    }
    
    V* find(const K& key) {
        size_t index = hash(key);
        size_t i = 0;
        
        while (i < tableSize) {
            size_t pos = (index + i * i) % tableSize;
            
            if (table[pos].status == EMPTY) {
                return nullptr;
            } else if (table[pos].status == OCCUPIED && table[pos].key == key) {
                return &table[pos].value;
            }
            
            i++;
        }
        
        return nullptr;
    }
    
    bool remove(const K& key) {
        size_t index = hash(key);
        size_t i = 0;
        
        while (i < tableSize) {
            size_t pos = (index + i * i) % tableSize;
            
            if (table[pos].status == EMPTY) {
                return false;
            } else if (table[pos].status == OCCUPIED && table[pos].key == key) {
                table[pos].status = DELETED;
                numElements--;
                return true;
            }
            
            i++;
        }
        
        return false;
    }
};
```

**Secondary Clustering:**

Quadratic probing eliminates primary clustering but suffers from **secondary clustering**: keys with the same hash value follow the same probe sequence.

**Important Theorem:** If table size m is prime and load factor α < 0.5, quadratic probing with `c₁=1, c₂=1` will always find an empty slot.

#### 3. Double Hashing

**Formula:** `h(k, i) = (h₁(k) + i·h₂(k)) mod m`

Uses two hash functions to create more varied probe sequences.

```cpp
template<typename K, typename V>
class DoubleHashingTable {
private:
    enum EntryStatus { EMPTY, OCCUPIED, DELETED };
    
    struct Entry {
        K key;
        V value;
        EntryStatus status;
        
        Entry() : status(EMPTY) {}
        Entry(const K& k, const V& v) : key(k), value(v), status(OCCUPIED) {}
    };
    
    vector<Entry> table;
    size_t tableSize;
    size_t numElements;
    
    static constexpr double MAX_LOAD_FACTOR = 0.7;  // Can use higher load factor
    
    // Primary hash function
    size_t hash1(const K& key) const {
        return std::hash<K>{}(key) % tableSize;
    }
    
    // Secondary hash function
    // Must never return 0, and should be relatively prime to table size
    size_t hash2(const K& key) const {
        size_t h = std::hash<K>{}(key);
        // Common formula: R - (h % R) where R is prime < m
        size_t R = tableSize - 1;
        while (!isPrime(R)) R--;
        return R - (h % R);
    }
    
    bool isPrime(size_t n) const {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        
        for (size_t i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0)
                return false;
        }
        return true;
    }
    
    size_t nextPrime(size_t n) const {
        if (n <= 2) return 2;
        if (n % 2 == 0) n++;
        
        while (!isPrime(n)) n += 2;
        return n;
    }
    
    void resize() {
        size_t oldSize = tableSize;
        tableSize = nextPrime(tableSize * 2);
        
        vector<Entry> oldTable = move(table);
        table.resize(tableSize);
        numElements = 0;
        
        for (const auto& entry : oldTable) {
            if (entry.status == OCCUPIED) {
                insert(entry.key, entry.value);
            }
        }
    }
    
public:
    DoubleHashingTable(size_t size = 101) 
        : tableSize(nextPrime(size)), numElements(0) {
        table.resize(tableSize);
    }
    
    void insert(const K& key, const V& value) {
        if (static_cast<double>(numElements) / tableSize > MAX_LOAD_FACTOR) {
            resize();
        }
        
        size_t h1 = hash1(key);
        size_t h2 = hash2(key);
        size_t i = 0;
        
        // Double hashing: h(k,i) = (h1(k) + i*h2(k)) mod m
        while (i < tableSize) {
            size_t pos = (h1 + i * h2) % tableSize;
            
            if (table[pos].status == EMPTY || table[pos].status == DELETED) {
                table[pos] = Entry(key, value);
                numElements++;
                return;
            } else if (table[pos].key == key) {
                table[pos].value = value;
                return;
            }
            
            i++;
        }
        
        throw runtime_error("Hash table is full");
    }
    
    V* find(const K& key) {
        size_t h1 = hash1(key);
        size_t h2 = hash2(key);
        size_t i = 0;
        
        while (i < tableSize) {
            size_t pos = (h1 + i * h2) % tableSize;
            
            if (table[pos].status == EMPTY) {
                return nullptr;
            } else if (table[pos].status == OCCUPIED && table[pos].key == key) {
                return &table[pos].value;
            }
            
            i++;
        }
        
        return nullptr;
    }
    
    bool remove(const K& key) {
        size_t h1 = hash1(key);
        size_t h2 = hash2(key);
        size_t i = 0;
        
        while (i < tableSize) {
            size_t pos = (h1 + i * h2) % tableSize;
            
            if (table[pos].status == EMPTY) {
                return false;
            } else if (table[pos].status == OCCUPIED && table[pos].key == key) {
                table[pos].status = DELETED;
                numElements--;
                return true;
            }
            
            i++;
        }
        
        return false;
    }
    
    double loadFactor() const {
        return static_cast<double>(numElements) / tableSize;
    }
    
    void printStatistics() const {
        cout << "\n=== Hash Table Statistics (Double Hashing) ===" << endl;
        cout << "Table Size: " << tableSize << endl;
        cout << "Elements: " << numElements << endl;
        cout << "Load Factor: " << fixed << setprecision(3) << loadFactor() << endl;
    }
};
```

**Advantages of Double Hashing:**
- Best open addressing method
- No primary clustering
- Minimal secondary clustering
- Probe sequences depend on both h₁(k) and h₂(k)

### The Deletion Problem in Open Addressing

**Problem:** Cannot simply mark slot as EMPTY, or search stops prematurely.

**Example:**
```
Insert A (hashes to 0)
Insert B (hashes to 0, probes to 1)
Delete A (if we mark slot 0 as EMPTY)
Search B (starts at 0, sees EMPTY, returns NOT FOUND) ❌
```

**Solutions:**

1. **Lazy Deletion (Used Above):**
   - Mark as DELETED
   - Treated as EMPTY for insertion
   - Treated as OCCUPIED for search
   - Problem: Table fills with tombstones

2. **Rehashing on Delete:**
   - After deletion, rehash elements in the cluster
   - Expensive but no tombstones

3. **Robin Hood Hashing:**
   - Sophisticated variant that makes deletion easier

---

## 7. Deep Comparison: Open Addressing vs. Chaining

### Performance Comparison

| Aspect | Chaining | Open Addressing |
|--------|----------|-----------------|
| **Space** | Extra pointers | No extra pointers |
| **Cache Performance** | Poor (pointer chasing) | Better (array locality) |
| **Load Factor** | Can exceed 1 | Must be < 1 |
| **Deletion** | Simple | Complex (tombstones) |
| **Worst Case Search** | O(n) | O(n) |
| **Average Search (α=0.5)** | ~1.25 probes | ~1.5 probes |
| **Average Search (α=0.9)** | ~1.45 probes | ~5.5 probes |
| **Sensitive to Hash Quality** | Less sensitive | More sensitive |
| **Memory Overhead** | Pointers (~8 bytes each) | Status bits (~2 bits) |

### Detailed Mathematical Analysis

**Expected number of probes for SUCCESSFUL search:**

**Chaining:**
```
E[probes] = 1 + α/2
```
For α = 1: ~1.5 probes

**Linear Probing:**
```
E[probes] = (1/2)(1 + 1/(1-α))
```
For α = 0.5: ~1.5 probes
For α = 0.9: ~5.5 probes

**Double Hashing:**
```
E[probes] = (1/α) ln(1/(1-α))
```
For α = 0.5: ~1.39 probes
For α = 0.9: ~2.56 probes

**Expected number of probes for UNSUCCESSFUL search:**

**Chaining:**
```
E[probes] = α
```

**Linear Probing:**
```
E[probes] = (1/2)(1 + 1/(1-α)²)
```

**Double Hashing:**
```
E[probes] = 1/(1-α)
```

### Empirical Comparison

```cpp
void compareHashTables() {
    const int NUM_OPERATIONS = 100000;
    const int TABLE_SIZE = 10007;  // Prime number
    
    // Test 1: Chaining
    auto start = chrono::high_resolution_clock::now();
    AdvancedHashChaining<int, string> chainTable(TABLE_SIZE);
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        chainTable.insert(i, "Value" + to_string(i));
    }
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        chainTable.find(i);
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto chainTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    // Test 2: Linear Probing
    start = chrono::high_resolution_clock::now();
    LinearProbingHashTable<int, string> linearTable(TABLE_SIZE);
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        linearTable.insert(i, "Value" + to_string(i));
    }
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        linearTable.find(i);
    }
    
    end = chrono::high_resolution_clock::now();
    auto linearTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    // Test 3: Double Hashing
    start = chrono::high_resolution_clock::now();
    DoubleHashingTable<int, string> doubleTable(TABLE_SIZE);
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        doubleTable.insert(i, "Value" + to_string(i));
    }
    
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        doubleTable.find(i);
    }
    
    end = chrono::high_resolution_clock::now();
    auto doubleTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    
    cout << "\n=== Performance Comparison ===" << endl;
    cout << "Operations: " << NUM_OPERATIONS << " inserts + " << NUM_OPERATIONS << " searches" << endl;
    cout << "Chaining: " << chainTime << " ms" << endl;
    cout << "Linear Probing: " << linearTime << " ms" << endl;
    cout << "Double Hashing: " << doubleTime << " ms" << endl;
}
```

### When to Use Each Strategy

**Use Chaining When:**
- Load factor can exceed 1
- Deletions are frequent
- Memory is not extremely constrained
- Hash function quality is uncertain
- Implementing is a priority (simpler)
- Keys/values are large (copying is expensive)

**Use Open Addressing When:**
- Load factor will stay < 0.7
- Cache performance is critical
- Memory is constrained (no pointers)
- Deletions are rare
- Hash function is high quality
- Keys/values are small (copying is cheap)

**Real-World Examples:**

- **Python dict**: Open addressing (until Python 3.6, now uses combined approach)
- **Java HashMap**: Chaining (switches to trees for long chains)
- **C++ std::unordered_map**: Implementation-defined (usually chaining)
- **Redis**: Chaining with rehashing
- **Go map**: Open addressing with quadratic probing

---

## 8. Advanced Topics and Optimizations

### 1. Robin Hood Hashing

An improvement on linear probing that reduces variance in probe lengths.

**Idea:** When inserting, if the current element has probed less than the element in the slot, swap them and continue inserting the displaced element.

```cpp
template<typename K, typename V>
class RobinHoodHashTable {
private:
    struct Entry {
        K key;
        V value;
        int psl;  // Probe Sequence Length
        bool occupied;
        
        Entry() : psl(0), occupied(false) {}
        Entry(const K& k, const V& v, int p) 
            : key(k), value(v), psl(p), occupied(true) {}
    };
    
    vector<Entry> table;
    size_t tableSize;
    size_t numElements;
    
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % tableSize;
    }
    
public:
    RobinHoodHashTable(size_t size = 101) 
        : tableSize(size), numElements(0) {
        table.resize(tableSize);
    }
    
    void insert(const K& key, const V& value) {
        size_t index = hash(key);
        Entry toInsert(key, value, 0);
        
        for (size_t i = 0; i < tableSize; i++) {
            size_t pos = (index + i) % tableSize;
            
            if (!table[pos].occupied) {
                // Empty slot found
                table[pos] = toInsert;
                numElements++;
                return;
            }
            
            if (table[pos].key == key) {
                // Update existing key
                table[pos].value = value;
                return;
            }
            
            // Robin Hood: If current entry has probed less than us, swap
            if (table[pos].psl < toInsert.psl) {
                swap(table[pos], toInsert);
            }
            
            toInsert.psl++;  // Increase probe count for displaced element
        }
        
        throw runtime_error("Table is full");
    }
    
    V* find(const K& key) {
        size_t index = hash(key);
        
        for (size_t i = 0; i < tableSize; i++) {
            size_t pos = (index + i) % tableSize;
            
            if (!table[pos].occupied) {
                return nullptr;
            }
            
            if (table[pos].key == key) {
                return &table[pos].value;
            }
            
            // Optimization: if current PSL < our probe length, key doesn't exist
            if (table[pos].psl < static_cast<int>(i)) {
                return nullptr;
            }
        }
        
        return nullptr;
    }
};
```

**Benefits:**
- More uniform probe lengths
- Faster searches (can terminate early)
- Better worst-case behavior

### 2. Cuckoo Hashing

Uses multiple hash tables and hash functions to guarantee O(1) worst-case lookup.

```cpp
template<typename K, typename V>
class CuckooHashTable {
private:
    struct Entry {
        K key;
        V value;
        bool occupied;
        
        Entry() : occupied(false) {}
        Entry(const K& k, const V& v) : key(k), value(v), occupied(true) {}
    };
    
    vector<Entry> table1;
    vector<Entry> table2;
    size_t tableSize;
    size_t numElements;
    
    static constexpr int MAX_ITERATIONS = 500;
    
    size_t hash1(const K& key) const {
        return std::hash<K>{}(key) % tableSize;
    }
    
    size_t hash2(const K& key) const {
        // Different hash function
        size_t h = 0;
        const char* bytes = reinterpret_cast<const char*>(&key);
        for (size_t i = 0; i < sizeof(K); i++) {
            h = h * 31 + bytes[i];
        }
        return h % tableSize;
    }
    
public:
    CuckooHashTable(size_t size = 101) 
        : tableSize(size), numElements(0) {
        table1.resize(tableSize);
        table2.resize(tableSize);
    }
    
    void insert(const K& key, const V& value) {
        // Check if key already exists
        if (find(key) != nullptr) {
            size_t pos1 = hash1(key);
            size_t pos2 = hash2(key);
            
            if (table1[pos1].occupied && table1[pos1].key == key) {
                table1[pos1].value = value;
            } else {
                table2[pos2].value = value;
            }
            return;
        }
        
        Entry toInsert(key, value);
        
        for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
            size_t pos1 = hash1(toInsert.key);
            
            if (!table1[pos1].occupied) {
                table1[pos1] = toInsert;
                numElements++;
                return;
            }
            
            // Kick out existing element
            swap(table1[pos1], toInsert);
            
            size_t pos2 = hash2(toInsert.key);
            
            if (!table2[pos2].occupied) {
                table2[pos2] = toInsert;
                numElements++;
                return;
            }
            
            // Kick out from table2
            swap(table2[pos2], toInsert);
        }
        
        // Rehash needed (cycle detected)
        rehash();
        insert(toInsert.key, toInsert.value);
    }
    
    V* find(const K& key) {
        size_t pos1 = hash1(key);
        if (table1[pos1].occupied && table1[pos1].key == key) {
            return &table1[pos1].value;
        }
        
        size_t pos2 = hash2(key);
        if (table2[pos2].occupied && table2[pos2].key == key) {
            return &table2[pos2].value;
        }
        
        return nullptr;  // O(1) worst-case!
    }
    
    void rehash() {
        // Implementation: increase size and reinsert all elements
        // Left as exercise
    }
};
```

**Characteristics:**
- O(1) worst-case lookup
- Amortized O(1) insertion
- More complex implementation
- Requires more space (2 tables)

### 3. Hopscotch Hashing

Combines linear probing with better cache performance.

**Idea:** Keep elements within a small neighborhood (usually 32 slots) of their hash position.

```cpp
template<typename K, typename V>
class HopscotchHashTable {
private:
    static constexpr int HOP_RANGE = 32;
    
    struct Entry {
        K key;
        V value;
        bool occupied;
        uint32_t hop_info;  // Bitmap of neighborhood
        
        Entry() : occupied(false), hop_info(0) {}
    };
    
    vector<Entry> table;
    size_t tableSize;
    
    // Implementation details omitted for brevity
    // Key idea: maintain hop_info bitmap showing which slots
    // in the neighborhood contain elements hashing here
};
```

### 4. Perfect Hashing

For static datasets, achieve O(1) worst-case with zero collisions.

**Two-level scheme:**
- First level: Regular hash table
- Second level: Perfect hash for each chain

```cpp
// For static, known datasets
template<typename K, typename V>
class PerfectHashTable {
    // Uses universal hashing to construct perfect hash function
    // O(n²) space in worst case, but often much better
    // Used in compilers for keyword lookup
};
```

---

## 9. Performance Analysis

### Comprehensive Benchmark

```cpp
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <iomanip>

class HashTableBenchmark {
private:
    static const int SMALL_SIZE = 1000;
    static const int MEDIUM_SIZE = 100000;
    static const int LARGE_SIZE = 1000000;
    
    template<typename HashTable>
    double benchmarkInsert(HashTable& table, const vector<int>& keys, const vector<string>& values) {
        auto start = chrono::high_resolution_clock::now();
        
        for (size_t i = 0; i < keys.size(); i++) {
            table.insert(keys[i], values[i]);
        }
        
        auto end = chrono::high_resolution_clock::now();
        return chrono::duration<double, milli>(end - start).count();
    }
    
    template<typename HashTable>
    double benchmarkSearch(HashTable& table, const vector<int>& keys) {
        auto start = chrono::high_resolution_clock::now();
        
        for (int key : keys) {
            volatile auto result = table.find(key);  // volatile prevents optimization
        }
        
        auto end = chrono::high_resolution_clock::now();
        return chrono::duration<double, milli>(end - start).count();
    }
    
    template<typename HashTable>
    double benchmarkDelete(HashTable& table, const vector<int>& keys) {
        auto start = chrono::high_resolution_clock::now();
        
        for (int key : keys) {
            table.remove(key);
        }
        
        auto end = chrono::high_resolution_clock::now();
        return chrono::duration<double, milli>(end - start).count();
    }
    
public:
    void runCompleteBenchmark() {
        vector<int> testSizes = {SMALL_SIZE, MEDIUM_SIZE, LARGE_SIZE};
        
        for (int size : testSizes) {
            cout << "\n========================================" << endl;
            cout << "Benchmark with " << size << " elements" << endl;
            cout << "========================================" << endl;
            
            // Generate random data
            vector<int> keys(size);
            vector<string> values(size);
            
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(1, size * 10);
            
            for (int i = 0; i < size; i++) {
                keys[i] = dis(gen);
                values[i] = "Value_" + to_string(i);
            }
            
            // Test Chaining
            {
                AdvancedHashChaining<int, string> table(size / 2);
                double insertTime = benchmarkInsert(table, keys, values);
                double searchTime = benchmarkSearch(table, keys);
                double deleteTime = benchmarkDelete(table, keys);
                
                cout << "\nChaining:" << endl;
                cout << "  Insert: " << fixed << setprecision(2) << insertTime << " ms" << endl;
                cout << "  Search: " << searchTime << " ms" << endl;
                cout << "  Delete: " << deleteTime << " ms" << endl;
                cout << "  Load Factor: " << table.loadFactor() << endl;
            }
            
            // Test Linear Probing
            {
                LinearProbingHashTable<int, string> table(size * 2);  // Keep load factor low
                double insertTime = benchmarkInsert(table, keys, values);
                double searchTime = benchmarkSearch(table, keys);
                double deleteTime = benchmarkDelete(table, keys);
                
                cout << "\nLinear Probing:" << endl;
                cout << "  Insert: " << insertTime << " ms" << endl;
                cout << "  Search: " << searchTime << " ms" << endl;
                cout << "  Delete: " << deleteTime << " ms" << endl;
                cout << "  Load Factor: " << table.loadFactor() << endl;
            }
            
            // Test Double Hashing
            {
                DoubleHashingTable<int, string> table(size * 2);
                double insertTime = benchmarkInsert(table, keys, values);
                double searchTime = benchmarkSearch(table, keys);
                
                cout << "\nDouble Hashing:" << endl;
                cout << "  Insert: " << insertTime << " ms" << endl;
                cout << "  Search: " << searchTime << " ms" << endl;
                cout << "  Load Factor: " << table.loadFactor() << endl;
            }
        }
    }
    
    // Memory usage analysis
    void analyzeMemoryUsage() {
        const int NUM_ELEMENTS = 100000;
        
        cout << "\n=== Memory Usage Analysis ===" << endl;
        cout << "For " << NUM_ELEMENTS << " elements:" << endl;
        
        // Chaining with linked list
        size_t chainingMemory = sizeof(void*) * NUM_ELEMENTS * 2;  // table + pointers
        chainingMemory += (sizeof(int) + sizeof(string) + sizeof(void*)) * NUM_ELEMENTS;  // nodes
        
        cout << "Chaining (linked list): ~" << chainingMemory / 1024 << " KB" << endl;
        
        // Open addressing
        size_t openAddressingMemory = (sizeof(int) + sizeof(string) + 1) * NUM_ELEMENTS * 1.5;
        
        cout << "Open Addressing: ~" << openAddressingMemory / 1024 << " KB" << endl;
        cout << "Memory savings: " << fixed << setprecision(1) 
             << (1.0 - static_cast<double>(openAddressingMemory) / chainingMemory) * 100 
             << "%" << endl;
    }
};
```

---

## 10. Choosing the Right Strategy

### Decision Framework

```cpp
class HashTableSelector {
public:
    enum Strategy {
        CHAINING_LINKED_LIST,
        CHAINING_VECTOR,
        LINEAR_PROBING,
        QUADRATIC_PROBING,
        DOUBLE_HASHING,
        ROBIN_HOOD,
        CUCKOO,
        HOPSCOTCH
    };
    
    Strategy recommendStrategy(
        bool frequentDeletions,
        bool memoryConstrained,
        bool needsWorstCaseGuarantee,
        bool largeKeys,
        double expectedLoadFactor,
        bool cachePerformanceCritical
    ) {
        // Worst-case guarantee needed
        if (needsWorstCaseGuarantee) {
            return CUCKOO;
        }
        
        // Frequent deletions
        if (frequentDeletions) {
            return CHAINING_LINKED_LIST;
        }
        
        // High load factor expected
        if (expectedLoadFactor > 0.75) {
            return CHAINING_LINKED_LIST;
        }
        
        // Memory constrained
        if (memoryConstrained) {
            if (cachePerformanceCritical) {
                return ROBIN_HOOD;
            }
            return DOUBLE_HASHING;
        }
        
        // Large keys (expensive to copy)
        if (largeKeys) {
            return CHAINING_LINKED_LIST;
        }
        
        // Cache performance critical
        if (cachePerformanceCritical) {
            return LINEAR_PROBING;
        }
        
        // General purpose
        return DOUBLE_HASHING;
    }
    
    void printRecommendation(Strategy strategy) {
        cout << "Recommended strategy: ";
        switch (strategy) {
            case CHAINING_LINKED_LIST:
                cout << "Chaining with Linked Lists" << endl;
                cout << "Reason: Simple, handles deletions well, supports high load factors" << endl;
                break;
            case LINEAR_PROBING:
                cout << "Linear Probing" << endl;
                cout << "Reason: Best cache performance, simple implementation" << endl;
                break;
            case DOUBLE_HASHING:
                cout << "Double Hashing" << endl;
                cout << "Reason: Good all-around performance, minimal clustering" << endl;
                break;
            case ROBIN_HOOD:
                cout << "Robin Hood Hashing" << endl;
                cout << "Reason: Excellent average case, low variance" << endl;
                break;
            case CUCKOO:
                cout << "Cuckoo Hashing" << endl;
                cout << "Reason: O(1) worst-case lookup required" << endl;
                break;
            default:
                cout << "Other specialized variant" << endl;
        }
    }
};
```

### Summary Table

| Criterion | Chaining | Open Addressing |
|-----------|----------|-----------------|
| **Implementation Complexity** | Simple | Moderate |
| **Memory Overhead** | High (pointers) | Low |
| **Cache Performance** | Poor | Good |
| **Load Factor Limit** | >1 allowed | Must be <1 |
| **Deletion** | Simple | Complex |
| **Worst Case** | O(n) | O(n) |
| **Hash Function Sensitivity** | Low | High |
| **Best Use Case** | General purpose, frequent deletes | Cache-critical, memory-constrained |

---

## Conclusion

Both chaining and open addressing are powerful collision resolution strategies with distinct trade-offs:

**Chaining** excels in:
- Simplicity of implementation
- Handling high load factors
- Frequent deletion scenarios
- Uncertain hash function quality

**Open Addressing** excels in:
- Memory efficiency
- Cache performance
- Controlled load factors
- Small key/value sizes

The choice depends on your specific requirements: memory constraints, access patterns, deletion frequency, and performance characteristics needed for your application.

Modern hash table implementations often use hybrid approaches or sophisticated variants like Robin Hood hashing or Cuckoo hashing to get the best of both worlds.

Understanding these fundamental strategies and their trade-offs is essential for:
- Implementing efficient data structures
- Choosing the right standard library container
- Optimizing performance-critical applications
- Understanding language runtime implementations

---

## Practice Problems

1. Implement a hash table that automatically switches from chaining to open addressing based on load factor.

2. Analyze the probe sequence length distribution for 10,000 random insertions in linear probing vs double hashing.

3. Implement lazy deletion for double hashing with automatic table cleanup when tombstone ratio exceeds 25%.

4. Design a hash table for a spell checker that minimizes memory while maintaining fast lookup.

5. Implement a concurrent hash table using either chaining with fine-grained locking or optimistic concurrency for open addressing.

---

## References and Further Reading

1. **Cormen, Leiserson, Rivest, Stein** - "Introduction to Algorithms" (CLRS), Chapter 11
2. **Knuth** - "The Art of Computer Programming, Volume 3: Sorting and Searching"
3. **Sedgewick & Wayne** - "Algorithms, 4th Edition"
4. Papers on modern variants:
   - "Robin Hood Hashing" by Pedro Celis
   - "Cuckoo Hashing" by Pagh and Rodler
   - "Hopscotch Hashing" by Herlihy, Shavit, and Tzafrir
