#include <iostream>
#include <functional>
#include <sstream>
#include <queue>

template <typename T>
class TreeNode {
public:
    explicit TreeNode(T _key)
      : key_(_key),
        left_(nullptr),
        right_(nullptr) {};

    virtual ~TreeNode() = default;

    T key_;
    TreeNode* left_;
    TreeNode* right_;
};

template <typename T>
class BSTNode : public TreeNode<T> {
public:
    explicit BSTNode(T _key) : TreeNode<T>(_key) {}
};

template <typename T>
class DTNode : public TreeNode<T> {
public:
    DTNode(T _key, int _priority)
      : TreeNode<T>(_key),
        priority_(_priority) {};

    ~DTNode() override {
        if ( this->left_ ) delete this->left_;
        if ( this->right_ ) delete this->right_;
    }

    int priority_;
};

template <typename T, class Node = TreeNode<T> >
class IBinaryTree {
public:
    explicit IBinaryTree(std::function<bool(const T &l, const T &r)> _comp)
      : comp_(_comp) {}
    virtual ~IBinaryTree() = default;

    virtual int WidestLayer()  {
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

protected:
    std::function<bool(const T &l, const T &r)> comp_;
    Node* root_;
};

template <typename T, class Node = BSTNode<T> >
class BST : public IBinaryTree<T, Node> {
public:
    explicit BST(std::function<bool(const T &l, const T &r)> _comp) : IBinaryTree<T, Node>(_comp) {};
    ~BST() override {
        Destruct();
    }

    void Insert(const T key) {
        auto new_node = new Node(key);

        /// Check if tree is empty
        if ( !this->root_ ) {
            this->root_ = new_node;
            return;
        }

        Node* node = this->root_;
        Node* prev = nullptr;

        /// Go to its leaf
        while ( node ) {
            prev = node;
            node = (this->comp_(node->key_, key) ? node->left_ : node->right_);
        }

        /// Replace empty leaf
        (this->comp_(prev->key_, key) ? prev->left_ : prev->right_) = new_node;
    }

    void Destruct() {
        /// Using BFS for destruction
        std::queue<Node*> q;
        q.push(this->root_);

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
};

template <typename T>
class DT : public IBinaryTree<T, DTNode<T> > {
public:
    explicit DT(std::function<bool(const T &l, const T &r)> _comp) : IBinaryTree<T, DTNode<T> >(_comp) {};
    ~DT() override {
        delete this->root_;
    }

    /// Разрезание декартового дерева по ключу.
    void Split(DTNode<T>* currentNode, T key, DTNode<T>*& left, DTNode<T>*& right) {
        if ( !currentNode ) {
            left = nullptr;
            right = nullptr;
        } else if ( currentNode->key_ <= key ) {
            Split( currentNode->right_, key, currentNode->right_, right );
            left = currentNode;
        } else {
            Split( currentNode->left_, key, left, currentNode->left_ );
            right = currentNode;
        }
    }
    void Insert(const T key, const int priority) {
        /// Check if tree is empty
        if ( !this->root_ ) {
            this->root_ = new DTNode<T>(key, priority);
            return;
        }
        /////////////////////////////////////////////// nah
        DTNode<T>* node = this->root_;
        while ( node ) {
            if ( this->comp_(node->priority_, priority) ) {
                if ( node->left_ ) {
                    if ( this->comp_(node->left_->priority_, priority) )
                        node = node->left_;
                } else if ( node->right_ ) {
                    if ( this->comp_(node->right_->priority_, priority) )
                        node = node->right_;
                }
            }
        }
        Split(node, key, node->left_, node->right_);
    }
};

void run(std::istream &input, std::ostream &output) {
    int amount = 0;
    input >> amount;

    std::function<bool(int l, int r)> comp = [](int l, int r) { return l > r; };

    BST<int> bst(comp);
    DT<int> dt(comp);

    int key = 0;
    int priority = 0;
    for ( size_t iii = 0; iii < amount; ++iii ) {
        input >> key >> priority;
        bst.Insert(key);
        dt.Insert(key, priority);
    }

    output << bst.WidestLayer() - dt.WidestLayer();
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

