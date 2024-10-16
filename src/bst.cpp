#include "bst.h"
#include <functional>
#include <iomanip>
#include <ostream>
#include <vector>

BST::Node::Node(const int value, Node *left, Node *right)
    : value(value)
      , left(left)
      , right(right) {
}

BST::Node::Node() {
    value = 0;
    left = nullptr;
    right = nullptr;
}

BST::Node::Node(const Node &node) {
    value = node.value;
    left = node.left;
    right = node.right;
}

bool BST::Node::operator<(const int a) const {
    return value < a;
}

bool BST::Node::operator<=(const int a) const {
    return value <= a;
}

bool BST::Node::operator>=(const int a) const {
    return value >= a;
}

bool BST::Node::operator>(const int a) const {
    return value > a;
}

bool BST::Node::operator==(const int a) const {
    return value == a;
}

bool operator>(const int a, const BST::Node &b) {
    return b < a;
}

bool operator>=(const int a, const BST::Node &b) {
    return b <= a;
}

bool operator<(const int a, const BST::Node &b) {
    return b > a;
}

bool operator<=(const int a, const BST::Node &b) {
    return b >= a;
}

bool operator==(const int a, const BST::Node &b) {
    return b == a;
}

BST &BST::operator++() {
    bfs([](BST::Node *&node) {
        node->value++;
    });
    std::cout << *this << std::endl;
    return *this;
}

BST &BST::operator++(int a) {
    static BST temp{*this};

    bfs([](BST::Node *&node) {
        node->value++;
    });

    return temp;
}

BST &BST::operator=(const BST &bst) {
    if (this == &bst) {
        return *this;
    }

    std::vector<int> nodes;
    bfs([&nodes](BST::Node *&node) {
        nodes.push_back(node->value);
    });
    for (const auto node: nodes) {
        add_node(node);
    }
    return *this;
}

BST &BST::operator=(BST &&bst) noexcept {
    root = bst.root;
    bst.root = nullptr;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const BST::Node &n) {
    os << &n << " value: " << n.value << " left: " << n.left << " right: " << n.right << std::endl;
    return os;
}

BST::Node *&BST::get_root() {
    return root;
}

void BST::bfs(const std::function<void(Node *&node)> &func) {
    if (root == nullptr) {
        func(root);
        return;
    }

    std::queue<BST::Node *> que;
    que.push(root);

    while (!que.empty()) {
        auto temp = que.front();
        que.pop();
        func(temp);

        if (temp->left != nullptr) {
            que.push(temp->left);
        }
        if (temp->right != nullptr) {
            que.push(temp->right);
        }
    }
}

std::ostream &operator<<(std::ostream &os, BST &bst) {
    os << std::string(80, '*') << std::endl;
    bst.bfs([&os](BST::Node *&node) {
        os << std::setiosflags(std::ios::left);
        os << std::setw(20) << node << "=> value:";
        os << std::setw(10) << node->value << "left:";
        os << std::setw(20) << node->left << "right:";
        os << std::setw(20) << node->right << std::endl;
    });
    os << "binary search tree size: " << bst.length() << std::endl;
    os << std::string(80, '*') << std::endl;
    return os;
}

size_t BST::length() {
    int count = 0;
    bfs([&count](Node *&node) { count++; });
    return count;
}

bool BST::add_node(const int value) {
    Node **node = &root;
    if (node == nullptr) {
        root = new Node(value);
        return true;
    }
    while (*node) {
        if (value < (*node)->value) {
            node = &(*node)->left;
        } else if (value > (*node)->value) {
            node = &(*node)->right;
        } else {
            return false;
        }
    }
    *node = new Node(value);
    return true;
}

BST::Node **BST::find_node(const int value) {
    Node **node = &root;
    while (*node) {
        if ((*node)->value == value) {
            return node;
        }
        if (value < (*node)->value) {
            node = &(*node)->left;
        } else {
            node = &(*node)->right;
        }
    }
    return nullptr;
}

BST::Node **BST::find_parent(const int value) {
    Node **node = &root;
    if ((*node)->value == value) {
        return nullptr;
    }
    while (*node) {
        Node *current = *node;
        if (value < current->value) {
            if (current->left && current->left->value == value) {
                return node;
            }
            node = &current->left;
        } else if (value > current->value) {
            if (current->right && current->right->value == value) {
                return node;
            }
            node = &current->right;
        } else {
            return nullptr;
        }
    }
    return nullptr;
}

// find largest value in left subtree
BST::Node **BST::find_successor(const int value) {
    Node **node = find_node(value);
    if (node == nullptr) {
        return nullptr;
    }
    Node **successor = &(*node)->left;
    while ((*successor)->right) {
        successor = &(*successor)->right;
    }
    return successor;
}

bool BST::delete_node(const int value) {
    Node **node = find_node(value);
    if (node == nullptr) {
        return false;
    }

    if ((*node)->left == nullptr && (*node)->right == nullptr) {
        // no child
        delete *node;
        *node = nullptr;
    } else if ((*node)->left == nullptr) {
        // only left child
        const Node *temp = *node;
        *node = (*node)->right;
        delete temp;
    } else if ((*node)->right == nullptr) {
        // only right child
        const Node *temp = *node;
        *node = (*node)->left;
        delete temp;
    } else {
        // both children
        // find the largest value in left subtree
        Node **successor = find_successor(value);
        // copy the value of successor to the node
        (*node)->value = (*successor)->value;
        // delete the successor
        delete *successor;
        *successor = nullptr;
    }
    return true;
}
