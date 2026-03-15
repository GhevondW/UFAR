# Project 1: City Route Planner

## Overview
Navigation apps like Google Maps and Waze are tools most people use daily without thinking about what runs underneath them. At their core, they are graph problems. In this project, you will build a simplified but fully functional route planning engine that models a city as a weighted directed graph and finds the most efficient paths between locations.

Your program will read a city map from a file (nodes = locations, edges = roads with distances or travel times), answer shortest-path queries from a user, and display the result as a readable path with total cost. This project gives you hands-on experience with one of the most practically important graph algorithms in computer science.

## Core Features

**Required:**
- Load a city graph from a text or CSV file (at minimum 20 nodes and 40 edges)
- Represent the graph using an adjacency list
- Implement Dijkstra's algorithm from scratch to find the shortest path between any two locations
- Display the result as the ordered list of locations visited and total distance/cost
- Handle edge cases: disconnected graph (no path exists), same source and destination, invalid input
- Provide a written complexity analysis: time and space complexity of your graph representation and Dijkstra's implementation

**Bonus:**
- Implement Bellman-Ford as a second engine and let the user choose which algorithm to use
- Compare execution times of both algorithms on the same dataset and present a performance graph
- Support a "top 3 alternative routes" feature using k-shortest paths

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 2: Autocomplete Search Engine

## Overview
Every time you type into a search bar and suggestions appear instantly, a Trie is likely involved. This project challenges you to build an autocomplete engine from scratch using a Trie data structure. Given a large dataset of words or phrases (for example, city names, product catalog entries, or movie titles), your system will return the most relevant completions for any prefix a user types.

This is a project about understanding how prefix-based retrieval works and why a Trie dramatically outperforms scanning a list when completions are needed at interactive speed.

## Core Features

**Required:**
- Implement a Trie from scratch with insert, search, and delete operations
- Each Trie node must track the frequency of how many times a word was inserted or queried
- Given a user-typed prefix, return the top-5 most frequent completions
- Load a real dataset of at least 1,000 words/phrases from a file
- Handle edge cases: prefix not found, single-character prefix returning hundreds of results, empty input
- Provide complexity analysis for insert, search, and prefix-query operations

**Bonus:**
- Support fuzzy prefix matching: return results even if the prefix contains one typo (edit distance ≤ 1)
- Implement a "recently searched" feature using a small hash map that boosts the rank of recently queried terms
- Benchmark Trie prefix search vs. linear scan on the same dataset and plot results

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 3: File System Simulator

## Overview
Every operating system manages files and directories using a tree structure internally. In this project, you will simulate a simplified Unix-like file system in memory using a general n-ary tree, where each node represents either a file or a directory. Users interact with your simulator through terminal commands, just like a real shell.

This project is deceptively rich: it combines tree design, recursive algorithms, and command parsing into a coherent, usable tool that mirrors real operating system internals.

## Core Features

**Required:**
- Implement a general tree where each node stores: name, type (file or directory), size, and creation timestamp
- Support the following commands: `mkdir`, `touch`, `ls`, `cd`, `pwd`, `rm`, `find <name>`, `tree` (display full hierarchy)
- `find` must use DFS to locate a file or directory anywhere in the tree
- `ls` must list immediate children; `tree` must recursively display the full subtree
- Calculate and display total size of a directory recursively
- Handle edge cases: removing a non-empty directory, navigating above root, creating duplicate names
- Provide complexity analysis for each supported operation

**Bonus:**
- Support `mv` (move) and `cp` (copy) operations
- Add a simple permission system (read/write flags per node)
- Implement `du` (disk usage) showing size breakdown per subdirectory

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 4: Social Network Friend Recommender

## Overview
Platforms like LinkedIn and Facebook suggest people you may know based on mutual connections. This feature, simple as it sounds, is a graph problem. In this project you will model a social network as an undirected graph and implement a friend recommendation engine based on mutual friend counts.

You will load a real or synthetic social network dataset, identify who each user is not yet connected to, and rank potential recommendations by the number of shared friends — the most common and intuitive recommendation signal.

## Core Features

**Required:**
- Represent the social network as an undirected graph using adjacency lists
- Load a dataset of at least 50 users and 150 connections from a file (CSV or plain text)
- Implement BFS from a given user node to explore friends and friends-of-friends
- For each non-connected user reachable within 2 hops, compute the number of mutual friends
- Return and display top-5 recommendations ranked by mutual friend count
- Detect and report connected components (isolated users or groups)
- Handle edge cases: users with no friends, fully connected graph, user not found
- Provide complexity analysis for BFS and the mutual-friend counting process

**Bonus:**
- Implement Jaccard similarity score as an alternative ranking method and compare it to mutual friend count
- Detect communities using a simple clustering approach (e.g., label propagation or union-find)

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 5: Inventory and Warehouse Management System

## Overview
Retail and logistics companies manage thousands of products and need to perform fast lookups, insertions, deletions, and range queries on their inventory every second. A simple sorted list becomes too slow at scale. This project asks you to build a warehouse inventory engine backed by an AVL Tree — a self-balancing binary search tree — and demonstrate why balance matters for performance.

You will implement the AVL tree from scratch, including all rotations, and use it to answer realistic inventory queries efficiently.

## Core Features

**Required:**
- Implement an AVL Tree from scratch with insert, delete, and search by product ID
- Each node stores: product ID (integer key), product name, category, quantity, price
- Support range query: return all products with ID between X and Y in sorted order (in-order traversal)
- Support in-order listing of the full inventory (sorted by ID)
- Load an initial inventory from a file and support interactive insert/delete/search/range-query commands
- After each insertion or deletion, verify that the tree remains balanced (track and display height)
- Handle edge cases: deleting a node with two children, duplicate IDs, empty tree queries
- Provide formal complexity analysis for all operations and explain how rotations maintain O(log n) height
- Empirically compare your AVL tree search and range query performance against a sorted array on datasets of size 1,000 and 10,000

**Bonus:**
- Add a secondary hash table for O(1) lookup by product name alongside the AVL tree by product ID
- Support a "low stock alert" feature that finds all products with quantity below a threshold using a range query

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 6: Plagiarism Detector

## Overview
Plagiarism detection systems used by universities and publishers work by fingerprinting documents and comparing those fingerprints at scale. In this project you will build a simplified plagiarism detector that uses hashing to identify shared or near-duplicate content between text documents. You will implement your own hash table from scratch and use it as the core of your detection engine.

This project sits at the intersection of string processing, hashing theory, and practical software engineering.

## Core Features

**Required:**
- Implement a hash table from scratch supporting insert, search, and delete, with your choice of collision resolution (chaining or open addressing — you must justify your choice)
- Extract n-grams (sequences of n consecutive words) from each document and store them as hash keys
- Given two or more documents, compute a similarity score as the percentage of shared n-grams
- Support configurable n (e.g., n=3 for trigrams, n=5 for 5-grams) so the user can tune sensitivity
- Load documents from text files; support at least pairwise comparison of 3+ documents
- Report: which document pairs exceed a similarity threshold (e.g., 40%), and which n-grams are shared
- Handle edge cases: very short documents, identical documents, documents with no overlap
- Implement dynamic resizing of the hash table when the load factor exceeds 0.7
- Provide complexity analysis for building the fingerprint table and querying similarity

**Bonus:**
- Implement a rolling hash (Rabin-Karp) for more efficient n-gram extraction
- Visualize the similarity between all document pairs as a similarity matrix printed to the terminal

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 7: Web Crawler and Link Graph Analyzer

## Overview
Search engines like Google begin their work with a crawler that follows links from page to page, building a massive directed graph of the web. In this project you will simulate this process on a small, local "web" defined by text files, then run graph analysis algorithms on the resulting link graph to extract structural insights.

This project is one of the most algorithmically rich in the list, requiring you to implement BFS, DFS, strongly connected component detection, and topological sort.

## Core Features

**Required:**
- Define a small web of at least 15 "pages" as text files, each containing a list of outgoing links to other pages
- Build a directed graph (adjacency list) by crawling the pages starting from a given root using BFS
- Implement BFS for crawl order and record the order in which pages are discovered
- Implement Kosaraju's or Tarjan's algorithm to find all Strongly Connected Components (SCCs)
- Compute in-degree for each node and identify "hub" pages (highest in-degree)
- Implement topological sort on the DAG formed by condensing SCCs
- Handle edge cases: dangling pages (no outgoing links), self-links, cycles
- Provide complexity analysis for BFS, SCC detection, and topological sort

**Bonus:**
- Implement a simplified PageRank (5–10 iterations) and rank pages by score
- Visualize the SCC structure by printing which pages belong to each component

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 8: Flight Connection Optimizer

## Overview
Every airline booking engine needs to answer questions like: what is the cheapest way to get from city A to city B, possibly with layovers? What airports are reachable from a given hub? Which routes are structurally most critical? This project asks you to build a flight network analyzer using real-world-inspired data and apply multiple graph algorithms to answer these questions.

## Core Features

**Required:**
- Load a flight dataset from a CSV file: at minimum 30 airports and 80 routes with cost and duration as edge weights
- Represent the network as a weighted directed graph using adjacency lists
- Implement Dijkstra's algorithm to find the cheapest and the fastest route between any two airports (two separate queries with different edge weights)
- Implement BFS to find all airports reachable from a given departure airport within at most K connections
- Detect airports that, if removed, would disconnect the graph (articulation points concept — can be simplified)
- Handle edge cases: no route exists, source equals destination, airport not in dataset
- Provide complexity analysis for all graph operations

**Bonus:**
- Implement Prim's or Kruskal's algorithm to compute the Minimum Spanning Tree of the network, visualized as a list of essential routes
- Add a "travel budget" mode: find all destinations reachable from an airport within a given cost budget using a modified BFS/Dijkstra

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 9: Banking Transaction Ledger

## Overview
Financial systems must maintain an ordered, searchable record of transactions and answer range queries efficiently — for example, retrieving all transactions in a given time window for a bank statement. In this project you will build a transaction ledger backed by an AVL tree (ordered by timestamp) and demonstrate through empirical benchmarking why a balanced BST is superior to a sorted linked list for this workload.

## Core Features

**Required:**
- Implement an AVL tree ordered by transaction timestamp (integer or string date key)
- Each transaction node stores: transaction ID, timestamp, amount, type (credit/debit), account ID
- Support insert, delete by ID, search by timestamp, and range query (all transactions between time T1 and T2)
- Load historical transaction data from a file (at least 500 records)
- Support an interactive query mode where the user enters time ranges and sees matching transactions
- Detect and flag duplicate transaction IDs using a hash set
- Handle edge cases: empty ledger, range with no results, deleting a non-existent transaction
- Benchmark range query performance vs. a sorted linked list on datasets of 500, 5,000, and 50,000 records; plot results
- Provide complexity analysis for all operations

**Bonus:**
- Add a fraud detection module that flags any account with more than N transactions within a sliding time window
- Support export of a time-range query result to a CSV file

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 10: Contact Book with Multi-Field Search

## Overview
A contact book sounds simple, but building one that supports fast search across multiple different fields simultaneously — name, phone number, and birthday — requires combining multiple data structures, each optimized for a different type of query. In this project you will implement three data structures that all index the same set of contacts and keep them synchronized through every insert and delete.

This project tests your ability to design a coherent system with multiple interacting components rather than a single isolated structure.

## Core Features

**Required:**
- Each contact stores: full name, phone number, email, birthday (date), notes
- Implement a Trie for fast prefix-based search by name
- Implement a hash table for O(1) exact lookup by phone number
- Implement a BST (AVL or standard) sorted by birthday for range queries (e.g., "all contacts with birthdays in March")
- All three structures must stay synchronized: inserting a contact adds it to all three; deleting removes it from all three
- Support interactive commands: `add`, `delete`, `search-name <prefix>`, `search-phone <number>`, `birthdays-between <date1> <date2>`, `list-all`
- Load initial contacts from a file; support saving the current state back to a file
- Handle edge cases: duplicate phone numbers, contacts with missing fields, search with no results
- Provide complexity analysis justifying why each data structure was chosen for its respective query type

**Bonus:**
- Export contacts sorted by name (using Trie in-order traversal or AVL in-order) to a CSV file
- Add an undo feature using a stack that tracks the last 5 operations

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 11: Maze Generator and Solver

## Overview
Mazes are graphs in disguise. Every cell is a node; every open passage between adjacent cells is an edge. This insight means the entire toolkit of graph algorithms applies directly to mazes. In this project you will both generate random mazes using graph-based algorithms and then solve them using BFS and DFS, comparing the two approaches visually and quantitatively.

## Core Features

**Required:**
- Represent the maze as a 2D grid graph where each cell can have walls on any of its four sides
- Implement randomized DFS (recursive backtracker) to generate a random, fully connected maze of user-specified dimensions (at least 10×10)
- Display the maze in the terminal using ASCII art (walls as `#`, passages as spaces, start as `S`, end as `E`)
- Implement BFS to find the shortest path from start to end; display the solution path overlaid on the maze
- Implement DFS to find a (not necessarily shortest) path from start to end; display its solution path
- Compare and report: path length found by BFS vs. DFS, and total nodes visited by each algorithm
- Handle edge cases: start equals end, maze with no solution (not possible if generated correctly, but handle for manually defined mazes), very small mazes (2×2)
- Provide complexity analysis for maze generation and both solving algorithms in terms of grid size (n×m)

**Bonus:**
- Implement A* search using Manhattan distance as the heuristic and compare its performance against BFS
- Support user-defined mazes loaded from a text file (using a simple character grid format)

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 12: LRU Cache Implementation

## Overview
An LRU (Least Recently Used) cache is one of the most commonly asked-about data structures in technical interviews — and for good reason. It appears in CPU memory management, database buffer pools, web browsers, and CDN servers. The challenge is implementing a cache that performs both get and put operations in O(1) time, which requires a clever combination of two data structures working together.

In this project you will implement an LRU cache from scratch and simulate it on a realistic access-pattern workload.

## Core Features

**Required:**
- Implement the LRU cache combining a doubly linked list (for O(1) eviction of the least recently used item) and a hash map (for O(1) key-to-node lookup) — both implemented from scratch, no standard library queue or map
- Support operations: `get(key)` returns the value or -1 if not present and marks the item as most recently used; `put(key, value)` inserts or updates and evicts the LRU item if at capacity
- Accept configurable cache capacity from the user
- Simulate the cache on a workload file: a sequence of get/put operations with keys and values
- After the simulation, report: total gets, cache hits, cache misses, hit rate percentage, and final cache contents
- Verify correctness: run a test sequence by hand and confirm your implementation matches expected output
- Handle edge cases: capacity of 1, all gets on an empty cache, repeated puts with the same key
- Provide complexity analysis proving that both get and put are O(1)

**Bonus:**
- Implement an LFU (Least Frequently Used) cache as a second mode and compare eviction decisions between LRU and LFU on the same workload
- Plot hit rate vs. cache size across several cache capacities on the same workload to visualize the cache size / hit rate tradeoff

Here are 10 more project ideas, each designed to be built with either a visual UI or CLI — your choice:

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 13: Personal Finance Tracker

## Overview
Budgeting apps need to do more than store transactions — they must support fast queries like "how much did I spend on food last month?" or "show all transactions between these two dates over $50." This project asks you to build a personal finance engine that indexes the same transaction data in three different ways simultaneously, teaching you when and why to reach for different data structures.

## Core Features

**Required:**
- Each transaction stores: ID, date, amount, category (food/rent/transport/etc.), merchant name, note
- Implement a hash map indexed by category for O(1) category-total lookup and per-category listing
- Implement an AVL tree indexed by date for efficient date-range queries
- Implement a max-heap for surfacing the top-N largest transactions on demand
- All three structures must stay synchronized on every insert and delete
- Support commands / UI actions: `add-transaction`, `delete <id>`, `range <date1> <date2>`, `category-summary`, `top-spending <n>`, `monthly-report`
- Load historical data from a CSV file (at least 300 records); support export of filtered results back to CSV
- Handle edge cases: transactions with the same date, deleting a transaction that is currently in the heap's top-N, category with zero transactions
- Provide complexity analysis justifying the choice of each structure for each query type

**Bonus:**
- Add a budget-limit feature: set a monthly cap per category and display a warning when usage exceeds 80%
- Build a visual monthly breakdown — either a bar chart in the terminal or a rendered chart in a graphical UI

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 14: Multiplayer Word Game Engine

## Overview
Games like Scrabble and Boggle need to answer one question millions of times per second: is this string a valid word? And for hint systems: what valid words can be formed from these letters? Both problems are beautifully solved by a Trie. In this project you will build the backend engine for a word game, using a Trie as the dictionary core and a graph search over the letter board for word discovery.

## Core Features

**Required:**
- Implement a Trie from scratch supporting insert, exact search, prefix check, and delete
- Load a dictionary of at least 50,000 words from a file into the Trie
- Represent the game board as an n×m grid of letter tiles (user-configurable size, at least 4×4)
- Implement DFS over the board grid (adjacency = 8 neighbors) combined with Trie prefix pruning to find all valid words reachable from a starting cell without reusing tiles
- Score each found word by length (standard Scrabble-style scoring)
- Support commands / UI actions: `new-game`, `validate <word>`, `hint` (show top-5 scoring words available on current board), `place-word <word> <start>`, `leaderboard`
- Handle edge cases: words shorter than 3 letters, same word reachable via multiple paths (count only once), board with no valid words
- Provide complexity analysis for DFS + Trie pruning vs. DFS without pruning; show empirically how pruning reduces nodes explored

**Bonus:**
- Build an interactive board UI (graphical or terminal with cursor navigation) where players click or type to trace words on the board
- Add an AI opponent that uses the hint engine to play optimally and compare its score to the human player

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 15: Smart Parking Lot Manager

## Overview
Modern parking systems need to allocate spaces efficiently, track occupancy in real time, and direct drivers to the nearest available spot as quickly as possible. This project models a multi-level parking structure as a graph and uses a combination of spatial data structures and shortest-path algorithms to simulate an intelligent parking management system.

## Core Features

**Required:**
- Model the parking lot as a weighted directed graph: nodes are parking zones or intersections, edges are driving lanes with traversal times
- Each parking space stores: space ID, level, zone, type (standard/handicapped/EV), status (free/occupied), vehicle plate if occupied
- Implement Dijkstra's algorithm to find the shortest driving path from the entrance to the nearest available space matching a given type
- Implement a hash map for O(1) vehicle lookup by license plate (for exit and payment)
- Simulate an arrival/departure event stream from a file (at least 200 events)
- Support commands / UI actions: `park <plate> <type>`, `exit <plate>`, `status` (occupancy by level and type), `find-space <type>`, `revenue-report`
- Handle edge cases: lot full, vehicle not found on exit, EV spaces requested when none available, re-entry of the same plate
- Provide complexity analysis for Dijkstra on the parking graph and hash map lookup

**Bonus:**
- Build a visual floor-plan display (ASCII grid or graphical) showing free/occupied spaces updating in real time as events are processed
- Add a reservation system: allow pre-booking a space for a future time window, blocking it from walk-in allocation

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 16: Network Packet Router Simulator

## Overview
Routers in computer networks forward packets by consulting a routing table — essentially a lookup structure that maps destination IP prefixes to outgoing interfaces. Modern routers use a Patricia trie (compressed prefix trie) to handle millions of prefix lookups at line rate. In this project you will simulate a simplified IP packet router and demonstrate why a trie-based routing table dramatically outperforms a linear scan.

## Core Features

**Required:**
- Implement a binary trie (one level per bit of a 32-bit IP address) supporting insert route, delete route, and longest-prefix match lookup
- Each routing table entry stores: IP prefix, prefix length, next-hop interface, metric (cost)
- Simulate packet forwarding: read a stream of packets (source IP, destination IP, payload size) from a file and route each one using longest-prefix match
- Load an initial routing table from a file (at least 50 routes); support dynamic route updates during simulation
- Report: packets routed per interface, unroutable packets (no matching prefix), average lookup time
- Support commands / UI actions: `add-route`, `delete-route`, `lookup <ip>`, `routing-table`, `packet-log`, `stats`
- Handle edge cases: overlapping prefixes (longest match wins), default route (0.0.0.0/0), packet with no matching route
- Provide complexity analysis: O(32) worst-case lookup regardless of table size, vs. O(n) linear scan

**Bonus:**
- Implement route aggregation (CIDR supernetting): combine two adjacent /n routes into a single /(n-1) route when possible
- Benchmark trie lookup vs. linear scan on routing tables of 50, 500, and 5,000 entries and plot the results

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 17: Epidemic Spread Simulator

## Overview
During an outbreak, epidemiologists model disease spread through a contact network — a graph where nodes are people and edges are contacts. This project asks you to simulate an SIR (Susceptible → Infected → Recovered) epidemic model on a contact graph, using graph traversal to propagate infection and graph analysis to identify the most influential spreaders and best intervention points.

## Core Features

**Required:**
- Load or generate a contact network of at least 100 people and 400 contact edges (undirected, with contact frequency as weight)
- Represent the network as a weighted adjacency list
- Implement BFS-based epidemic spread: at each time step, each infected node infects uninfected neighbors with a configurable probability proportional to contact weight
- Implement the full SIR model: susceptible → infected (after contact) → recovered (after configurable days) → immune
- Identify the top-5 super-spreaders: nodes whose removal (simulate node deletion) reduces total infections most
- Compute and report at each time step: number of S, I, R individuals; new infections; R0 estimate
- Support commands / UI actions: `run-simulation <steps>`, `infect <node_id>`, `quarantine <node_id>`, `stats`, `reset`
- Handle edge cases: fully recovered population stopping spread, isolated nodes never getting infected, network with multiple components
- Provide complexity analysis for BFS-based spread over T time steps on a graph of V nodes and E edges

**Bonus:**
- Animate the epidemic curve (S/I/R counts over time) as a real-time updating chart in a graphical UI or terminal sparkline
- Implement a vaccination strategy: given a budget of K vaccinations (node removals), compare random vs. highest-degree targeting in reducing total infections

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 18: Recipe and Ingredient Knowledge Graph

## Overview
Cooking apps like Yummly model recipes, ingredients, and dietary tags as a knowledge graph where relationships between entities enable powerful queries: "what can I cook with what's in my fridge?" or "find all vegan recipes under 30 minutes that share at least 3 ingredients with this dish." In this project you will build that knowledge graph and implement graph traversal algorithms to answer real culinary queries.

## Core Features

**Required:**
- Model the knowledge graph as a heterogeneous directed graph: three node types (Recipe, Ingredient, Tag) with typed edges (Recipe→Ingredient: "contains", Recipe→Tag: "has-tag", Ingredient→Ingredient: "substitutes-for")
- Load a dataset of at least 60 recipes and 80 ingredients from a structured file (CSV or JSON-like format)
- Implement BFS from a set of available ingredients to find all fully or partially cookable recipes (missing ≤ 1 ingredient)
- Implement a similarity score between two recipes: count shared ingredients / total unique ingredients (Jaccard)
- Support commands / UI actions: `what-can-i-cook <ingredient-list>`, `similar-to <recipe>`, `substitutes-for <ingredient>`, `filter <tag>`, `add-recipe`, `ingredient-graph`
- Build a hash map index of ingredients for O(1) lookup when checking recipe feasibility
- Handle edge cases: ingredient not in any recipe, recipe with no tags, substitution chains (A substitutes B substitutes C)
- Provide complexity analysis for BFS over the heterogeneous graph and the Jaccard similarity computation

**Bonus:**
- Build a visual graph display (even a simple ASCII adjacency diagram) showing recipe-ingredient relationships
- Implement a "meal planner" feature: given a list of available ingredients and a target number of meals, select a set of recipes that maximizes ingredient reuse with minimum waste

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

---

# Project 19: Smart Home Automation Rule Engine

## Overview
Smart home platforms like Home Assistant let users define automation rules: "if motion is detected AND it's after 9pm, turn on the lights." Evaluating thousands of such rules efficiently when sensor events arrive is a rule-matching problem that benefits from indexing conditions in a trie and organizing rule dependencies in a graph. In this project you will build that rule engine from scratch.

## Core Features

**Required:**
- Each automation rule stores: rule ID, name, list of conditions (sensor ID + operator + threshold), list of actions (device ID + command), enabled/disabled flag
- Implement a trie indexed by sensor ID for fast retrieval of all rules that reference a given sensor when an event fires
- Represent rule chaining (one rule's action can trigger another rule's condition) as a directed graph; detect and prevent infinite loops using cycle detection
- Implement an event queue (ring buffer or linked list) for incoming sensor events; process events in order
- Support commands / UI actions: `add-rule`, `delete-rule`, `enable/disable <id>`, `trigger-event <sensor> <value>`, `rule-list`, `execution-log`, `dependency-graph`
- Simulate a stream of 200 sensor events from a file and log which rules fired and which actions executed
- Handle edge cases: rule with unsatisfied conditions, chained rule cycle (A triggers B triggers A), event for an unknown sensor
- Provide complexity analysis for rule lookup via trie vs. linear scan over all rules

**Bonus:**
- Build an interactive UI (graphical or terminal) where users visually add/edit rules and watch the execution log update in real time as events stream in
- Add conflict detection: identify pairs of rules that could simultaneously set the same device to contradictory states

## Demo Application

The demo application for this project can be implemented either as a command-line (CLI) tool or as a simple graphical interface. The choice of interface is up to you

