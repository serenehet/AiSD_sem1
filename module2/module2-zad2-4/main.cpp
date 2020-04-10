// Алексеев Сергей АПО-13
// Модуль 2 задание 2(4)

// Условие:
// Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N.
// Требуется построить бинарное дерево поиска, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
// то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
// Выведите элементы в порядке level-order (по слоям, “в ширину”).

#include <iostream>
#include <queue>
#include <functional>
using namespace std;

template<typename T>
struct Node {
    T inf;
    Node<T> * left;
    Node<T> * right;
    Node(T element) {
        inf = element;
        left = nullptr;
        right = nullptr;
    }
};
template <typename T, typename Comparator = std::less<T>>
class BinaryTree {
public:
    BinaryTree() {
        head = nullptr;
    }
    ~BinaryTree() {
        widthSearch([](Node<T> * node) { delete node; });
    }
    bool isEmpty() {
        return head == nullptr;
    }
    void add(T element) {
        if (isEmpty()) {
            head = new Node(element);
            return;
        }
        Node<T> * node = head;
        Node<T> * parent;
        while(node != nullptr) {
            parent = node;
            if (cmp(element, node->inf)) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        if (cmp(element, parent->inf)) {
            parent->left = new Node(element);
        } else {
            parent->right = new Node(element);
        }
    }
    void printLevelOrder() {
        widthSearch([](Node<T> * node) {cout << node->inf << " "; });
    }
private:
    Node<T> * head;
    Comparator cmp;
    void widthSearch(const function<void(Node<T> * node)> f) {
        Node<T> * temp = head;
        if (temp == nullptr) { return; }
        queue<Node<T> *> q;
        q.push(temp);
        while(!q.empty()) {
            Node<T> * node = q.front();
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
};

int main() {
    BinaryTree<int> tree;
    size_t n;
    cin >> n;
    for (size_t i = 0; i < n; ++i) {
        int temp;
        cin >> temp;
        tree.add(temp);
    }
    // вывод решения
    tree.printLevelOrder();
    return 0;
}
