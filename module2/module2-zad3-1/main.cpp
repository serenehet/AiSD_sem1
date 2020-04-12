#include <iostream>
#include <queue>
#include <stack>
#include <functional>
#include <math.h>
using namespace std;

template<typename T>
struct BNode {
    T inf;
    BNode<T> * left;
    BNode<T> * right;
    BNode(T element) : inf(element), left (nullptr), right(nullptr) {}
};

template <typename T, typename Comparator = std::less<T>>
class BTree {
public:
    BTree() {
        head = nullptr;
    }
    ~BTree() {
        widthSearch([](BNode<T> * node) { delete node; });
    }
    bool isEmpty() {
        return head == nullptr;
    }
    void add(T element) {
        if (isEmpty()) {
            head = new BNode<T>(element);
            return;
        }
        BNode<T> * node = head;
        BNode<T> * parent;
        while(node != nullptr) {
            parent = node;
            if (cmp(element, node->inf)) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        if (cmp(element, parent->inf)) {
            parent->left = new BNode<T>(element);
        } else {
            parent->right = new BNode<T>(element);
        }
    }
    int height() {
        return h(head);
    }
    void printLevelOrder() {
        widthSearch([](BNode<T> * node) {cout << node->inf << " "; });
    }
private:
    BNode<T> * head;
    Comparator cmp;
    void widthSearch(const function<void(BNode<T> * node)> f) {
        BNode<T> * temp = head;
        if (temp == nullptr) { return; }
        queue<BNode<T> *> q;
        q.push(temp);
        while(!q.empty()) {
            BNode<T> * node = q.front();
            q.pop();
            if (node->left != nullptr) {
                q.push(node->left);
            }
            if (node->right != nullptr) {
                q.push(node->right);
            }
            f(node);
        }
    }
    int h(BNode<T> * node ) {
        if (node == nullptr) { return 0; }
        else {
            return 1 + max(h(node->left), h(node->right));
        }
    }
};

template<typename T>
struct DNode {
    T inf;
    int priority;
    DNode<T> * left;
    DNode<T> * right;
    DNode(T element, int y) : inf(element), priority(y), left(nullptr), right(nullptr){ }
};

template<typename T>
class DTree {
public:
    DTree() : head(nullptr) {}
    ~DTree() {

    }
    void add(int element, int priority) {
        if (head == nullptr) {
            head = new DNode<T>(element, priority);
        } else {
            auto * node = head;
            auto * parent = head;
            while (node && (node->priority >= priority)) {
                parent = node;
                if (node->inf >= element) {
                    node = node->left;
                } else {
                    node = node->right;
                }
            }
            auto * item = new DNode<T>(element, priority);
            split(node, element, item->left, item->right);
            if (node == head) {
                head = item;
            } else {
                if (parent->inf >= element) {
                    parent->left = item;
                } else {
                    parent->right = item;
                }
            }
        }
    }

    int height() {
        return h(head);
    }

    int h(DNode<T> * node ) {
        if (node == nullptr) { return 0; }
        else {
            return 1 + max(h(node->left), h(node->right));
        }
    }

private:
    DNode<T> * head;
    void split(DNode<T>* currentNode, T key, DNode<T> *& left, DNode<T>*& right) {
        if (!currentNode) {
            left = right = nullptr;
            return;
        }
        if (currentNode->inf <= key) {
            split(currentNode->right, key, currentNode->right, right);
            left = currentNode;
        } else {
            split(currentNode->left, key, left, currentNode->left);
            right = currentNode;
        }
    }
};

int main() {
//    BTree<int> tree;
//    tree.add(2);
//    tree.add(1);
//    tree.add(3);
//    tree.add(4);
//    cout << tree.height();
    int n;
    cin >> n;
    BTree<int> btree;
    DTree<int> dtree;
    for(size_t i = 0; i < n; ++i) {
        int x, y;
        cin >> x >> y;
        dtree.add(x, y);
        btree.add(x);
    }
    cout << (btree.height() - dtree.height());
    return 0;
}
