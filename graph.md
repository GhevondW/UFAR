# Graphs: Representations, Traversals, and Classic Algorithms

## Table of Contents
1. [Introduction to Graphs](#introduction-to-graphs)
2. [Graph Representations](#graph-representations)
3. [Graph Algorithms & Problem Formulation](#graph-algorithms--problem-formulation)
   - 3.1 [Complex Graph Traversal & Connectivity](#complex-graph-traversal--connectivity)
     - 3.1.1 [Strongly Connected Components and Topological Sort](#strongly-connected-components-scc)
     - 3.1.2 [Shortest Path Paradigms: Dijkstra and Bellman-Ford](#shortest-path-paradigms-dijkstra-and-bellman-ford)
   - 3.2 [Minimum Spanning Tree](#minimum-spanning-tree)
4. [Comparison and Use Cases](#comparison-and-use-cases)
5. [Complete C++ Implementations](#complete-c-implementations)

---

## Introduction to Graphs

### What is a Graph?

A **graph** G = (V, E) is a collection of **vertices** (nodes) and **edges** (connections between nodes). Unlike trees, graphs have no inherent root, no parent-child hierarchy, and may contain cycles.

```
Undirected graph:        Directed graph (digraph):
  1 --- 2                  1 --→ 2
  |   / |                  ↑   / |
  |  /  |                  |  /  ↓
  | /   |                  | /   3
  3 --- 4                  3
```

### Key Terminology

- **Vertex (node)**: A fundamental unit; often labeled with an integer index or key
- **Edge**: A connection between two vertices; may be directed or undirected
- **Directed graph (digraph)**: Edges have a direction (u → v ≠ v → u)
- **Undirected graph**: Edges are bidirectional (u — v implies v — u)
- **Weighted graph**: Each edge carries a numeric cost/weight
- **Adjacent vertices**: Two vertices connected by an edge
- **Degree**: Number of edges incident to a vertex; in digraphs, split into **in-degree** and **out-degree**
- **Path**: A sequence of vertices where consecutive pairs are connected by edges
- **Cycle**: A path that starts and ends at the same vertex
- **Connected graph**: Every vertex is reachable from every other vertex (for undirected graphs)
- **Strongly connected**: Every vertex is reachable from every other vertex in both directions (for digraphs)
- **DAG**: Directed Acyclic Graph — a digraph with no cycles
- **Sparse graph**: |E| ≪ |V|²; **Dense graph**: |E| ≈ |V|²

### Why Graphs Matter

Graphs model a vast class of real-world problems:

| Domain | Vertices | Edges |
|---|---|---|
| Road networks | Intersections | Roads |
| Social networks | Users | Friendships / follows |
| The web | Web pages | Hyperlinks |
| Dependency resolution | Packages | Dependencies |
| Circuit design | Components | Wires |
| Task scheduling | Tasks | Prerequisite constraints |
| Neural networks | Neurons | Weighted synapses |

---

## Graph Representations

The choice of representation determines the time complexity of every subsequent operation. The two primary choices are **adjacency matrix** and **adjacency list**.

### Adjacency Matrix

A 2D array `matrix[u][v]` stores the weight of edge (u, v), or 0/∞ if no edge exists.

```
Graph:         Adjacency matrix (unweighted):
  0 -- 1           0  1  2  3
  |    |      0  [ 0  1  1  0 ]
  2 -- 3      1  [ 1  0  0  1 ]
              2  [ 1  0  0  1 ]
              3  [ 0  1  1  0 ]
```

```cpp
// Adjacency matrix (fixed-size, unweighted)
const int V = 4;
int matrix[V][V] = {
    {0, 1, 1, 0},
    {1, 0, 0, 1},
    {1, 0, 0, 1},
    {0, 1, 1, 0}
};

// Check if edge (u, v) exists: O(1)
bool hasEdge(int u, int v) { return matrix[u][v] != 0; }
```

**Space**: O(V²) regardless of edge count  
**Edge check**: O(1)  
**Enumerate neighbors**: O(V) — must scan entire row  
**Best for**: Dense graphs where V is small and fast edge-existence queries are needed

### Adjacency List

Each vertex stores a list of its neighbors (and edge weights if the graph is weighted).

```
Graph:               Adjacency list:
  0 -- 1             0: [1, 2]
  |    |             1: [0, 3]
  2 -- 3             2: [0, 3]
                     3: [1, 2]
```

```cpp
#include <vector>
#include <utility>

// Unweighted
std::vector<std::vector<int>> adj(V);
adj[0] = {1, 2};
adj[1] = {0, 3};

// Weighted (neighbor, weight)
std::vector<std::vector<std::pair<int,int>>> wadj(V);
wadj[0].push_back({1, 5});   // Edge 0→1 with weight 5
wadj[0].push_back({2, 3});   // Edge 0→2 with weight 3
```

**Space**: O(V + E)  
**Edge check**: O(degree(v)) — scan neighbor list  
**Enumerate neighbors**: O(degree(v))  
**Best for**: Sparse graphs (the common case)

### Edge List

A flat list of (u, v, weight) tuples — minimal memory, convenient for algorithms that process edges in bulk (e.g., Kruskal's MST).

```cpp
struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const { return weight < other.weight; }
};

std::vector<Edge> edges = { {0, 1, 4}, {0, 2, 3}, {1, 3, 2}, {2, 3, 6} };
```

### Comparison

| | Adjacency Matrix | Adjacency List | Edge List |
|---|---|---|---|
| Space | O(V²) | **O(V + E)** | O(E) |
| Add edge | O(1) | O(1) | O(1) |
| Remove edge | O(1) | O(degree) | O(E) |
| Edge existence | **O(1)** | O(degree) | O(E) |
| Iterate neighbors | O(V) | **O(degree)** | O(E) |
| Best for | Dense, small | **Sparse (typical)** | Bulk edge algorithms |

---

## Graph Algorithms & Problem Formulation

## Complex Graph Traversal & Connectivity

Traversal algorithms visit every reachable vertex exactly once. They form the foundation for almost every other graph algorithm.

### Breadth-First Search (BFS)

BFS explores vertices level by level — first all neighbors at distance 1, then distance 2, and so on. It uses a **queue**.

```
Graph:              BFS from vertex 0:
  0 -- 1 -- 3
  |         |       Level 0: [0]
  2 -- 4    |       Level 1: [1, 2]
       |         Level 2: [3, 4]
       5          Level 3: [5]

Visit order: 0, 1, 2, 3, 4, 5
```

**Properties:**
- Finds **shortest path** (fewest edges) in unweighted graphs
- Discovered shortest distances are exact: `dist[v] = min edges from source to v`
- Time: O(V + E)
- Space: O(V) for the visited array and queue

```cpp
#include <queue>
#include <vector>

void bfs(const std::vector<std::vector<int>>& adj, int source) {
    int V = adj.size();
    std::vector<bool> visited(V, false);
    std::vector<int>  dist(V, -1);

    std::queue<int> q;
    visited[source] = true;
    dist[source] = 0;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        std::cout << u << " ";

        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}
```

### Depth-First Search (DFS)

DFS explores as deep as possible along each branch before backtracking. It can be implemented recursively (implicit stack) or iteratively (explicit stack).

```
Graph:                DFS from vertex 0:
  0 -- 1 -- 3
  |         |         Visit 0 → go deep → 1 → 3 → 2 (backtrack)
  2 -- 4               → 4 → 5

Visit order (one possible): 0, 1, 3, 2, 4, 5
```

**Properties:**
- Natural for detecting cycles, topological sort, connected components
- Produces **DFS tree** with discovery and finish timestamps
- Time: O(V + E)
- Space: O(V) for the visited array and call stack

```cpp
void dfsRecursive(const std::vector<std::vector<int>>& adj,
                  int u, std::vector<bool>& visited) {
    visited[u] = true;
    std::cout << u << " ";

    for (int v : adj[u]) {
        if (!visited[v]) {
            dfsRecursive(adj, v, visited);
        }
    }
}

void dfs(const std::vector<std::vector<int>>& adj, int source) {
    std::vector<bool> visited(adj.size(), false);
    dfsRecursive(adj, source, visited);
}
```

### BFS vs DFS

| Aspect | BFS | DFS |
|---|---|---|
| Data structure | Queue (FIFO) | Stack / recursion (LIFO) |
| Shortest path (unweighted) | **Yes** | No |
| Memory (wide graph) | High | Low |
| Memory (deep graph) | Low | High (stack overflow risk) |
| Cycle detection | Yes | **Yes (simpler)** |
| Topological sort | No | **Yes** |
| Connected components | Yes | Yes |
| Typical use | Shortest path, level order | Topology, SCC, backtracking |

### Why O(V + E)? — Including Dense Graphs

O(V + E) is a **parameterized** bound, not a best-case claim. It expands correctly for any graph density:

- **Sparse graph** (E ≈ V): O(V + E) = O(V)
- **Dense graph** (E ≈ V²): O(V + E) = O(V²)

The work splits into two independent parts:

**The V term** — every vertex is processed exactly once (marked visited, enqueued/entered). That's O(1) per vertex → O(V) total.

**The E term** — every edge is examined exactly once. For each vertex `u`, you iterate over its neighbor list, which costs O(degree(u)). Summed across all vertices:

```
∑ degree(u) for all u  =  E   (directed)
                        =  2E  (undirected)
```

You do **not** pay O(V) per vertex for neighbor iteration — you pay O(degree(u)), and the sum of all degrees equals E. This is the key identity behind the formula.

**Representation matters.** With an adjacency matrix, finding neighbors of `u` requires scanning the full row (V entries) even if `u` has only 2 neighbors. That forces O(V²) regardless of actual edge count:

| Representation | Sparse (E ≈ V) | Dense (E ≈ V²) |
|---|---|---|
| Adjacency list | **O(V)** | O(V²) |
| Adjacency matrix | O(V²) | O(V²) |

For dense graphs both representations converge to O(V²). For sparse graphs the adjacency list is strictly superior.

---

## Shortest Path Paradigms: Dijkstra and Bellman-Ford

### Dijkstra's Algorithm

Finds the shortest path from a **single source** to all vertices in a graph with **non-negative edge weights**.

**Core idea**: Greedily extract the unvisited vertex with the smallest known distance, then relax its outgoing edges.

```
Graph (weighted):         Dijkstra from vertex 0:
  0 --(4)--> 1            Step 1: Extract 0 (dist=0)
  0 --(1)--> 2              Relax: dist[1]=4, dist[2]=1
  2 --(2)--> 1            Step 2: Extract 2 (dist=1)
  1 --(1)--> 3              Relax: dist[1]=min(4, 1+2)=3
  2 --(5)--> 3            Step 3: Extract 1 (dist=3)
                            Relax: dist[3]=3+1=4
                          Step 4: Extract 3 (dist=4) — done

Final distances: [0:0, 1:3, 2:1, 3:4]
Shortest paths:  0→2→1→3
```

```cpp
#include <queue>
#include <vector>
#include <limits>

using pii = std::pair<int,int>;  // (distance, vertex)

std::vector<int> dijkstra(
    const std::vector<std::vector<pii>>& adj, int source) {

    int V = adj.size();
    std::vector<int> dist(V, std::numeric_limits<int>::max());
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // Skip stale entries (a shorter path was already found)
        if (d > dist[u]) continue;

        for (auto [weight, v] : adj[u]) {
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}
```

**Time complexity**: O((V + E) log V) with a binary heap  
**Limitation**: Does not handle **negative edge weights**

### Bellman-Ford Algorithm

Handles **negative edge weights** and detects **negative cycles**. Works by relaxing all edges V-1 times.

**Core idea**: After k relaxation passes, `dist[v]` holds the shortest path using at most k edges. A correct shortest path uses at most V-1 edges, so V-1 passes suffice. A V-th pass that still finds improvements indicates a negative cycle.

```cpp
struct Edge { int u, v, weight; };

std::vector<int> bellmanFord(
    int V, const std::vector<Edge>& edges, int source) {

    std::vector<int> dist(V, std::numeric_limits<int>::max());
    dist[source] = 0;

    // Relax all edges V-1 times
    for (int i = 0; i < V - 1; ++i) {
        for (const auto& e : edges) {
            if (dist[e.u] != std::numeric_limits<int>::max() &&
                dist[e.u] + e.weight < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.weight;
            }
        }
    }

    // Check for negative cycles
    for (const auto& e : edges) {
        if (dist[e.u] != std::numeric_limits<int>::max() &&
            dist[e.u] + e.weight < dist[e.v]) {
            std::cout << "Negative cycle detected!\n";
            return {};
        }
    }

    return dist;
}
```

**Time complexity**: O(V · E) — slower than Dijkstra, but handles negative weights

### Floyd-Warshall Algorithm

Finds **all-pairs shortest paths** in a single pass. Works with negative weights (but not negative cycles).

**Core idea**: For each intermediate vertex k, check whether routing through k improves the path from i to j.

```cpp
void floydWarshall(std::vector<std::vector<int>>& dist, int V) {
    // dist[i][j] is initialized to edge weight or INF if no direct edge

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}
```

**Time complexity**: O(V³)  
**Space complexity**: O(V²)  
**Best for**: Dense graphs or when all-pairs distances are needed

### Shortest Path Algorithm Comparison

| Algorithm | Graph type | Weights | Time | Space | Best for |
|---|---|---|---|---|---|
| BFS | Unweighted | — | O(V+E) | O(V) | Fewest-hop paths |
| Dijkstra | Weighted | Non-negative | O((V+E) log V) | O(V) | Single source |
| Bellman-Ford | Weighted | Any | O(V·E) | O(V) | Negative weights |
| Floyd-Warshall | Weighted | Any | O(V³) | O(V²) | All pairs |

---

## Minimum Spanning Tree

A **Minimum Spanning Tree (MST)** of a connected weighted undirected graph is a spanning subgraph (touches all vertices) with the minimum total edge weight and no cycles.

```
Graph:                    MST (Kruskal picks cheapest non-cycle edges):
  0 -(1)- 1               0 -(1)- 1
  |       |               |
 (4)     (2)             (4)
  |       |               |
  2 -(3)- 3               2 -(2)-3? No, pick (2) edge: 1-3
                          Final: edges (0,1,1), (1,3,2), (0,2,4) → total = 7
```

### Kruskal's Algorithm

Sort edges by weight. Greedily add the cheapest edge that does not create a cycle. Uses a **Union-Find (Disjoint Set Union)** structure to detect cycles in O(α(V)) ≈ O(1).

```cpp
#include <algorithm>
#include <numeric>

struct UnionFind {
    std::vector<int> parent, rank;

    UnionFind(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);  // Path compression
        return parent[x];
    }

    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;  // Already in same component — would create cycle
        if (rank[px] < rank[py]) std::swap(px, py);
        parent[py] = px;
        if (rank[px] == rank[py]) rank[px]++;
        return true;
    }
};

std::vector<Edge> kruskal(int V, std::vector<Edge> edges) {
    std::sort(edges.begin(), edges.end());  // Sort by weight (Edge::operator<)
    UnionFind uf(V);
    std::vector<Edge> mst;

    for (const auto& e : edges) {
        if (uf.unite(e.u, e.v)) {
            mst.push_back(e);
            if (static_cast<int>(mst.size()) == V - 1) break;  // MST complete
        }
    }

    return mst;  // V-1 edges forming the MST
}
```

**Time complexity**: O(E log E) dominated by sorting  
**Best for**: Sparse graphs

### Prim's Algorithm

Start from any vertex. Greedily add the cheapest edge that connects a new vertex to the growing MST. Uses a **min-heap** (similar structure to Dijkstra).

```cpp
std::vector<Edge> prim(const std::vector<std::vector<pii>>& adj, int V) {
    std::vector<bool> inMST(V, false);
    std::vector<int>  key(V, std::numeric_limits<int>::max());
    std::vector<int>  parent(V, -1);
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

    key[0] = 0;
    pq.push({0, 0});

    std::vector<Edge> mst;

    while (!pq.empty()) {
        auto [w, u] = pq.top();
        pq.pop();

        if (inMST[u]) continue;
        inMST[u] = true;

        if (parent[u] != -1) {
            mst.push_back({parent[u], u, w});
        }

        for (auto [weight, v] : adj[u]) {
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    return mst;
}
```

**Time complexity**: O((V + E) log V) with binary heap  
**Best for**: Dense graphs

---

## Topological Sort and Cycle Detection

> **Note**: Topological sort applies only to DAGs. For graphs that may contain cycles, first compute SCCs (see section above), contract the condensation to a DAG, then apply topological sort.

### Topological Sort

A **topological ordering** of a DAG is a linear ordering of all vertices such that for every directed edge u → v, vertex u appears **before** v in the result. Think of it as answering: "given these dependencies, in what order can I safely process everything?"

```
DAG (course prerequisites):
  "Math" → "Algorithms" → "Thesis"
  "Math" → "OS"         → "Thesis"

Valid topological orders:
  Math, Algorithms, OS, Thesis
  Math, OS, Algorithms, Thesis

Invalid: Algorithms before Math — violates the prerequisite edge
```

A topological order always exists for a DAG and may not be unique (when there are no ordering constraints between some pairs of nodes). If the graph has a cycle, no topological order is possible — there is no "first" node to start from.

---

### Approach 1: DFS Post-Order

**Core idea**: A node belongs *before* all the nodes it points to. So a node should appear in the result only **after** all its descendants have been fully processed. In DFS this happens naturally at the moment of backtrack — we push the node onto a stack when we return from it, then reverse at the end.

**Why post-order gives the right answer:**
- When we push `u` onto the result, every node reachable from `u` has already been pushed
- Reversing means `u` ends up before all of those nodes
- For every edge u → v: `v` finishes before `u` finishes → `v` is pushed first → after reversing, `u` comes before `v` ✓

```
DAG:  5 → 0, 5 → 2, 4 → 0, 4 → 1, 2 → 3, 3 → 1

DFS from 5:
  Visit 5 → go to 0 → no unvisited neighbors → push 0
          → go to 2 → go to 3 → go to 1 → push 1
                               → push 3
                    → push 2
  push 5

DFS from 4:
  Visit 4 → 0 already visited
          → 1 already visited
  push 4

Stack (bottom → top): 0, 1, 3, 2, 5, 4
After reverse:        4, 5, 2, 3, 1, 0  ← valid topological order
```

#### Recursive DFS

```cpp
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

// Post-order DFS: push u AFTER all its descendants are visited
void dfsPostOrder(const std::vector<std::vector<int>>& adj,
                  int u,
                  std::vector<bool>& visited,
                  std::vector<int>& result) {
    visited[u] = true;

    for (int v : adj[u]) {
        if (!visited[v])
            dfsPostOrder(adj, v, visited, result);
    }

    // All descendants processed — now it is safe to place u in the order
    result.push_back(u);
}

std::vector<int> topoSortDFS(const std::vector<std::vector<int>>& adj) {
    int V = adj.size();
    std::vector<bool> visited(V, false);
    std::vector<int>  result;
    result.reserve(V);

    // Cover all nodes including disconnected components
    for (int u = 0; u < V; ++u) {
        if (!visited[u])
            dfsPostOrder(adj, u, visited, result);
    }

    // Post-order gives reverse topological order — flip it
    std::reverse(result.begin(), result.end());
    return result;
}
```

#### Iterative DFS (same logic, no recursion)

The post-visit step — pushing `u` after all its neighbors — is simulated with the same `{node, isPost}` sentinel pattern used in cycle detection.

```cpp
std::vector<int> topoSortDFSIterative(const std::vector<std::vector<int>>& adj) {
    int V = adj.size();
    std::vector<bool> visited(V, false);
    std::vector<int>  result;
    result.reserve(V);

    for (int start = 0; start < V; ++start) {
        if (visited[start]) continue;

        // {node, isPost}: isPost=true fires after all neighbors are done
        std::stack<std::pair<int, bool>> stk;
        stk.push({start, false});

        while (!stk.empty()) {
            auto [u, isPost] = stk.top();
            stk.pop();

            if (isPost) {
                // All descendants done — safe to record u
                result.push_back(u);
                continue;
            }

            if (visited[u]) continue;
            visited[u] = true;

            // Push post-visit sentinel first (runs last, after all neighbors)
            stk.push({u, true});

            for (int v : adj[u]) {
                if (!visited[v])
                    stk.push({v, false});
            }
        }
    }

    std::reverse(result.begin(), result.end());
    return result;
}
```

---

### Approach 2: Kahn's Algorithm (BFS / In-Degree)

**Core idea**: A node with in-degree 0 has no prerequisites — it can safely go first. Process it, remove its outgoing edges (decrement neighbors' in-degrees), and repeat. Any node whose in-degree drops to 0 becomes available next.

**Why this works:**
- In a DAG there is always at least one node with in-degree 0 (a source)
- Removing a source and its edges produces another DAG
- The order in which sources are dequeued is a valid topological order
- **Cycle detection is free**: if the graph has a cycle, none of the nodes in the cycle will ever reach in-degree 0, so they are never enqueued — the result will contain fewer than V nodes

```
DAG:  5→0, 5→2, 4→0, 4→1, 2→3, 3→1

In-degrees: 0:2, 1:2, 2:1, 3:1, 4:0, 5:0

Queue start (in-degree 0): [4, 5]

Step 1: Dequeue 4 → output [4]
  Decrement 0 (→1), 1 (→1). Neither hits 0. Queue: [5]

Step 2: Dequeue 5 → output [4, 5]
  Decrement 0 (→0), 2 (→0). Both hit 0. Queue: [0, 2]

Step 3: Dequeue 0 → output [4, 5, 0]
  No outgoing edges. Queue: [2]

Step 4: Dequeue 2 → output [4, 5, 0, 2]
  Decrement 3 (→0). Queue: [3]

Step 5: Dequeue 3 → output [4, 5, 0, 2, 3]
  Decrement 1 (→0). Queue: [1]

Step 6: Dequeue 1 → output [4, 5, 0, 2, 3, 1]
  All V=6 nodes processed → valid topological order ✓
```

```cpp
#include <iostream>
#include <vector>
#include <queue>

std::vector<int> topoSortKahn(const std::vector<std::vector<int>>& adj) {
    int V = adj.size();

    // Count incoming edges for each node
    std::vector<int> indegree(V, 0);
    for (int u = 0; u < V; ++u)
        for (int v : adj[u])
            indegree[v]++;

    // Start with all nodes that have no prerequisites
    std::queue<int> q;
    for (int u = 0; u < V; ++u)
        if (indegree[u] == 0) q.push(u);

    std::vector<int> result;
    result.reserve(V);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.push_back(u);

        // "Remove" u from the graph by decrementing its neighbors' in-degrees
        for (int v : adj[u]) {
            indegree[v]--;
            if (indegree[v] == 0)
                q.push(v);   // v's last prerequisite is done — it's available now
        }
    }

    // If not all nodes were processed, a cycle exists
    if (static_cast<int>(result.size()) != V) {
        std::cout << "Cycle detected — no topological order exists\n";
        return {};
    }

    return result;
}
```

---

### DFS vs Kahn's: When to Use Which

| | DFS Post-Order | Kahn's (BFS) |
|---|---|---|
| Approach | Recursive / iterative DFS | BFS on in-degrees |
| Cycle detection | Needs separate `recStack[]` | **Free** — result size < V means cycle |
| Output | Reverse of finish order | Naturally ordered |
| Multiple valid orders | One per DFS traversal order | Can vary by queue priority |
| Preferred when | You need finish times (SCC, etc.) | You want cycle detection built in |
| Space | O(V) stack | O(V) queue + in-degree array |

Both run in **O(V + E)**.

---

### Complete Driver

```cpp
int main() {
    // DAG: 5→0, 5→2, 4→0, 4→1, 2→3, 3→1
    std::vector<std::vector<int>> dag = {
        {},       // 0: no outgoing edges
        {},       // 1: no outgoing edges
        {3},      // 2 → 3
        {1},      // 3 → 1
        {0, 1},   // 4 → 0, 4 → 1
        {0, 2}    // 5 → 0, 5 → 2
    };

    std::cout << "=== DFS Post-Order (recursive) ===\n";
    auto r1 = topoSortDFS(dag);
    for (int v : r1) std::cout << v << " ";

    std::cout << "\n\n=== DFS Post-Order (iterative) ===\n";
    auto r2 = topoSortDFSIterative(dag);
    for (int v : r2) std::cout << v << " ";

    std::cout << "\n\n=== Kahn's Algorithm (BFS) ===\n";
    auto r3 = topoSortKahn(dag);
    for (int v : r3) std::cout << v << " ";

    // Graph WITH a cycle — Kahn's should report it
    std::cout << "\n\n=== Cyclic graph (Kahn's detects cycle) ===\n";
    std::vector<std::vector<int>> cyclic = {
        {1},   // 0 → 1
        {2},   // 1 → 2
        {0}    // 2 → 0 (cycle)
    };
    topoSortKahn(cyclic);  // Prints: "Cycle detected — no topological order exists"

    return 0;
}
```

**Output:**
```
=== DFS Post-Order (recursive) ===
4 5 2 3 1 0

=== DFS Post-Order (iterative) ===
4 5 2 3 1 0

=== Kahn's Algorithm (BFS) ===
4 5 0 2 3 1

=== Cyclic graph (Kahn's detects cycle) ===
Cycle detected — no topological order exists
```

> Both orderings are valid — DFS and Kahn's may produce different valid orders depending on traversal sequence. What matters is the constraint: for every edge u→v, u always comes before v.

### Cycle Detection

#### Why checking only `visited[]` is wrong for directed graphs

This is the most common mistake. Consider this graph — two paths converging at node 3, but **no cycle**:

```
  0 → 1 → 3
  0 → 2 → 3

DFS from 0:
  Visit 0 → visit 1 → visit 3 (mark visited) → backtrack
  Continue from 0 → visit 2 → try neighbor 3 → already visited!
  ❌ Wrongly reports a cycle
```

Node 3 is reached twice via separate paths. That is not a cycle — nothing points back to an ancestor. `visited[]` only answers "have we ever been here", not "are we currently on a path through here".

**What actually forms a cycle is a back edge** — an edge pointing to a node that is still active on the current DFS call stack:

```
  0 → 1 → 2 → 0   ← edge 2→0 points back to 0, which is still on the stack
                      THIS is a cycle
```

The fix: track two arrays in parallel.

```
visited[u]  = true  once u is first entered (never reset)
recStack[u] = true  while u is on the active recursion path
            = false once we fully backtrack out of u

A cycle exists when we reach a neighbor v where recStack[v] == true
— meaning v is an ancestor we haven't finished yet.
```

The check order matters too. On entering a node u:
1. If `recStack[u]` is true → we looped back to an active ancestor → **cycle**
2. If `visited[u]` is true (but recStack is false) → already fully processed, safe to skip
3. Otherwise → first visit, mark both and recurse

---

#### Directed Graph — Recursive DFS (clean approach)

```cpp
#include <iostream>
#include <vector>

// Returns true if a cycle is found starting from node u.
// visited[]  — globally marks nodes we have entered at least once
// recStack[] — marks nodes currently on the active DFS call path
bool dfsHasCycle(const std::vector<std::vector<int>>& adj,
                 int u,
                 std::vector<bool>& visited,
                 std::vector<bool>& recStack) {

    // 1. Back edge found: u is an ancestor still on the current path
    if (recStack[u]) return true;

    // 2. Already fully processed in a previous DFS call — safe, skip
    if (visited[u]) return false;

    // 3. First time visiting u: mark it and add to current path
    visited[u]  = true;
    recStack[u] = true;

    for (int v : adj[u]) {
        if (dfsHasCycle(adj, v, visited, recStack))
            return true;
    }

    // Backtracking: u is no longer on the active path
    recStack[u] = false;
    return false;
}

bool hasCycleDirected(const std::vector<std::vector<int>>& adj) {
    int V = adj.size();
    std::vector<bool> visited(V, false);
    std::vector<bool> recStack(V, false);

    // Start DFS from every unvisited node to cover disconnected graphs
    for (int u = 0; u < V; ++u) {
        if (!visited[u] && dfsHasCycle(adj, u, visited, recStack))
            return true;
    }
    return false;
}
```

**Walkthrough — cycle: `0 → 1 → 2 → 0`**

```
dfsHasCycle(0): visited[0]=T, recStack[0]=T
  dfsHasCycle(1): visited[1]=T, recStack[1]=T
    dfsHasCycle(2): visited[2]=T, recStack[2]=T
      neighbor 0: recStack[0]==true → return true ✓ CYCLE
```

**Walkthrough — no cycle: `0→1→3, 0→2→3`**

```
dfsHasCycle(0): visited[0]=T, recStack[0]=T
  dfsHasCycle(1): visited[1]=T, recStack[1]=T
    dfsHasCycle(3): visited[3]=T, recStack[3]=T
      no neighbors → recStack[3]=F, return false
    recStack[1]=F, return false
  dfsHasCycle(2): visited[2]=T, recStack[2]=T
    neighbor 3: visited[3]=T, recStack[3]=F → skip (step 2), return false
    recStack[2]=F, return false
  recStack[0]=F → return false
No cycle found ✓
```

---

#### Directed Graph — Iterative DFS

The iterative version must manually simulate the backtrack step (`recStack[u] = false`). We do this with a **post-visit sentinel**: push `{u, true}` onto the stack before pushing neighbors. When we pop it, all of u's descendants have been processed — that is the exact moment to remove u from `recStack`.

```cpp
#include <iostream>
#include <vector>
#include <stack>

bool hasCycleDirectedIterative(const std::vector<std::vector<int>>& adj) {
    int V = adj.size();
    std::vector<bool> visited(V, false);
    std::vector<bool> recStack(V, false);

    for (int start = 0; start < V; ++start) {
        if (visited[start]) continue;

        // Stack entries: {node, is_post_visit}
        // is_post_visit=false → entering node for the first time
        // is_post_visit=true  → all descendants done, time to backtrack
        std::stack<std::pair<int, bool>> stk;
        stk.push({start, false});

        while (!stk.empty()) {
            auto [u, isPost] = stk.top();
            stk.pop();

            if (isPost) {
                recStack[u] = false;  // Backtrack: remove from active path
                continue;
            }

            if (recStack[u]) return true;  // Back edge → cycle
            if (visited[u])  continue;     // Already fully processed → skip

            visited[u]  = true;
            recStack[u] = true;

            // Push post-visit sentinel FIRST (fires after all neighbors finish)
            stk.push({u, true});

            for (int v : adj[u]) {
                if (recStack[v]) return true;  // Back edge detected early
                if (!visited[v]) stk.push({v, false});
            }
        }
    }

    return false;
}
```

---

#### Undirected Graph Cycle Detection

In an undirected graph every edge `u—v` appears as both `u→v` and `v→u`. Without extra tracking, we'd immediately flag the edge we arrived on as a back edge. The fix: record the **parent** we came from and skip that edge.

Any other visited neighbor is a genuine back edge.

```cpp
#include <iostream>
#include <vector>
#include <stack>

// Recursive helper — tracks parent to avoid re-flagging the arrival edge
bool dfsHasCycleUndirected(const std::vector<std::vector<int>>& adj,
                            int u, int parent,
                            std::vector<bool>& visited) {
    visited[u] = true;

    for (int v : adj[u]) {
        if (!visited[v]) {
            if (dfsHasCycleUndirected(adj, v, u, visited))
                return true;
        } else if (v != parent) {
            // Visited neighbor that is not where we came from → back edge → cycle
            return true;
        }
    }
    return false;
}

bool hasCycleUndirected(const std::vector<std::vector<int>>& adj) {
    int V = adj.size();
    std::vector<bool> visited(V, false);

    for (int u = 0; u < V; ++u) {
        if (!visited[u] && dfsHasCycleUndirected(adj, u, -1, visited))
            return true;
    }
    return false;
}

// Iterative version — stack stores {node, parent}
bool hasCycleUndirectedIterative(const std::vector<std::vector<int>>& adj) {
    int V = adj.size();
    std::vector<bool> visited(V, false);

    for (int start = 0; start < V; ++start) {
        if (visited[start]) continue;

        std::stack<std::pair<int, int>> stk;  // {node, parent}
        stk.push({start, -1});

        while (!stk.empty()) {
            auto [u, parent] = stk.top();
            stk.pop();

            if (visited[u]) continue;
            visited[u] = true;

            for (int v : adj[u]) {
                if (!visited[v]) {
                    stk.push({v, u});
                } else if (v != parent) {
                    return true;  // Back edge → cycle
                }
            }
        }
    }

    return false;
}
```

---

#### Complete Driver

```cpp
int main() {
    // Undirected — no cycle: simple path 0 — 1 — 2
    std::vector<std::vector<int>> ug1 = {
        {1},      // 0
        {0, 2},   // 1
        {1}       // 2
    };

    // Undirected — cycle: triangle 0 — 1 — 2 — 0
    std::vector<std::vector<int>> ug2 = {
        {1, 2},   // 0
        {0, 2},   // 1
        {0, 1}    // 2
    };

    // Directed — no cycle: diamond 0→1→3, 0→2→3
    std::vector<std::vector<int>> dg1 = {
        {1, 2},   // 0
        {3},      // 1
        {3},      // 2
        {}        // 3 (sink)
    };

    // Directed — cycle: 0→1→2→0
    std::vector<std::vector<int>> dg2 = {
        {1},      // 0
        {2},      // 1
        {0}       // 2 → back to 0
    };

    std::cout << "=== Undirected (recursive) ===\n";
    std::cout << "Path 0-1-2:      " << (hasCycleUndirected(ug1) ? "CYCLE" : "NO CYCLE") << "\n";
    std::cout << "Triangle 0-1-2:  " << (hasCycleUndirected(ug2) ? "CYCLE" : "NO CYCLE") << "\n";

    std::cout << "\n=== Undirected (iterative) ===\n";
    std::cout << "Path 0-1-2:      " << (hasCycleUndirectedIterative(ug1) ? "CYCLE" : "NO CYCLE") << "\n";
    std::cout << "Triangle 0-1-2:  " << (hasCycleUndirectedIterative(ug2) ? "CYCLE" : "NO CYCLE") << "\n";

    std::cout << "\n=== Directed (recursive) ===\n";
    std::cout << "Diamond 0→1→3:   " << (hasCycleDirected(dg1) ? "CYCLE" : "NO CYCLE") << "\n";
    std::cout << "Cycle 0→1→2→0:   " << (hasCycleDirected(dg2) ? "CYCLE" : "NO CYCLE") << "\n";

    std::cout << "\n=== Directed (iterative) ===\n";
    std::cout << "Diamond 0→1→3:   " << (hasCycleDirectedIterative(dg1) ? "CYCLE" : "NO CYCLE") << "\n";
    std::cout << "Cycle 0→1→2→0:   " << (hasCycleDirectedIterative(dg2) ? "CYCLE" : "NO CYCLE") << "\n";

    return 0;
}
```

**Output:**
```
=== Undirected (recursive) ===
Path 0-1-2:      NO CYCLE
Triangle 0-1-2:  CYCLE

=== Undirected (iterative) ===
Path 0-1-2:      NO CYCLE
Triangle 0-1-2:  CYCLE

=== Directed (recursive) ===
Diamond 0→1→3:   NO CYCLE
Cycle 0→1→2→0:   CYCLE

=== Directed (iterative) ===
Diamond 0→1→3:   NO CYCLE
Cycle 0→1→2→0:   CYCLE
```

---

## Strongly Connected Components (SCC)

A **Strongly Connected Component** is a maximal subgraph in which every vertex is reachable from every other vertex. SCCs partition a directed graph into its "mutually reachable" clusters.

```
Directed graph:              SCCs:
  0 → 1 → 3                 SCC 1: {0, 1, 2}  (all reach each other)
  ↑   ↓                     SCC 2: {3}         (no way back to 0,1,2)
  2 ← 1                     SCC 3: {4}
  3 → 4
```

SCCs are foundational for: compiler dead-code elimination, detecting circular dependencies, web graph analysis (finding "knots" of mutually linking pages), and decomposing a digraph into a DAG of components.

### Kosaraju's Algorithm (Two-Pass DFS)

**Core idea**: The reverse graph has the same SCCs. A vertex that finishes last in DFS on the original graph is the "root" of an SCC in the reverse graph.

**Steps:**
1. Run DFS on the original graph; push each vertex onto a stack in **finish order**
2. Transpose (reverse) all edges
3. Pop vertices from the stack and run DFS on the transposed graph — each DFS tree is one SCC

```
Original graph:     Transposed graph:
  0 → 1              0 ← 1
  1 → 2              1 ← 2
  2 → 0              2 ← 0
  1 → 3              1 ← 3
  3 → 4              3 ← 4

Step 1 finish order (one possible): 4, 3, 2, 0, 1 → stack top = 1
Step 3: DFS from 1 on transposed → visits 1, 2, 0 → SCC {0,1,2}
        DFS from 3 on transposed → visits 3       → SCC {3}
        DFS from 4 on transposed → visits 4       → SCC {4}
```

```cpp
#include <vector>
#include <stack>

void dfsFinish(const std::vector<std::vector<int>>& adj,
               int u, std::vector<bool>& visited,
               std::stack<int>& finishStack) {
    visited[u] = true;
    for (int v : adj[u])
        if (!visited[v]) dfsFinish(adj, v, visited, finishStack);
    finishStack.push(u);  // Push after all descendants finish
}

void dfsCollect(const std::vector<std::vector<int>>& radj,
                int u, std::vector<bool>& visited,
                std::vector<int>& component) {
    visited[u] = true;
    component.push_back(u);
    for (int v : radj[u])
        if (!visited[v]) dfsCollect(radj, v, visited, component);
}

std::vector<std::vector<int>> kosarajuSCC(
    const std::vector<std::vector<int>>& adj, int V) {

    // Step 1: DFS on original graph, record finish order
    std::vector<bool> visited(V, false);
    std::stack<int> finishStack;
    for (int u = 0; u < V; ++u)
        if (!visited[u]) dfsFinish(adj, u, visited, finishStack);

    // Step 2: Build transposed graph
    std::vector<std::vector<int>> radj(V);
    for (int u = 0; u < V; ++u)
        for (int v : adj[u]) radj[v].push_back(u);

    // Step 3: DFS on transposed graph in reverse finish order
    std::fill(visited.begin(), visited.end(), false);
    std::vector<std::vector<int>> sccs;

    while (!finishStack.empty()) {
        int u = finishStack.top(); finishStack.pop();
        if (!visited[u]) {
            std::vector<int> component;
            dfsCollect(radj, u, visited, component);
            sccs.push_back(component);
        }
    }

    return sccs;  // Each inner vector is one SCC
}
```

**Time complexity**: O(V + E) — two DFS passes plus graph transposition  
**Space**: O(V + E) for the transposed graph

### Tarjan's Algorithm (Single-Pass DFS)

**Core idea**: During DFS, track each vertex's **discovery time** (`disc`) and the lowest discovery time reachable from its subtree (`low`). A vertex `u` is the root of an SCC when `low[u] == disc[u]` — meaning no back edge escapes its subtree to an earlier-discovered ancestor.

Vertices are kept on a stack during DFS. When an SCC root is identified, pop the stack down to (and including) that root — everything popped is one SCC.

```
DFS on:  0 → 1 → 2 → 0   (cycle)
              ↓
              3 → 4

disc/low after full DFS:
  0: disc=0, low=0  ← SCC root (low==disc): pop {0,2,1} → SCC {0,1,2}
  1: disc=1, low=0
  2: disc=2, low=0
  3: disc=3, low=3  ← SCC root: pop {3} → SCC {3}
  4: disc=4, low=4  ← SCC root: pop {4} → SCC {4}
```

```cpp
#include <vector>
#include <stack>
#include <algorithm>

class TarjanSCC {
    const std::vector<std::vector<int>>& adj;
    int V, timer = 0;
    std::vector<int>  disc, low, comp;
    std::vector<bool> onStack;
    std::stack<int>   stk;
    std::vector<std::vector<int>> sccs;

    void dfs(int u) {
        disc[u] = low[u] = timer++;
        stk.push(u);
        onStack[u] = true;

        for (int v : adj[u]) {
            if (disc[v] == -1) {
                // Tree edge: recurse and pull up low value
                dfs(v);
                low[u] = std::min(low[u], low[v]);
            } else if (onStack[v]) {
                // Back edge to ancestor still on stack
                low[u] = std::min(low[u], disc[v]);
            }
            // Cross/forward edges (v not on stack) are ignored
        }

        // u is the root of an SCC when low[u] == disc[u]
        if (low[u] == disc[u]) {
            std::vector<int> component;
            while (true) {
                int w = stk.top(); stk.pop();
                onStack[w] = false;
                component.push_back(w);
                if (w == u) break;
            }
            sccs.push_back(component);
        }
    }

public:
    TarjanSCC(const std::vector<std::vector<int>>& adj, int V)
        : adj(adj), V(V), disc(V, -1), low(V), comp(V, -1), onStack(V, false) {}

    std::vector<std::vector<int>> compute() {
        for (int u = 0; u < V; ++u)
            if (disc[u] == -1) dfs(u);
        return sccs;
    }
};
```

**Time complexity**: O(V + E) — single DFS pass  
**Space**: O(V) stack + O(V) disc/low arrays

### Kosaraju vs Tarjan

| | Kosaraju | Tarjan |
|---|---|---|
| DFS passes | 2 | 1 |
| Extra space | O(V + E) transposed graph | O(V) stack only |
| Implementation | Simpler to reason about | More compact |
| Output order | Reverse topological order of SCCs | Reverse topological order of SCCs |
| Use when | Clarity matters | Memory is constrained |

Both run in O(V + E). The resulting SCC sets are identical; only the ordering within each component may differ.

### Relationship Between SCC and Topological Sort

Once SCCs are computed, contract each SCC to a single node. The resulting **condensation graph** is always a DAG — and can be topologically sorted. This two-step pattern (SCC → condensation → topological sort) is a standard technique for problems on general digraphs that would be easier on DAGs.

```
Original digraph (with cycles):
  0 ⇄ 1 ⇄ 2    3 → 4

After SCC contraction:
  {0,1,2} → {3} → {4}    ← DAG, can be topologically sorted
```

---

## Comparison and Use Cases

### Algorithm Selection Guide

| Problem | Algorithm | Time |
|---|---|---|
| Shortest path, unweighted | BFS | O(V+E) |
| Shortest path, non-negative weights | Dijkstra | O((V+E) log V) |
| Shortest path, negative weights | Bellman-Ford | O(V·E) |
| All-pairs shortest paths | Floyd-Warshall | O(V³) |
| Minimum spanning tree (sparse) | Kruskal | O(E log E) |
| Minimum spanning tree (dense) | Prim | O((V+E) log V) |
| Topological sort / cycle detection | DFS or Kahn | O(V+E) |
| Connected components | BFS/DFS union-find | O(V+E) |
| Strongly connected components | Tarjan / Kosaraju | O(V+E) |

### Real-World Applications

**BFS / DFS:**
- Web crawlers (BFS for breadth-first crawl)
- Social network "degrees of separation"
- Maze solving, game tree search
- Garbage collection (mark-and-sweep)

**Dijkstra:**
- GPS navigation (road networks)
- Network routing protocols (OSPF)
- Game AI pathfinding (A* is Dijkstra with a heuristic)

**Bellman-Ford:**
- BGP internet routing (detects negative routing loops)
- Currency arbitrage detection (negative cycle = arbitrage opportunity)

**MST (Kruskal / Prim):**
- Network cable layout (minimize wire length)
- Cluster analysis in machine learning
- Image segmentation
- Approximation algorithms for NP-hard problems (TSP)

**Topological Sort:**
- Build systems (Make, Bazel, CMake dependency resolution)
- Package managers (apt, npm, cargo)
- Spreadsheet cell evaluation order
- Course prerequisite scheduling

---

## Complete C++ Implementations

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <numeric>
#include <limits>
#include <utility>

using pii = std::pair<int, int>;

// ===================================================================
// GRAPH CLASS — adjacency list, supports directed/undirected, weighted
// ===================================================================
class Graph {
public:
    int V;
    bool directed;
    std::vector<std::vector<pii>> adj;  // adj[u] = { (weight, v), ... }

    Graph(int V, bool directed = false)
        : V(V), directed(directed), adj(V) {}

    void addEdge(int u, int v, int weight = 1) {
        adj[u].push_back({weight, v});
        if (!directed) adj[v].push_back({weight, u});
    }

    // -----------------------------------------------------------------
    // BFS from source; returns distances (-1 = unreachable)
    // -----------------------------------------------------------------
    std::vector<int> bfs(int source) const {
        std::vector<int> dist(V, -1);
        std::queue<int> q;
        dist[source] = 0;
        q.push(source);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto [w, v] : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        return dist;
    }

    // -----------------------------------------------------------------
    // DFS from source; returns visit order
    // -----------------------------------------------------------------
    std::vector<int> dfs(int source) const {
        std::vector<bool> visited(V, false);
        std::vector<int>  order;
        dfsHelper(source, visited, order);
        return order;
    }

    // -----------------------------------------------------------------
    // DIJKSTRA from source (non-negative weights only)
    // -----------------------------------------------------------------
    std::vector<int> dijkstra(int source) const {
        std::vector<int> dist(V, std::numeric_limits<int>::max());
        std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;

            for (auto [weight, v] : adj[u]) {
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }

    // -----------------------------------------------------------------
    // TOPOLOGICAL SORT (Kahn's algorithm; detects cycles)
    // -----------------------------------------------------------------
    std::vector<int> topologicalSort() const {
        std::vector<int> indegree(V, 0);
        for (int u = 0; u < V; ++u)
            for (auto [w, v] : adj[u]) indegree[v]++;

        std::queue<int> q;
        for (int u = 0; u < V; ++u)
            if (indegree[u] == 0) q.push(u);

        std::vector<int> order;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (auto [w, v] : adj[u])
                if (--indegree[v] == 0) q.push(v);
        }

        if (static_cast<int>(order.size()) != V) {
            std::cerr << "Cycle detected — no topological order\n";
            return {};
        }
        return order;
    }

    // -----------------------------------------------------------------
    // CYCLE DETECTION (directed graph)
    // -----------------------------------------------------------------
    bool hasCycle() const {
        std::vector<bool> visited(V, false), inStack(V, false);
        for (int u = 0; u < V; ++u)
            if (!visited[u] && cycleHelper(u, visited, inStack))
                return true;
        return false;
    }

private:
    void dfsHelper(int u, std::vector<bool>& visited,
                   std::vector<int>& order) const {
        visited[u] = true;
        order.push_back(u);
        for (auto [w, v] : adj[u])
            if (!visited[v]) dfsHelper(v, visited, order);
    }

    bool cycleHelper(int u, std::vector<bool>& visited,
                     std::vector<bool>& inStack) const {
        visited[u] = inStack[u] = true;
        for (auto [w, v] : adj[u]) {
            if (!visited[v] && cycleHelper(v, visited, inStack)) return true;
            if (inStack[v]) return true;
        }
        inStack[u] = false;
        return false;
    }
};

// ===================================================================
// KRUSKAL'S MST (edge list input)
// ===================================================================
struct Edge { int u, v, weight; bool operator<(const Edge& o) const { return weight < o.weight; } };

struct UnionFind {
    std::vector<int> parent, rank;
    UnionFind(int n) : parent(n), rank(n, 0) { std::iota(parent.begin(), parent.end(), 0); }
    int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }
    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        if (rank[x] < rank[y]) std::swap(x, y);
        parent[y] = x;
        if (rank[x] == rank[y]) rank[x]++;
        return true;
    }
};

std::vector<Edge> kruskal(int V, std::vector<Edge> edges) {
    std::sort(edges.begin(), edges.end());
    UnionFind uf(V);
    std::vector<Edge> mst;
    for (const auto& e : edges) {
        if (uf.unite(e.u, e.v)) {
            mst.push_back(e);
            if (static_cast<int>(mst.size()) == V - 1) break;
        }
    }
    return mst;
}

// ===================================================================
// DEMO
// ===================================================================
int main() {
    std::cout << "=== BFS / DFS (undirected) ===\n";
    Graph g(6, false);
    g.addEdge(0, 1); g.addEdge(0, 2); g.addEdge(1, 3);
    g.addEdge(1, 4); g.addEdge(2, 5);

    auto bfsDist = g.bfs(0);
    std::cout << "BFS distances from 0: ";
    for (int i = 0; i < 6; ++i) std::cout << i << ":" << bfsDist[i] << " ";

    auto dfsOrder = g.dfs(0);
    std::cout << "\nDFS order from 0: ";
    for (int v : dfsOrder) std::cout << v << " ";

    std::cout << "\n\n=== Dijkstra (weighted directed) ===\n";
    Graph wg(5, true);
    wg.addEdge(0, 1, 4); wg.addEdge(0, 2, 1); wg.addEdge(2, 1, 2);
    wg.addEdge(1, 3, 1); wg.addEdge(2, 3, 5); wg.addEdge(3, 4, 3);

    auto dist = wg.dijkstra(0);
    std::cout << "Shortest distances from 0: ";
    for (int i = 0; i < 5; ++i) std::cout << i << ":" << dist[i] << " ";

    std::cout << "\n\n=== Topological Sort (DAG) ===\n";
    Graph dag(6, true);
    dag.addEdge(5, 2); dag.addEdge(5, 0); dag.addEdge(4, 0);
    dag.addEdge(4, 1); dag.addEdge(2, 3); dag.addEdge(3, 1);

    auto topo = dag.topologicalSort();
    std::cout << "Topological order: ";
    for (int v : topo) std::cout << v << " ";

    std::cout << "\n\n=== Cycle Detection ===\n";
    Graph cyclic(3, true);
    cyclic.addEdge(0, 1); cyclic.addEdge(1, 2); cyclic.addEdge(2, 0);
    std::cout << "Cyclic graph has cycle: " << (cyclic.hasCycle() ? "YES" : "NO");
    std::cout << "\nDAG has cycle: " << (dag.hasCycle() ? "YES" : "NO");

    std::cout << "\n\n=== Kruskal's MST ===\n";
    std::vector<Edge> edges = {
        {0, 1, 4}, {0, 2, 3}, {1, 2, 1}, {1, 3, 2}, {2, 3, 4}, {3, 4, 2}
    };
    auto mst = kruskal(5, edges);
    int totalWeight = 0;
    std::cout << "MST edges:\n";
    for (const auto& e : mst) {
        std::cout << "  " << e.u << " -- " << e.v << " (weight " << e.weight << ")\n";
        totalWeight += e.weight;
    }
    std::cout << "Total MST weight: " << totalWeight << "\n";

    return 0;
}
```

### Expected Output

```
=== BFS / DFS (undirected) ===
BFS distances from 0: 0:0 1:1 2:1 3:2 4:2 5:2
DFS order from 0: 0 1 3 4 2 5

=== Dijkstra (weighted directed) ===
Shortest distances from 0: 0:0 1:3 2:1 3:4 4:7

=== Topological Sort (DAG) ===
Topological order: 4 5 0 2 3 1

=== Cycle Detection ===
Cyclic graph has cycle: YES
DAG has cycle: NO

=== Kruskal's MST ===
MST edges:
  1 -- 2 (weight 1)
  1 -- 3 (weight 2)
  3 -- 4 (weight 2)
  0 -- 2 (weight 3)
Total MST weight: 8
```

---

## Summary

| Algorithm | Time | Space | Key property |
|---|---|---|---|
| BFS | O(V+E) | O(V) | Level-order; shortest hop-count path |
| DFS | O(V+E) | O(V) | Post-order enables topological sort |
| Dijkstra | O((V+E) log V) | O(V) | Greedy; non-negative weights only |
| Bellman-Ford | O(V·E) | O(V) | Negative weights; detects negative cycles |
| Floyd-Warshall | O(V³) | O(V²) | All-pairs; simple to implement |
| Kruskal | O(E log E) | O(V) | Sort + union-find; sparse graphs |
| Prim | O((V+E) log V) | O(V) | Greedy growth; dense graphs |
| Topological Sort | O(V+E) | O(V) | DAGs only |
| Kosaraju SCC | O(V+E) | O(V+E) | Two-pass DFS; simpler code |
| Tarjan SCC | O(V+E) | O(V) | Single-pass DFS; memory-efficient |

### Key Takeaways

1. **Choose your representation first** — adjacency lists are almost always the right default; matrices only make sense for dense graphs or when O(1) edge queries are critical.
2. **BFS for unweighted shortest paths; Dijkstra for weighted** — never use Dijkstra on unweighted graphs (BFS is faster) or graphs with negative weights (use Bellman-Ford).
3. **Topological sort requires a DAG** — always check for cycles first if the graph is not known to be acyclic.
4. **Kruskal and Prim always produce the same MST weight**, but Kruskal is simpler for sparse graphs while Prim is better for dense graphs.
5. **DFS timestamps and coloring** unlock a wide family of algorithms: SCC (Tarjan/Kosaraju), bridge/articulation point detection, and bipartiteness testing.

---

## Further Study

- **Maximum Flow**: Ford-Fulkerson, Edmonds-Karp, Dinic's algorithm
- **Bipartite Matching**: Hopcroft-Karp algorithm
- **A\* Search**: Dijkstra with a heuristic — widely used in game AI and robotics
- **Bridge and Articulation Point Detection**: Extensions of Tarjan's DFS using disc/low values
- **Network Flow Applications**: Bipartite matching, min-cut, project selection
- **Planar Graphs and Graph Coloring**: Theoretical foundations and NP-hardness results
