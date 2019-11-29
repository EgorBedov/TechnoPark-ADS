#include <cassert>
#include <iostream>
#include <functional>
#include <sstream>
#include <queue>

template <typename T>
class BST {
struct Node {
    Node() : left_(nullptr), right_(nullptr) {}
    explicit Node(const T& _key)
      : key_(_key),
        left_(nullptr),
        right_(nullptr) {};

    T key_;
    Node* left_;
    Node* right_;
};
public:
    explicit BST(std::function<bool(const T &l, const T &r)> _comp) : comp_(_comp), root_(nullptr) {};
    ~BST() { Destruct(); }

    void Insert(const T& key) {
        auto new_node = new Node(key);

        /// Check if tree is empty
        if ( !root_ ) {
            root_ = new_node;
            return;
        }

        Node* node = root_;
        Node* prev = nullptr;

        /// Go to its leaf
        while ( node ) {
            prev = node;
            node = (comp_(node->key_, key) ? node->left_ : node->right_);
        }

        /// Replace empty leaf
        (comp_(prev->key_, key) ? prev->left_ : prev->right_) = new_node;
    }

    int WidestLayer()  {
        Node* node = root_;
        int result = 0;
        /// Empty tree case
        if ( !node )
            return result;

        /// Create queue with root in it
        std::queue<Node*> q;
        q.push(node);

        /// Pop and push nodes layer by layer
        while ( !q.empty() ) {
            int count = q.size();
            result = std::max(count, result);

            while ( count-- ) {
                node = q.front();
                if ( node->left_ )
                    q.push(node->left_);
                if ( node->right_ )
                    q.push(node->right_);
                q.pop();
            }
        }
        return result;
    }

private:
    void Destruct() {
        /// Using BFS for destruction
        std::queue<Node*> q;
        q.push(root_);

        Node* node = nullptr;
        while ( !q.empty() ) {
            node = q.front();
            if ( node->left_ )
                q.push(node->left_);
            if ( node->right_ )
                q.push(node->right_);
            delete node;
            q.pop();
        }
    }

    std::function<bool(const T &l, const T &r)> comp_;
    Node* root_;
};

template <typename T>
class DT {
struct Node {
    Node() : left_(nullptr), right_(nullptr) {}
    Node(const T& _key, int _priority)
      : key_(_key),
        priority_(_priority),
        left_(nullptr),
        right_(nullptr) {};

    ~Node() {
        if ( left_ ) delete left_;
        if ( right_ ) delete right_;
    }

    T key_;
    int priority_;
    Node* left_;
    Node* right_;
};
public:
    explicit DT(std::function<bool(const T &l, const T &r)> _comp) : comp_(_comp), root_(nullptr) {};
    ~DT() { delete root_; }

    void Insert(const T& key, const int priority) {
        auto new_node = new Node(key, priority);
        Insert_(root_, new_node);
    }
    int WidestLayer()  {
        Node* node = root_;
        int result = 0;
        /// Empty tree case
        if ( !node )
            return result;

        /// Create queue with root in it
        std::queue<Node*> q;
        q.push(node);

        /// Pop and push nodes layer by layer
        while ( !q.empty() ) {
            int count = q.size();
            result = std::max(count, result);

            while ( count-- ) {
                node = q.front();
                if ( node->left_ )
                    q.push(node->left_);
                if ( node->right_ )
                    q.push(node->right_);
                q.pop();
            }
        }
        return result;
    }

private:
    /// Разрезание декартового дерева по ключу.
    void Split(Node* currentNode, const T& key, Node*& left, Node*& right) {
        if ( !currentNode ) {
            left = nullptr;
            right = nullptr;
        } else if ( comp_(currentNode->key_, key) ) {
            Split( currentNode->left_, key, left, currentNode->left_ );
            right = currentNode;
        } else {
            Split( currentNode->right_, key, currentNode->right_, right );
            left = currentNode;
        }
    }

    void Insert_(Node*& node, Node* new_node) {
        if ( !node ) node = new_node;
        else if ( comp_(new_node->priority_, node->priority_) ) {
            Split(node, new_node->key_, new_node->left_, new_node->right_ );
            node = new_node;
        } else {
            Insert_( comp_(node->key_, new_node->key_) ? node->left_ : node->right_, new_node);
        }
    }

    std::function<bool(const T &l, const T &r)> comp_;
    Node* root_;
};

void run(std::istream &input, std::ostream &output) {
    int amount = 0;
    input >> amount;

    std::function<bool(const int& l, const int& r)> comp = [](const int& l, const int& r) { return l > r; };

    BST<int> bst(comp);
    DT<int> dt(comp);

    int key = 0;
    int priority = 0;
    for ( size_t iii = 0; iii < amount; ++iii ) {
        input >> key >> priority;
        bst.Insert(key);
        dt.Insert(key, priority);
    }

    output << dt.WidestLayer() - bst.WidestLayer();
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

        assert(output.str() == "1");
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

        assert(output.str() == "1" && "Wrong answer!");
    }
    std::cout << "OK" << std::endl;
}

int main() {

    testLogic();
//    run(std::cin, std::cout);

    return 0;
}
