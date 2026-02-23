#include <iostream>
#include <string_view>
#include <array>
#include <vector>

class PrefixTree {
    struct TreeNode {
        TreeNode() {
            next.fill(nullptr);
            cnt = 0;
            is_word = false;
        }

        TreeNode* get_node(char c) {
            return next[c - 'a'];
        }

        void set_node(char c) {
            next[c - 'a'] = new TreeNode();
        }

        std::array<TreeNode*, 26> next{};
        int cnt{};
        bool is_word{};
    };

public:
    PrefixTree()
    {
        root_ = new TreeNode();
    }

    // N
    // Time : O(N)
    // Memory : O(N)
    void insert(std::string_view word) {
        TreeNode* curr = root_;
        for(int i = 0; i < word.size(); ++i) {
            char c = word[i];
            TreeNode* next = curr->get_node(c);
            if(next == nullptr) {
                curr->set_node(c);
            }
            curr = curr->get_node(c);
        }

        curr->is_word = true;
        ++(curr->cnt); 
    }

    // N
    // Time : O(N)
    // Memory : O(1)
    bool find(std::string_view word) { 
        TreeNode* curr = root_;
        for(int i = 0; i < word.size(); ++i) {
            char c = word[i];
            curr = curr->get_node(c);
            if(curr == nullptr) {
                return false;
            }
        }
        return curr->is_word;
    }

    std::vector<std::string> get_all_words() {
        std::vector<std::string> ans;
        std::string path;
        dfs(root_, path, ans);
        return ans;
    }

    void erase(std::string_view word) {}
    std::vector<std::string> prefix_find(std::string_view prefix) { 
        // TODO 
        return {}; 
    }

private:
    // K
    // O(K) > O(L)
    void dfs(TreeNode* root, std::string& path, std::vector<std::string>& ans) {
        if(root == nullptr){
            return;
        }

        for(int i = 0; i < 26; ++i) {
            auto child = root->next[i];
            if(child){
                char c = static_cast<char>('a' + i);
                path.push_back(c);
                if(child->is_word) {
                    ans.push_back(path);
                }
                dfs(root->next[i], path, ans);
                path.pop_back();
            }
        }
    }


    TreeNode* root_;
};

int main() {
    PrefixTree tree;
    tree.insert("abc");
    tree.insert("aac");
    tree.insert("def");

    std::cout << tree.find("abc") << std::endl;
    std::cout << tree.find("aac") << std::endl;
    std::cout << tree.find("def") << std::endl;
    std::cout << tree.find("gtr") << std::endl;
    std::cout << tree.find("aaaaaa") << std::endl;

    std::cout << "--------------------------" << std::endl;

    auto all = tree.get_all_words();
    std::cout << all.size() << std::endl;
    for(auto& w : all) {
        std::cout << w << ", ";
    }
}
