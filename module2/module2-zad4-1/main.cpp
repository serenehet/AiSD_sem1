#include <iostream>

template <typename T>
class AvlTree {
    struct Node {
        Node(const T &data) : data(data), left(nullptr), right(nullptr), height(1), nChild(1) {}
        T data;
        Node *left, *right;
        size_t height;
        size_t nChild;
    };

public:
    AvlTree() : root(nullptr) {}
    ~AvlTree() {
        destroyTree(root);
    }

    bool has(const T &data) {
        Node *tmp = root;
        while (tmp)
        {
            if (tmp->data == data)
                return true;
            else if (tmp->data < data)
                tmp = tmp->right;
            else
                tmp = tmp->left;
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
            if (node->data < data) {
                node = node->right;
            } else {
                size_t nRight = 0;
                if (node->right != nullptr) { nRight = node->right->nChild; }
                tmp += (nRight + 1);
                node = node->left;
            }
        }
        if (node == nullptr) { return 0; }
        size_t nRight = 0;
        if (node->right != nullptr) {
            nRight = node->right->nChild;
        }
        return tmp + nRight;
    }

    void remove(const T &data) {
        root = removeInternal(root, data);
    }

    void removeIndex(int pos) {
        int tmp = pos;
        Node* node = root;

        while (true){
            int tmpRightNum = node->right ? node->right->nChild : 0;

            if (tmpRightNum == tmp){
                remove(node->data);
                break;
            }
            else if((tmp > tmpRightNum)){
                node = node->left;
                tmp -= (tmpRightNum + 1);
            }
            else{
                node = node->right;
            }
        }
    }

private:
    Node *root;
    void fixHeight(Node *node) {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        node->nChild = getNChild(node->right) + getNChild(node->left) + 1;
    }

    size_t getHeight(Node *node) {
        return node ? node->height : 0;
    }

    size_t getNChild(Node *node) {
        return node ? node->nChild : 0;
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

        switch (getBalance(node))
        {
            case 2:
            {
                if (getBalance(node->right) < 0)
                    node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            case -2:
            {
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

    Node* addInternal(Node *node, const T &data) {
        if (!node)
            return new Node(data);
        if (node->data <= data)
            node->right = addInternal(node->right, data);
        else
            node->left = addInternal(node->left, data);
        return doBalance(node);
    }

    Node* removeInternal(Node *node, const T &data) {
        if (!node)
            return nullptr;
        if (node->data < data) {
            node->right = removeInternal(node->right, data);
        }
        else if (node->data > data) {
            node->left = removeInternal(node->left, data);
        }
        else {
            Node *left = node->left;
            Node *right = node->right;

            delete node;

            if (!right)
                return left;

            /*
             Необходимо сделать 2 модификации:
             1. Из двух функций findMin и removeMin сделать одну -- findAndRemoveMin, которая делает работу этих двух за один проход.
             2. Я вместо удаляемого подставляю минимальный элемент из правого поддерева. Вам нужно посмотреть на высоты правого и левого поддеревьев и использовать либо максимальный из левого, либо минимальный из правого, в зависимости от того, какое поддерево более глубокое (брать более глубокое).
             */

            Node *min = findMin(right);
            min->right = removeMin(right);
            min->left = left;

            return doBalance(min);
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
                avlTree.removeIndex((size_t)h);
                break;
            default:
                std::cout << "error";
                break;
        }
    }
    return 0;
}