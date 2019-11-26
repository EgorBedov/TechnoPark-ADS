#include <cassert>
#include <iostream>
#include <sstream>
#include <queue>


struct BSTNode {
    int data;
    BSTNode* left;
    BSTNode* right;

    explicit BSTNode(int _data) : data(_data), left(nullptr), right(nullptr) {}
};

bool more(const int left, const int right) {
    return left > right;
}

void Insert(BSTNode*& root, const int data, bool (*comparison_func)(const int left, const int right)) {
    auto new_node = new BSTNode(data);
    BSTNode* node = root;
    BSTNode* prev = nullptr;

    /// Check if tree is empty
    if ( !root ) {
        root = new_node;
        return;
    }

    /// Head to its leaf
    while ( node ) {
        prev = node;
        node = (comparison_func(node->data, data) ? node->left : node->right);
    }

    /// Replace empty leaf
    ( comparison_func(prev->data, data) ? prev->left : prev->right ) = new_node;
}

void BFS(const BSTNode* node, std::ostream& output) {
    /// Create queue with root in it
    std::queue<const BSTNode*> q;
    q.push(node);

    /// Pop and push nodes layer by layer
    while ( !q.empty() ) {
        node = q.front();
        if ( node->left )
            q.push(node->left);
        if ( node->right )
            q.push(node->right);
        output << node->data << " ";
        q.pop();
    }
}

void run(std::istream &input, std::ostream &output) {
    int amount = 0;
    input >> amount;

    BSTNode* root = nullptr;

    int data = 0;
    for ( size_t iii = 0; iii < amount; ++iii ) {
        input >> data;
        Insert(root, data, more);
    }

    BFS(root, output);

    /// Clear table
    std::queue<BSTNode*> q;
    q.push(root);

    /// Pop and push nodes layer by layer
    BSTNode* node = nullptr;
    while ( !q.empty() ) {
        node = q.front();
        if ( node->left )
            q.push(node->left);
        if ( node->right )
            q.push(node->right);
        delete node;
        q.pop();
    }
}

void testLogic() {
    { // 1 Условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "3\n2 1 3" << std::endl;

        run(input, output);

        assert(output.str() == "2 1 3 ");
    }
    { // 1 Условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "3\n1 2 3" << std::endl;

        run(input, output);

        assert(output.str() == "1 2 3 ");
    }
    { // 1 Условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "3\n3 1 2" << std::endl;

        run(input, output);

        assert(output.str() == "3 1 2 ");
    }
    { // 1 Условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "4\n3 1 4 2" << std::endl;

        run(input, output);

        assert(output.str() == "3 1 4 2 ");
    }

    std::cout << "OK" << std::endl;
}

int main() {

    testLogic();
//    run(std::cin, std::cout);

    return 0;
}