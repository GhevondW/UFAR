# Prefix Trees (Tries): Structure, Algorithms, and Applications

## Table of Contents
1. [Introduction to Prefix Trees](#introduction-to-prefix-trees)
2. [The Problem: Why We Need Tries](#the-problem-why-we-need-tries)
3. [Trie Structure and Properties](#trie-structure-and-properties)
4. [Core Operations](#core-operations)
5. [Variants](#variants)
6. [Comparison with Other Data Structures](#comparison-with-other-data-structures)
7. [Complete C++ Implementation](#complete-c-implementation)

---

## Introduction to Prefix Trees

### What is a Trie?

A **Trie** (pronounced "try", derived from re**trie**val) is a tree-shaped data structure used to store strings where each node represents a single character. Unlike a BST where a node holds a complete key, a trie distributes a key across a path from root to a terminal node.

```
Trie storing: "cat", "car", "card", "care", "bat"

         root
        /    \
       c      b
       |      |
       a      a
      / \     |
     t   r    t*
    *   / \
       d*  e*

* = marks end of a valid word
```

Every path from the root to a marked node spells out a stored word.

### Key Terminology

- **Root**: Empty node; the starting point for all lookups
- **Edge**: Represents a single character transition
- **Node**: Holds a map of children (one per possible next character)
- **Terminal / End marker**: Flag on a node indicating a complete word ends here
- **Prefix**: Any leading substring of a stored string
- **Depth**: The length of the prefix represented at that node
- **Alphabet size (Σ)**: Number of distinct characters (26 for lowercase ASCII, 128 for full ASCII, etc.)

---

## The Problem: Why We Need Tries

### What Hash Maps and BSTs Miss

For string keys, both hash maps and BSTs treat each key as an opaque unit. Given a set of strings, they cannot answer questions like:

- "Does any stored word start with `pre`?"
- "List all words matching the pattern `ca_`"
- "What is the longest stored word that is a prefix of this query?"

Tries answer all of these natively, in time proportional to the **length of the query string**, not the number of stored strings.

### Performance Comparison

| Query type | Hash Map | BST (balanced) | Trie |
|---|---|---|---|
| Exact lookup | O(L) avg | O(L log n) | O(L) |
| Prefix existence | O(n·L) | O(L log n) | **O(L)** |
| All words with prefix | O(n·L) | O(L log n + k) | **O(L + k)** |
| Lexicographic sort | O(n log n) | O(n) in-order | **O(n)** DFS |
| Longest prefix match | O(L²) | O(L log n) | **O(L)** |

Where **L** = length of query string, **n** = number of stored strings, **k** = number of results returned.

### Real-World Use Cases

- **Autocomplete / search suggestions**: Type `"searc"` → retrieve all stored completions
- **Spell checkers**: Check whether a word exists; suggest nearest matches
- **IP routing tables**: Longest-prefix match on binary representations of addresses
- **T9 / mobile keyboards**: Predictive text on numeric keys
- **DNA sequence search**: Matching over a 4-character alphabet (A/C/G/T)
- **Compiler symbol tables**: Fast identifier lookup with namespace prefix filtering
- **Browser URL bar**: Prefix matching over history and bookmarks

---

## Trie Structure and Properties

### Node Structure

Each trie node stores:
1. A map from characters to child nodes
2. A boolean flag marking whether the path to this node forms a complete word

```cpp
struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool is_end;

    TrieNode() : is_end(false) {}
};
```

For a fixed lowercase alphabet, a flat array of 26 pointers is often used instead of a map — trading memory for cache locality:

```cpp
struct TrieNode {
    TrieNode* children[26];
    bool is_end;

    TrieNode() : is_end(false) {
        std::fill(children, children + 26, nullptr);
    }
};
```

### Visual Walkthrough: Building a Trie

Insert `"app"`, `"apple"`, `"apt"`, `"bat"` step by step:

**After inserting `"app"`:**
```
root
 └─ a
     └─ p
         └─ p*
```

**After inserting `"apple"`:**
```
root
 └─ a
     └─ p
         └─ p*
             └─ l
                 └─ e*
```

**After inserting `"apt"`:**
```
root
 └─ a
     └─ p
        ├─ p*
        │   └─ l
        │       └─ e*
        └─ t*
```

**After inserting `"bat"`:**
```
root
 ├─ a
 │   └─ p
 │      ├─ p*
 │      │   └─ l
 │      │       └─ e*
 │      └─ t*
 └─ b
     └─ a
         └─ t*
```

Notice that `"app"` and `"apple"` **share the prefix path** `a → p → p`. This prefix compression is what makes tries memory-efficient when many strings share common prefixes.

---

## Core Operations

### Insert

Walk the trie character by character. Create any missing nodes. Mark the final node as a word end.

```
Insert("card"):
1. At root, follow edge 'c' (create if missing)
2. At 'c', follow edge 'a' (create if missing)
3. At 'a', follow edge 'r' (create if missing)
4. At 'r', follow edge 'd' (create if missing)
5. Mark node at 'd' as is_end = true
```

**Time complexity**: O(L) — one node visit per character  
**Space complexity**: O(L) in the worst case (no shared prefix with existing strings)

### Search (Exact Match)

Walk the trie along the query characters. If any character's edge is missing, the word is absent. If we exhaust all characters, check the `is_end` flag.

```
Search("car"):
  root → c → a → r   is_end? → must be true to count as found

Search("ca"):
  root → c → a        is_end? → false (no word "ca" was inserted)
  → NOT FOUND (path exists but no terminal marker)

Search("xyz"):
  root → 'x' edge missing
  → NOT FOUND immediately
```

**Time complexity**: O(L)

### Prefix Search (StartsWith)

Identical to Search, but we **do not** check `is_end` at the end — we only verify that the path exists:

```
StartsWith("car"):
  root → c → a → r   path exists? → YES
  (Returns true even if "car" itself was never inserted as a complete word)
```

**Time complexity**: O(L)

### Delete

Deletion is the most nuanced operation. We must:

1. Verify the word exists
2. Unmark the terminal node
3. **Prune** any nodes that are now unused (no children, not a terminal for another word)

```
Delete("app") from a trie containing "app" and "apple":

Before:
  a → p → p* → l → e*

After deleting "app":
  a → p → p  → l → e*
              (is_end cleared, but node kept because 'l' child exists)
```

If we deleted `"apple"` instead:

```
Before:
  a → p → p* → l → e*

After deleting "apple":
  a → p → p*
          (l and e nodes pruned — they have no children and are not word ends)
```

**Time complexity**: O(L)

### Autocomplete (Collect All Words with Prefix)

1. Walk to the node representing the end of the prefix
2. DFS from that node, collecting all paths that reach a terminal node

```
Autocomplete("car") from trie containing "car", "card", "care", "cart", "cat":

Walk to node at end of "car":
  root → c → a → r

DFS from 'r':
  r* → emit "car"
  r  → d* → emit "card"
  r  → e* → emit "care"
  r  → t* → emit "cart"

Result: ["car", "card", "care", "cart"]
"cat" is not returned because it does not share the "car" prefix
```

**Time complexity**: O(L + k) where k = total characters in all returned words

---

## Variants

### Compressed Trie (Patricia Trie / Radix Tree)

A standard trie can waste space on long chains of single-child nodes. A **compressed trie** collapses such chains into a single edge labeled with the full substring:

```
Standard trie for "interview", "into", "inside":

  i → n → t → e → r → v → i → e → w*
                └─ o*
          └─ s → i → d → e*

Compressed trie (Radix Tree):

  i → n → t → "erview"*
            └─ "o"*
        └─ "side"*
```

This reduces node count dramatically for sparse key sets. The Linux kernel VFS uses a radix tree for page cache indexing.

**Space**: O(n) nodes regardless of key length (n = number of keys)

### Ternary Search Tree (TST)

A TST blends a BST and a trie. Each node stores one character and has three children:
- **Left**: characters less than the stored character
- **Middle**: next character in the string (matching)
- **Right**: characters greater than the stored character

TSTs use significantly less memory than tries for large alphabets, at the cost of O(log n) rather than O(1) branching at each level.

### Suffix Trie / Suffix Tree

A **suffix trie** inserts all suffixes of a text string. This enables substring search in O(L) time rather than O(n·L). Used in bioinformatics, text editors, and plagiarism detection. In compressed form this becomes a **suffix tree**.

### Bitwise Trie

For integer or IP address keys, a trie over individual bits (alphabet = {0, 1}) enables extremely fast longest-prefix match. This is the basis for many IP routing implementations (LPM — Longest Prefix Match).

---

## Comparison with Other Data Structures

| | Trie | Hash Map | Balanced BST |
|---|---|---|---|
| Exact lookup | O(L) | O(L) avg | O(L log n) |
| Prefix queries | **O(L)** | O(n·L) | O(L log n) |
| Ordered iteration | **O(n·L)** DFS | Not supported | O(n·L) |
| Memory (dense keys) | High (many pointers) | Low | Low |
| Memory (sparse/prefixed keys) | **Low** | High | Moderate |
| Worst-case guarantees | O(L) always | O(L·n) on collision | O(L log n) |
| Implementation complexity | Moderate | Low | High |

**Rule of thumb**: Use a trie when prefix operations matter or when you have many strings sharing common prefixes. Use a hash map when only exact lookups are needed.

---

## Complete C++ Implementation

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class Trie {
private:
    struct TrieNode {
        std::unordered_map<char, TrieNode*> children;
        bool is_end;
        TrieNode() : is_end(false) {}
    };

    TrieNode* root;

    // DFS helper: collect all words reachable from `node`, prepending `prefix`
    void collectWords(TrieNode* node, const std::string& prefix,
                      std::vector<std::string>& results) const {
        if (node->is_end) {
            results.push_back(prefix);
        }
        for (auto& [ch, child] : node->children) {
            collectWords(child, prefix + ch, results);
        }
    }

    // Recursive delete; returns true if the current node should be pruned
    bool deleteHelper(TrieNode* node, const std::string& word, int depth) {
        if (depth == static_cast<int>(word.size())) {
            if (!node->is_end) return false;   // Word not present
            node->is_end = false;
            return node->children.empty();     // Prune if leaf
        }

        char ch = word[depth];
        auto it = node->children.find(ch);
        if (it == node->children.end()) return false;  // Word not present

        bool should_prune = deleteHelper(it->second, word, depth + 1);
        if (should_prune) {
            delete it->second;
            node->children.erase(it);
            // Prune current node if it has no other purpose
            return !node->is_end && node->children.empty();
        }
        return false;
    }

    void destroyTree(TrieNode* node) {
        for (auto& [ch, child] : node->children) {
            destroyTree(child);
        }
        delete node;
    }

public:
    Trie() : root(new TrieNode()) {}

    ~Trie() { destroyTree(root); }

    // ---------------------------------------------------------------
    // INSERT: O(L)
    // ---------------------------------------------------------------
    void insert(const std::string& word) {
        TrieNode* cur = root;
        for (char ch : word) {
            if (!cur->children.count(ch)) {
                cur->children[ch] = new TrieNode();
            }
            cur = cur->children[ch];
        }
        cur->is_end = true;
    }

    // ---------------------------------------------------------------
    // SEARCH: O(L) — exact match
    // ---------------------------------------------------------------
    bool search(const std::string& word) const {
        TrieNode* cur = root;
        for (char ch : word) {
            auto it = cur->children.find(ch);
            if (it == cur->children.end()) return false;
            cur = it->second;
        }
        return cur->is_end;
    }

    // ---------------------------------------------------------------
    // STARTS WITH: O(L) — prefix existence check
    // ---------------------------------------------------------------
    bool startsWith(const std::string& prefix) const {
        TrieNode* cur = root;
        for (char ch : prefix) {
            auto it = cur->children.find(ch);
            if (it == cur->children.end()) return false;
            cur = it->second;
        }
        return true;  // Don't check is_end
    }

    // ---------------------------------------------------------------
    // AUTOCOMPLETE: O(L + k) — all words with given prefix
    // ---------------------------------------------------------------
    std::vector<std::string> autocomplete(const std::string& prefix) const {
        std::vector<std::string> results;
        TrieNode* cur = root;

        // Walk to end of prefix
        for (char ch : prefix) {
            auto it = cur->children.find(ch);
            if (it == cur->children.end()) return results;  // No matches
            cur = it->second;
        }

        // Collect all words reachable from here
        collectWords(cur, prefix, results);
        return results;
    }

    // ---------------------------------------------------------------
    // DELETE: O(L)
    // ---------------------------------------------------------------
    bool remove(const std::string& word) {
        return deleteHelper(root, word, 0);
    }

    // ---------------------------------------------------------------
    // COUNT WORDS WITH PREFIX: O(L + subtree size)
    // ---------------------------------------------------------------
    int countWithPrefix(const std::string& prefix) const {
        return static_cast<int>(autocomplete(prefix).size());
    }

    // ---------------------------------------------------------------
    // LONGEST PREFIX OF query that exists in the trie: O(L)
    // ---------------------------------------------------------------
    std::string longestPrefixMatch(const std::string& query) const {
        TrieNode* cur = root;
        std::string longest;
        std::string current;

        for (char ch : query) {
            auto it = cur->children.find(ch);
            if (it == cur->children.end()) break;
            current += ch;
            cur = it->second;
            if (cur->is_end) longest = current;
        }
        return longest;
    }
};

// ---------------------------------------------------------------
// DEMO
// ---------------------------------------------------------------
int main() {
    Trie trie;

    std::vector<std::string> words = {
        "apple", "app", "application", "apply",
        "apt", "bat", "batch", "bath", "car", "card", "care", "cart"
    };

    std::cout << "=== Inserting words ===\n";
    for (const auto& w : words) {
        trie.insert(w);
        std::cout << "  Inserted: " << w << "\n";
    }

    std::cout << "\n=== Exact Search ===\n";
    for (const auto& q : {"app", "ap", "apple", "batch", "bat", "ba"}) {
        std::cout << "  search(\"" << q << "\") = "
                  << (trie.search(q) ? "FOUND" : "NOT FOUND") << "\n";
    }

    std::cout << "\n=== Prefix Check ===\n";
    for (const auto& p : {"app", "ap", "car", "xyz"}) {
        std::cout << "  startsWith(\"" << p << "\") = "
                  << (trie.startsWith(p) ? "YES" : "NO") << "\n";
    }

    std::cout << "\n=== Autocomplete ===\n";
    for (const auto& p : {"app", "ba", "car", "z"}) {
        auto results = trie.autocomplete(p);
        std::cout << "  autocomplete(\"" << p << "\") = [";
        for (size_t i = 0; i < results.size(); i++) {
            std::cout << "\"" << results[i] << "\"";
            if (i + 1 < results.size()) std::cout << ", ";
        }
        std::cout << "]\n";
    }

    std::cout << "\n=== Longest Prefix Match ===\n";
    for (const auto& q : {"applejuice", "batcher", "cartoon", "xyz"}) {
        std::cout << "  longestPrefixMatch(\"" << q << "\") = \""
                  << trie.longestPrefixMatch(q) << "\"\n";
    }

    std::cout << "\n=== Delete ===\n";
    std::cout << "  Deleting \"app\"\n";
    trie.remove("app");
    std::cout << "  search(\"app\") = "
              << (trie.search("app") ? "FOUND" : "NOT FOUND") << "\n";
    std::cout << "  search(\"apple\") = "
              << (trie.search("apple") ? "FOUND" : "NOT FOUND") << "\n";
    std::cout << "  startsWith(\"app\") = "
              << (trie.startsWith("app") ? "YES" : "NO") << "\n";

    return 0;
}
```

---

## Time and Space Complexity Summary

| Operation | Time | Space |
|---|---|---|
| Insert | O(L) | O(L) worst case |
| Search | O(L) | O(1) |
| StartsWith | O(L) | O(1) |
| Delete | O(L) | O(1) |
| Autocomplete | O(L + k) | O(k) results |
| Longest prefix | O(L) | O(1) |
| Build trie from n strings | O(n·L) | O(n·L) |

**L** = length of the query/inserted string  
**k** = size of the result set

### Space Analysis

A naive trie with array-based children (`char[26]`) uses O(n · L · 26) pointers. A hash-map-based trie uses O(n · L) on average, but with pointer overhead per node. Compressed tries (radix trees) reduce node count to O(n) at the cost of edge label storage.

---

## Key Takeaways

1. **Tries enable O(L) prefix queries** — something hash maps and BSTs cannot match without scanning the entire collection.
2. **Memory scales with shared prefixes** — tries are most efficient when the stored strings share long common prefixes.
3. **Autocomplete is a natural fit** — the trie structure directly mirrors the character-by-character input process.
4. **Deletion requires careful pruning** — unmarking a terminal node is not enough; orphaned nodes must be reclaimed.
5. **Variants trade space for time** — radix trees, TSTs, and bitwise tries each optimize for different constraints.
