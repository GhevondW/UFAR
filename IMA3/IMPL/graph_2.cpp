#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct TreeNode {
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

struct Edge {
    int a;
    int b;
};

using SimpleGraph = std::vector<Edge>;

using Graph = std::unordered_map<int, std::unordered_set<int>>;

using VisitedSet = std::unordered_set<int>;

Graph CreateGraph(const SimpleGraph& graph) {
    Graph ans;
    for(const Edge& edge : graph) {
        ans[edge.a].insert(edge.b);
        ans[edge.b].insert(edge.a);
    }
    return ans;
}

// (N, E)
// Time - O(N * E)
// Memory - O(N)
VisitedSet visited;
void dfs(int node, Graph& graph) {
    if(visited.find(node) != visited.end()) {
        return;
    }

    std::cout << node << std::endl;
    visited.insert(node);

    const std::unordered_set<int>& next = graph[node];
    for(int n : next) {
        dfs(n, graph);
    }
}

void dfs_iterative(int node, Graph& graph) {
    // TODO
}

int bfs(int begin, int end, Graph& graph) {
    std::queue<int> q;
    int level = 0;
    q.push(begin);
    while (!q.empty())
    {
        int cnt = q.size();
        while (cnt > 0)
        {
            int curr = q.front();
            q.pop();

            if(curr == end){
                return level;
            }

            if(visited.find(curr) != visited.end()) {
                continue;
            }

            std::cout << curr << std::endl;
            visited.insert(curr);

            const std::unordered_set<int>& next = graph[curr];
            for(int n : next) {
                q.push(n);
            }
            --cnt;
        }
        ++level;
    }
    return -1;
}

int main() {
    SimpleGraph edges {
        Edge{1, 2},
        Edge{1, 3},
        Edge{2, 3},
        Edge{3, 4},
        Edge{3, 5},
        // Edge{3, 7},
        Edge{4, 5},
        Edge{6, 7}
    };

    auto graph = CreateGraph(edges);
    dfs(1, graph);

    std::cout << "-----------------------" << std::endl;

    // visited.clear();
    // bfs(1, graph);
    return 0;
}
