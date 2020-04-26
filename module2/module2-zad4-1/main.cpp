// Алексеев Сергей АПО-13
// Модуль 2 задание 4(1)


// Условие:
// В одной военной части решили построить в одну шеренгу по росту.
// Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
// а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
// Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие,
// а в конце – самые низкие. За расстановку солдат отвечал прапорщик,
// который заметил интересную особенность – все солдаты в части разного роста. Ваша задача состоит в том,
// чтобы помочь прапорщику правильно расставлять солдат, а именно для каждого приходящего солдата указывать,
// перед каким солдатом в строе он должен становится. Требуемая скорость выполнения команды - O(log n).


#include <iostream>
#include <string>

template <typename T, typename Comparator = std::less<T>>
class AvlTree {
    struct Node {
        Node(const T &data) : data(data), left(nullptr), right(nullptr), height(1), number(1) {}
        T data;
        Node *left, *right;
        size_t height;
        size_t number;
    };

public:
    AvlTree() : root(nullptr) {}
    ~AvlTree() {
        destroyTree(root);
    }

    bool has(const T &data) const {
        Node *tmp = root;
        while (tmp) {
            if (tmp->data == data) { return true; }
            else if (cmp(tmp->data, data)) { tmp = tmp->right; }
            else { tmp = tmp->left; }
        }
        return false;
    }

    void add(const T &data) {
        root = addInternal(root, data);
    }

    size_t searchIndex(const T &data) {
        size_t tmp = 0;
        Node * node = root;
        while(node != nullptr && node->data != data) {
            if (cmp(node->data, data)) {
                node = node->right;
            } else {
                size_t nRight = 0;
                if (node->right != nullptr) { nRight = node->right->number; }
                tmp += (nRight + 1);
                node = node->left;
            }
        }
        if (node == nullptr) { return 0; }
        size_t nRight = 0;
        if (node->right != nullptr) {
            nRight = node->right->number;
        }
        return tmp + nRight;
    }

    void remove(const T &data) {
        root = removeInternal(root, data);
    }

    void removeIndex(size_t index) {
        if (root == nullptr && index > root->number) { return; }

        size_t tmp = index;
        Node* node = root;

        int nRight = 0;
        if (node->right != nullptr) {
            nRight = node->right->number;
        }

        while(tmp != nRight) {
            if (cmp(nRight, tmp)) {
                node = node->left;
                tmp -= (nRight + 1);
            } else {
                node = node->right;
            }
            nRight = 0;
            if (node->right != nullptr) {
                nRight = node->right->number;
            }
        }
        remove(node->data);
    }

private:
    Node * root;
    Comparator cmp;
    void fixHeight(Node *node) {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        node->number = getNChild(node->right) + getNChild(node->left) + 1;
    }

    size_t getHeight(Node *node) {
        return node ? node->height : 0;
    }

    size_t getNChild(Node *node) {
        return node ? node->number : 0;
    }

    int getBalance(Node *node) {
        return getHeight(node->right) - getHeight(node->left);
    }

    Node* rotateLeft(Node *node) {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;

        fixHeight(node);
        fixHeight(tmp);

        return tmp;
    }

    Node* rotateRight(Node *node) {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }

    Node* doBalance(Node *node) {
        fixHeight(node);

        switch (getBalance(node)) {
            case 2: {
                if (getBalance(node->right) < 0)
                    node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            case -2: {
                if (getBalance(node->left) > 0)
                    node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            default:
                return node;
        }
    }

    Node* findMin(Node *node) {
        while (node->left)
            node = node->left;
        return node;
    }

    Node* removeMin(Node *node) {
        if (!node->left)
            return node->right;
        node->left = removeMin(node->left);
        return doBalance(node);
    }

    enum changeDelete {
        MIN, MAX
    };

    Node * findAndRemoveMinOrMax(Node * node, Node *& res, changeDelete flag) {
        switch (flag) {
            case changeDelete::MIN :
                if (node->left == nullptr) {
                    res = node;
                    return node->right;
                }
                node->left = findAndRemoveMinOrMax(node->left, res, flag);
                return doBalance(node);
            case changeDelete::MAX :
                if(node->right == nullptr) {
                    res = node;
                    return node->left;
                }
                node->right = findAndRemoveMinOrMax(node->right, res, flag);
                return doBalance(node);
        }
    }

    Node* addInternal(Node *node, const T &data) {
        if (!node)
            return new Node(data);
        if (!cmp(data, node->data))
            node->right = addInternal(node->right, data);
        else
            node->left = addInternal(node->left, data);
        return doBalance(node);
    }

    Node* removeInternal(Node *node, const T &data) {
        if (!node)
            return nullptr;
        if (cmp(node->data, data)) {
            node->right = removeInternal(node->right, data);
        }
        else if (cmp(data, node->data)) {
            node->left = removeInternal(node->left, data);
        }
        else {
            Node *left = node->left;
            Node *right = node->right;

            delete node;

            if (!right)
                return left;

            Node * work = nullptr;
            changeDelete flag;
            if(getHeight(left) >= getHeight(right)) {
                flag = changeDelete::MAX;
                Node * tmp = findAndRemoveMinOrMax(left, work, flag);
                work->left = tmp;
                work->right = right;
            } else {
                flag = changeDelete::MIN;
                Node * tmp = findAndRemoveMinOrMax(right, work, flag);
                work->right = tmp;
                work->left = left;
            }

            return doBalance(work);
        }
        return doBalance(node);
    }

    void destroyTree(Node *node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
};

int main(int argc, const char * argv[]) {
    AvlTree<int> avlTree;
    int cmd, h, n;
    std::cin >> n;
    for(size_t i = 0; i < n; ++i) {
        std::cin >> cmd >> h;
        switch(cmd) {
            case 1:
                avlTree.add(h);
                std::cout << avlTree.searchIndex(h) << std::endl;
                break;
            case 2:
                avlTree.removeIndex(h);
                break;
            default:
                std::cout << "error";
                break;
        }
    }
    return 0;
}