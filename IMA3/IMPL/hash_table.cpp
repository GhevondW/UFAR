#include <iostream>
#include <unordered_set>
#include <vector>

// int
class HashTable {
private:
    struct ListNode {
        int data;
        ListNode* next;
    };

    int hash(int data, int K) {
        return data % K;
    }

    double get_balance_factor() {
        int K = table_.size();
        return static_cast<double>(size_) / K;
    }

    // Time - O(K + N)
    // Memory - O(K + N)
    void resize() {
        std::vector<ListNode*> new_table(table_.size() * 2, nullptr);
        for (ListNode* head : table_)
        {
            while (head)
            {
                insert(head->data, new_table);
                head = head->next;
            }
        }

        std::swap(table_, new_table);
    }

    void insert(int data, std::vector<ListNode*>& table) {
        int index = hash(data, table.size());

        ListNode* head = table[index];
        if(head == nullptr) {
            table[index] = new ListNode{data, nullptr};
            ++size_;
            return; 
        }

        while (head->next != nullptr)
        {
            head = head->next;
        }
        
        head->next = new ListNode{data, nullptr};
        ++size_;
    }

public:
    // Time - O(N)
    // Memory - O(N)
    HashTable(int cap)
        : size_(0)
        , table_(std::max(cap, 10), nullptr)
    {}

    // Time - O(1), O(1), O(N + K)
    // Memory - O(1), O(N + K)
    void insert(int data) {
        if(get_balance_factor() > 0.5) {
            resize();
        }

        insert(data, table_);
    }
    
    // O(1) O(N)
    // O(1)
    bool find(int data) {
        int index = hash(data, table_.size());
        ListNode* head = table_[index];
        while (head)
        {
            if(head->data == data) {
                return true;
            }
        }
        
        return false;
    }

    
    bool erase(int data) {
        // ...
        return false;
    }
    
    // O(1)
    int size() {
        return size_;
    }

    // Time - O(K + N)
    // Memory - O(1)
    void print() {
        for (ListNode* head : table_)
        {
            while (head)
            {
                std::cout << head->data << std::endl;
                head = head->next;
            }
        }
    }

private:
    int size_;
    std::vector<ListNode*> table_;
};

int main() {
    HashTable table(10);

    table.insert(1);
    table.insert(15);
    table.insert(1);
    table.insert(2);
    table.insert(3);
    table.insert(1);
    table.insert(12);

    table.print();
}
