#ifndef BST_H
#define BST_H
#include <functional>
#include <initializer_list>
#include <iostream>
#include <queue>

class BST {
public:
    class Node {
    public:
        explicit Node(int value, Node *left = nullptr, Node *right = nullptr);

        Node();

        Node(const Node &node);

        bool operator<(int a) const;

        bool operator<=(int a) const;

        bool operator>(int a) const;

        bool operator>=(int a) const;

        bool operator==(int a) const;

        friend std::ostream &operator<<(std::ostream &os, const Node &n);

        int value;
        Node *left;
        Node *right;
    };

    BST() {
        root = nullptr;
    }

    BST(BST &bst) {
        root = nullptr;
        add_node(bst.get_root()->value);

        std::vector<int> nodes;
        bst.bfs([&nodes](BST::Node *&node) {
            nodes.push_back(node->value);
        });

        for (const auto node: nodes) {
            add_node(node);
        }
    }

    BST(BST &&bst) noexcept {
        root = bst.root;
        bst.root = nullptr;
    }

    BST(std::initializer_list<int> nodes) {
        root = nullptr;
        for (const auto node: nodes) {
            add_node(node);
        }
    }

    ~BST() {
        std::vector<Node *> nodes;
        bfs([&nodes](BST::Node *&node) { nodes.push_back(node); });
        for (const auto &node: nodes)
            delete node;
    }

    BST &operator++();

    BST &operator++(int a);

    BST &operator=(const BST &bst);

    BST &operator=(BST &&bst) noexcept;

    Node *&get_root();

    void bfs(const std::function<void(Node *&node)> &func);

    size_t length();

    bool add_node(int value);

    Node **find_node(int value);

    Node **find_parent(int value);

    Node **find_successor(int value);

    bool delete_node(int value);

    friend std::ostream &operator<<(std::ostream &os, BST &bst);

private:
    Node *root;
};

bool operator>(int a, const BST::Node &b);

bool operator>=(int a, const BST::Node &b);

bool operator<(int a, const BST::Node &b);

bool operator<=(int a, const BST::Node &b);

bool operator==(int a, const BST::Node &b);

#endif // BST_H
