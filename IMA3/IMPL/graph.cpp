#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct Edge {
    int a{-1};
    int b{-1};
};

struct TreeNode
{
    int data;
    TreeNode* left;
    TreeNode* right;
};


void dfs(TreeNode* root) {
    if(root == nullptr) {
        return;
    }

    // ...
    dfs(root->left);
    dfs(root->right);
}

template<typename T>
using Set = std::unordered_set<T>;

// Time - O(N + E)
// Memory - O(N)
void dfs_graph(int node, std::unordered_map<int, Set<int>>& graph, Set<int>& visited) {
    if(visited.find(node) != visited.end()) {
        return;
    }

    visited.insert(node);

    const auto& next = graph[node];
    
    std::cout << node << std::endl;

    for(int n : next) {
        dfs_graph(n, graph, visited);
    }
}

// O(N + E)
// O(N + E)
void dfs(int start_node, const std::vector<Edge>& edges) {
    std::unordered_map<int, std::unordered_set<int>> graph;
    
    // Time - O(E)
    // Memory - O(N + E)
    for(const Edge& edge : edges) {
        graph[edge.a].insert(edge.b);
        graph[edge.b].insert(edge.a);
    }

    // Time - O(N + E)
    // Memory - O(N)
    Set<int> visited;
    dfs_graph(start_node, graph, visited);
}

void bfs(int start_node, const std::vector<Edge>& edges) {
    std::unordered_map<int, std::unordered_set<int>> graph;
    
    // Time - O(E)
    // Memory - O(N + E)
    for(const Edge& edge : edges) {
        graph[edge.a].insert(edge.b);
        graph[edge.b].insert(edge.a);
    }

    Set<int> visited;

    std::queue<int> q;
    q.push(start_node);

    while (!q.empty())
    {
        int curr = q.front();
        q.pop();

        if(visited.find(curr) != visited.end()){
            continue;
        }

        std::cout << curr << std::endl;
        visited.insert(curr);

        const auto& next = graph[curr];
        for(int n : next) {
            q.push(n);
        }
    }
}

int main() {
    std::vector<Edge> edges {
        Edge{1, 2},
        Edge{1, 5},
        Edge{1, 4},
        Edge{2, 3},
        Edge{3, 4},
        Edge{3, 6},
        Edge{4, 5}
    };

    dfs(1, edges); // TODO : iterative

    std::cout << "-----------------" << std::endl;

    bfs(1, edges);

    return 0;
}
