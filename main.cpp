#include <cassert>
#include <ostream>
#include <iostream>
#include <vector>

// Don't use size_t for now in the vector
using index_type = int;
// Just use numbers for now
using data_type = int;


struct Node
{
    data_type data;
    index_type left_child_index = 0;
    index_type right_child_index = 0;

    Node(data_type data, index_type left_child_index, index_type right_child_index):
        data(data), left_child_index(left_child_index), right_child_index(right_child_index)
    {
    }
};


std::ostream &operator<<(std::ostream &os, Node const &node) {
    return os << "Node(" << node.data << ", " << node.left_child_index << ", " << node.right_child_index << ")";
}


struct BinaryTree
{
    // I'll be using indices instead of pointers to point to children because
    // the vector may decide to reallocate on growth, which would invalidate
    // pointers. Putting all the nodes in a vector is also more cache friendly
    // and means I don't have to think about ownership issues
    //
    // Also using the convenction that member variables end with `_`
    //
    // NOTES:
    // The root node is always at index 0, and by definition, no children can
    // point to it.  This lets me use 0 to mean "no children" as a sentinal
    // value for left and right


    enum class Direction {
        LEFT,
        RIGHT
    };


    std::vector<Node> nodes_;
    // Just go ahead and make this a const thing
    const index_type root_index = 0;

    // TODO: for now, always insert at the back of the vector. Don't worry about removing nodes just yet...
    index_type insert_and_get_index(index_type parent_index, Direction direction, data_type data)
    {
        nodes_.push_back(Node(data, 0, 0));
        index_type node_index = nodes_.size() - 1;
        switch (direction) {
            case Direction::LEFT: nodes_[parent_index].left_child_index = node_index; break;
            case Direction::RIGHT: nodes_[parent_index].right_child_index = node_index; break;
            default: assert( direction != Direction::LEFT && direction != Direction::RIGHT && "Wrong Direction");
        }
        return node_index;
    }

    BinaryTree(data_type data)
    {
        nodes_.push_back(Node(data, 0, 0));
    }

    void print_as_tree(index_type current_index=0, int indent=0)
    {
        assert(nodes_.size() > 0 && "Need a non-empty tree!");

        Node current_node = nodes_[current_index];
        for(int i = 0; i < indent; ++i)
        {
            std::cout << "  ";
        }
        std::cout << current_node << "\n";
        if (current_node.left_child_index)
        {
            print_as_tree(current_node.left_child_index, indent + 1);
        }
        if (current_node.right_child_index)
        {
            print_as_tree(current_node.right_child_index, indent + 1);
        }
    }

    void print_as_vector()
    {
        assert(nodes_.size() > 0 && "Need a non-empty tree!");

        index_type size = static_cast<index_type>(nodes_.size());
        for(index_type i = 0; i < size; ++i)
        {
            std::cout << nodes_[i] << "\n";
        }
    }
};


void test_tree()
{
    BinaryTree bt(1);
    auto root_l = bt.insert_and_get_index(bt.root_index, BinaryTree::Direction::LEFT, 2);
    std::cout << "Printing as Vector:\n";
    bt.print_as_vector();
    std::cout << "Printing as Tree:\n";
    bt.print_as_tree();
}


int main()
{
    test_tree();
}
