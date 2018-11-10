#pragma once
#include <iostream>

template <typename T>
struct Node {
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    T key;

    size_t size;

    Node() {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        size = 1;
    }
    Node(const T& a) : Node() {
        key = a;
    }

};

template <typename T>
struct splay_tree {
    Node<T>* root;

    splay_tree() {
        root = nullptr;
    }

    splay_tree(Node<T>* root) {
        this->root = root;
    }

    void insert(const T& key) {
        if (root == nullptr) {
            root = new Node<T>(key);
            return;
        }

        Node<T>* res = find(key);

        if (res->key == key) return;
        if (key > res->key) {
            res->right = new Node<T>(key);
            res->right->parent = res;

            splay(res->right);
        }
        else {
            res->left = new Node<T>(key);
            res->left->parent = res;

            splay(res->left);
        }

    }

    bool exists(const T& key) {
        Node<T>* res = find(key);

        if (res == nullptr || res->key != key)
            return 0;
        else
            return 1;
    }
    

    Node<T>* find_by_order(size_t order) {
        if (order > size()) return nullptr;
        return find_by_order(order, root);
    }

    Node<T>* find_by_key(const T& key) {
        Node<T>* res = find(key);

        if (res->key != key) return nullptr;
        else return res;
    }

    size_t size() {
        return size(root);
    }

    void join(splay_tree<T>* R) {
        if (root == nullptr && R->root == nullptr) {
            delete R;
            return;
        }
        splay(find_by_order(size()));

        if (root != nullptr) {
            root->right = R->root;

            if (root->right != nullptr)
                root->right->parent = root;
        }
        else {
            root = R->root;
            root->parent = nullptr;
        }
        mend(root);

        delete R;
    }

    void print() {
        if (root != nullptr) {
            print_tree(root);
        }
    }

private:

    Node<T>* find_by_order(size_t order, Node<T>* cur) {

        if (order == size(cur->left) + 1) return cur;
        if (order <= size(cur->left)) {
            return find_by_order(order, cur->left);
        }
        else {
            return find_by_order(order - size(cur->left) - 1, cur->right);
        }
    }

    size_t size(Node<T>* cur) {
        return (cur == nullptr) ? 0 : cur->size;
    }
    //fix all functions in nodes
    Node<T>* mend(Node<T>* cur) {
        if (cur != nullptr) {
            cur->size = 1 + size(cur->left) + size(cur->right);
        }
        return cur;
    }

    Node<T>* find(const T& key) {
        if (root == nullptr) return nullptr;

        Node<T>* cur = root;
        while (true) {
            if (key > cur->key) {
                if (cur->right == nullptr) break;
                else cur = cur->right;
            }
            else if (key < cur->key) {
                if (cur->left == nullptr) break;
                else cur = cur->left;
            }
            else {
                break;
            }
        }

        return cur;
    }

    void zig(Node<T>* cur) {
        if (cur == nullptr) return;
        if (cur->parent == nullptr) {
            mend(cur);
            return;
        }
        if (cur->parent->parent) {
            if (cur->parent->parent->left == cur->parent) {
                cur->parent->parent->left = cur;
            }
            else {
                cur->parent->parent->right = cur;
            }
        }

        if (cur->parent->left == cur) {
            Node<T>* P = cur->parent;
            Node<T>* R = cur->right;
            cur->parent = P->parent;
            cur->right = P;
            P->parent = cur;
            P->left = R;
            if (R) {
                R->parent = P;
            }
        }

        else if (cur->parent->right == cur) {
            Node<T>* P = cur->parent;
            Node<T>* L = cur->left;
            cur->parent = P->parent;
            cur->left = P;
            P->parent = cur;
            P->right = L;
            if (L) {
                L->parent = P;
            }
        }

        if (cur->parent == nullptr) {
            root = cur;
        }

        mend(cur->left);
        mend(cur->right);
        mend(cur);
    }

    void splay(Node<T>* cur) {
        if (cur == nullptr) return;
        if (cur->parent == nullptr) {
            mend(cur);
            root = cur;
            return;
        }
        if (cur->parent->parent == nullptr) {
            zig(cur);
            root = cur;
            return;
        }
        zig(cur->parent);
        zig(cur);
        splay(cur);
    }

    Node<T>* get_max() {
        Node<T>* cur = root;
        if (cur == nullptr) return nullptr;
        while (cur->right != nullptr) {
            cur = cur->right;
        }

        return cur;
    }

    friend std::pair<splay_tree<T>*, splay_tree<T>*> split(splay_tree<T>* cur, const T& order) {
        if (order == 0) return { new splay_tree(nullptr), cur };
        Node<T>* res = cur->find_by_order(order);

        if (res == nullptr) return { nullptr, nullptr };

        cur->splay(res);

        Node<T>* L = cur->root;
        Node<T>* R = cur->root->right;
        cur->root->right = nullptr;
        if (R != nullptr)
            R->parent = nullptr;

        cur->mend(cur->root);
        delete cur;
        return {new splay_tree<T>(L), new splay_tree<T>(R)};
    }

};

template <class T>
void print_tree(Node<T>* cur) {
    if (cur->left) print_tree(cur->left);
    std::cout << cur->key << ' ';
    if (cur->right) print_tree(cur->right);
}