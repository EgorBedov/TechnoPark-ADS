#include <cassert>
#include <iostream>
#include <sstream>

// TODO: comparator

template <typename T>
struct Node {
    T data;
    int height;
    Node* left;
    Node* right;

    Node(T _data) :
        data(_data),
        height(1),
        left(nullptr),
        right(nullptr)
        {};
};

template <typename T>
class Comparator {
public:
    bool operator() (const T &left, const T& right) {
        return left < right;
    }
};

template <typename T, class Comparator=std::less<T> >
class AVLTree {
public:
    AVLTree() : root_(nullptr) {};
    ~AVLTree() { free_(root_); }

    void Insert(const T data) {
        root_ = Insert_(root_, data);
    }
    Node<T>* Find(const T data) const {
        return Find_(root_, data);
    }
    void Delete(const T data) {
        root_ = Delete_(root_, data);
    }
    int GetKStat(int k) {
        return GetKStat_(root_, k);
    }

private:
    Node<T>* Insert_(Node<T>* node, const T data) {
        if ( !node )
            return new Node<T>(data);
        if ( comp(data, node->data) )   /*node->data > data*/
            node->left = Insert_(node->left, data);
        else
            node->right = Insert_(node->right, data);
        return Rebalance_(node);
    }
    Node<T>* Find_(Node<T>* node, const T data) const {
        if ( !node )
            return nullptr;
        if ( node->data == data )
            return node;
        if ( comp(data, node->data) )   /*node->data > data*/
            return Find_(node->left, data);
        if ( comp(node->data, data) )   /*node->data < data*/
            return Find_(node->right, data);
    }
    Node<T>* Delete_(Node<T>*& node, const T data) {
        if ( !node ) return nullptr;

        if ( comp(data, node->data) )   /*node->data > data*/
            node->left = Delete_(node->left, data);
        else if ( comp(node->data, data) )   /*node->data < data*/
            node->right = Delete_(node->right, data);
        else {
            Node<T>* new_parent = node->left;
            Node<T>* prev_node = node->right;
            delete node;

            if( !prev_node ) return new_parent;

            Node<T>* min = GetDeleteMin(prev_node);
            min->right = prev_node;
            min->left = new_parent;

            return Rebalance_(min);
        }
        return Rebalance_(node);
    }

    int GetKStat_(Node<T>* node, const int k) const {
        assert(node);
        int result = 0;
        int amount = Count_(node->left);
        if ( amount > k ) {
			result = GetKStat_(node->left, k);
		} else if ( amount < k ) {
			result = GetKStat_(node->right, k - amount - 1);
		} else {
			result = node->data;
		}
		return result;
    }
    int Count_(Node<T>* node) const {
        if ( !node ) return 0;
        int result = 1;
        result += Count_(node->left);
        result += Count_(node->right);
        return result;
    }
    int GetHeight_(const Node<T>* node) const {
        return node ? node->height : 0;
    }
    int GetBF_(const Node<T>* node) const {
        assert(node);
        return GetHeight_(node->right) - GetHeight_(node->left);
    }
    void FixHeight_(Node<T>* node) {
        if ( !node ) return;
        int left_height = GetHeight_(node->left);
	    int right_height = GetHeight_(node->right);
	    node->height = (left_height > right_height ? left_height : right_height ) + 1;
    }
    Node<T>* Rebalance_(Node<T>* node) {
        if (!node) return nullptr;
        FixHeight_(node);
        int bf = GetBF_(node);
        /// Right subtree is unbalanced
        if (bf == 2) {
            if (GetBF_(node->right) < 0) {    // right-right case
                node->right = RightRotation_(node->right);
            }                                   // right-left case
            return LeftRotation_(node);
        }
        /// Left subtree is unbalanced
        if (bf == -2) {
            if (GetBF_(node->left) > 0) {     // left-left case
                node->left = LeftRotation_(node->left);
            }                                   // left-right case
            return RightRotation_(node);
        }
        return node;
    }
    Node<T>* LeftRotation_(Node<T>* node) {
        if ( !node ) return nullptr;
        Node<T>* new_parent = node->right;
        node->right = new_parent->left;
        new_parent->left = node;
        FixHeight_(node);
        FixHeight_(new_parent);
        return new_parent;
    }
    Node<T>* RightRotation_(Node<T>* node) {
        if ( !node ) return nullptr;
        Node<T>* new_parent = node->left;
        node->left = new_parent->right;
        new_parent->right = node;
        FixHeight_(node);
        FixHeight_(new_parent);
        return new_parent;
    }
    Node<T>* GetMax_(Node<T>* node) {
        while ( node->right )
            node = node->right;
        return node;
    }
    Node<T>* GetDeleteMin(Node<T>*& node) {
		Node<T>* min_node = nullptr;
		if ( !node->left ) {
			min_node = node;
			node = node->right;
			min_node->right = nullptr;
			return min_node;
		}
		min_node = GetDeleteMin(node->left);
		node = Rebalance_(node);
		return min_node;
	}
    void free_(Node<T>*& node) {
        if ( !node ) return;
		free_(node->left);
		free_(node->right);
		delete node;
    }

    Node<T>* root_;
    Comparator comp;
};

void run(std::istream &input, std::ostream &output) {
    AVLTree<int, Comparator<int> > avlTree;

    size_t amount = 0;
    input >> amount;

    int data = 0;
    int k = 0;
    for ( size_t iii = 0; iii < amount; ++iii ) {
        input >> data >> k;
        data >= 0 ? avlTree.Insert(data) : avlTree.Delete(abs(data));
        output << avlTree.GetKStat(k) << std::endl;
    }
}

void testLogic() {
    { // Условие из задачи
        std::stringstream input;
        std::stringstream output;

        input << "5\n"
                 "40 0\n"
                 "10 1\n"
                 "4 1\n"
                 "-10 0\n"
                 "50 2" << std::endl;

        run(input, output);

        assert(output.str() == "40\n40\n10\n4\n50\n");
    }
    std::cout << "OK" << std::endl;
}

int main() {

    testLogic();
//    run(std::cin, std::cout);

    return 0;
}
