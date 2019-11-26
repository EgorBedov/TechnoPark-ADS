#include <iostream>
#include <sstream>
#include <queue>


// TODO: consider making class BaseNode which BSTNode and DTNode can inherit from
// that way you can make WidestLayer work with BaseNode while working with those two (upcast)


bool more(const int left, const int right) {
    return left > right;
}

namespace BST {
struct Node {
    int data;
    Node* left;
    Node* right;

    explicit Node(int _data) : data(_data), left(nullptr), right(nullptr) {}
    ~Node() {    /// Valgrind proves that this thing actually clears everything
        if ( left )     delete left;
        if ( right )    delete right;
    }
};

void Insert(Node *&root, const int data, bool (*comparison_func)(const int left, const int right)) {
    auto new_node = new Node(data);
    Node *node = root;
    Node *prev = nullptr;

    /// Check if tree is empty
    if (!root) {
        root = new_node;
        return;
    }

    /// Head to its leaf
    while (node) {
        prev = node;
        node = (comparison_func(node->data, data) ? node->left : node->right);
    }

    /// Replace empty leaf
    (comparison_func(prev->data, data) ? prev->left : prev->right) = new_node;
}
} // namespace

namespace DT {  // Treap - Декартово дерево
struct Node {
    int data;
    int priority;
    Node* left;
    Node* right;

    explicit Node(int _data, int _priority) :
        data(_data),
        priority(_priority),
        left(nullptr),
        right(nullptr) {}
    ~Node() {    /// Valgrind proves that this thing actually clears everything
        if ( left )     delete left;
        if ( right )    delete right;
    }
};
// Разрезание декартового дерева по ключу.
void Split( Node* currentNode, int data, Node*& left, Node*& right ) {
    if( !currentNode ) {
        left = nullptr;
        right = nullptr;
    } else if( currentNode->data <= data ) {
        Split( currentNode->right, data, currentNode->right, right );
        left = currentNode;
    } else {
        Split( currentNode->left, data, left, currentNode->left );
        right = currentNode;
    }
}
void Insert(Node *&node, const int data, const int priority, bool (*comparison_func)(const int left, const int right)) {
    while ( node ) {
        if ( node->priority > priority ) {
            if ( node->left ) {
                if ( node->left->priority > priority )
                    node = node->left;
            } else if ( node->right ) {
                if ( node->right->priority > priority )
                    node = node->right;
            }
        }
    }
    Split(node, data, nullptr, nullptr);
}
}

template <typename Node>
int WidestLayer(const Node *node) {
    int result = 0;

    /// Empty tree case
    if ( !node )
        return result;

    /// Create queue with root in it
    std::queue<const Node*> q;
    q.push(node);

    /// Pop and push nodes layer by layer
    while ( !q.empty() ) {
        int count = q.size();
        result = std::max(count, result);

        while ( count-- ) {
            node = q.front();
            if ( node->left )
                q.push(node->left);
            if ( node->right )
                q.push(node->right);
            q.pop();
        }
    }
    return result;
}

void run(std::istream &input, std::ostream &output) {
    int amount = 0;
    input >> amount;

    BST::Node* BST_root = nullptr;
    DT::Node* DT_root = nullptr;

    int data = 0;
    int priority = 0;
    for ( size_t iii = 0; iii < amount; ++iii ) {
        input >> data >> priority;
        BST::Insert(BST_root, data, more);
        DT::Insert(DT_root, data, priority, more);
    }

    output << WidestLayer(BST_root) - WidestLayer(DT_root);
}

void testLogic() {
    { // 1 Условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "10\n"
                 "5 11\n"
                 "18 8\n"
                 "25 7\n"
                 "50 12\n"
                 "30 30\n"
                 "15 15\n"
                 "20 10\n"
                 "22 5\n"
                 "40 20\n"
                 "45 9" << std::endl;

        run(input, output);

        assert(output.str() == "2");
    }
    { // 2 Условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "10\n"
                 "38 19\n"
                 "37 5\n"
                 "47 15\n"
                 "35 0\n"
                 "12 3\n"
                 "0 42\n"
                 "31 37\n"
                 "21 45\n"
                 "30 26\n"
                 "41 6" << std::endl;

        run(input, output);

        assert(output.str() == "2");
    }
}

int main() {

    testLogic();
//    run(std::cin, std::cout);

    return 0;
}

