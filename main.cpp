#include <cassert>
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

// Don't use size_t for now in the vector
using index_type = int;
// Just use numbers for now
using data_type = int;


// annoying to type...
inline static size_t
to_size_t(index_type index)
{
    return static_cast<size_t>(index); 
}


struct Node
{
    data_type data_;
    index_type left_child_index_ = 0;
    index_type right_child_index_ = 0;

    // TODO: does this work with non-POD types?
    Node(data_type data, index_type left_child_index, index_type right_child_index):
        data_(data),
        left_child_index_(left_child_index),
        right_child_index_(right_child_index)
    {
    }
};


std::ostream &operator<<(std::ostream &os, Node const &node) {
    return os << "Node(" << node.data_ << ", " << node.left_child_index_ << ", " << node.right_child_index_ << ")";
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
        nodes_.emplace_back(data, 0, 0);
        index_type node_index = static_cast<index_type>(nodes_.size() - 1);
        switch (direction) {
            case Direction::LEFT: nodes_[to_size_t(parent_index)].left_child_index_ = node_index; break;
            case Direction::RIGHT: nodes_[to_size_t(parent_index)].right_child_index_ = node_index; break;
        }
        return node_index;
    }

    BinaryTree(data_type data)
    {
        nodes_.emplace_back(data, 0, 0);
    }

    // Provide a default constructor so my BinarySearchTree can use it
    BinaryTree()
    {
    }

    void print_as_tree(std::ostream &out_stream, index_type current_index=0, int indent=0)
    {
        assert(nodes_.size() > 0 && "Need a non-empty tree!");

        Node current_node = nodes_[to_size_t(current_index)];
        for(int i = 0; i < indent; ++i)
        {
            out_stream << "  ";
        }
        out_stream << current_node << "\n";
        if (current_node.left_child_index_)
        {
            print_as_tree(out_stream, current_node.left_child_index_, indent + 1);
        }
        else
        {
            for(int i = 0; i < indent + 1; ++i)
            {
                out_stream << "  ";
            }
            out_stream << "No left child\n";
        }
        if (current_node.right_child_index_)
        {
            print_as_tree(out_stream, current_node.right_child_index_, indent + 1);
        }
        else
        {
            for(int i = 0; i < indent + 1; ++i)
            {
                out_stream << "  ";
            }
            out_stream << "No right child\n";
        }
    }

    void print_as_tree()
    {
        print_as_tree(std::cout);
    }

    void print_as_vector(std::ostream &out_stream)
    {
        assert(nodes_.size() > 0 && "Need a non-empty tree!");

        index_type size = static_cast<index_type>(nodes_.size());
        for(index_type i = 0; i < size; ++i)
        {
            out_stream << nodes_[to_size_t(i)] << "\n";
        }
    }
    void print_as_vector()
    {
        print_as_vector(std::cout);
    }

    void print_as_dot(std::ostream &out_stream)
    {
        assert(nodes_.size() > 0 && "Need a non-empty tree!");

        out_stream << "digraph my_graph {\n";

        index_type size = static_cast<index_type>(nodes_.size());
        for(index_type i = 0; i < size; ++i)
        {
            auto node = nodes_[to_size_t(i)] ;
            out_stream << "  node_" << i << "[label=" <<  node.data_ << "];\n";
            if (node.left_child_index_)
            {
                out_stream << "  node_" << i << " -> " << "node_" << node.left_child_index_ << "[label=" << node.left_child_index_ << "];\n";
            }
            else
            {
                out_stream << "  null_left_" << i << "[shape=point];\n";
                out_stream << "  node_" << i << " -> null_left_" << i << ";\n";
            }
            if (node.right_child_index_)
            {
                out_stream << "  node_" << i << " -> " << "node_" << node.right_child_index_ << "[label=" << node.right_child_index_ << "];\n";
            }
            else
            {
                out_stream << "  null_right_" << i << "[shape=point];\n";
                out_stream << "  node_" << i << " -> null_right_" << i << ";\n";
            }
        }
        out_stream << "}\n";
    }
    void print_as_dot()
    {
        print_as_dot(std::cout);
    }
};

// TODO: make the print methods const so I can make this const
// NOTE: This only works on OSX with imgcat installed. That's why it's not a
// method
void imgcat_tree(BinaryTree& bt, const std::string& file_name)
{
    std::cout << "# Saving dot to " << file_name << "\n";

    // Do this in it's own scope to ensure the file is written
    // before calling dot on it
    {
        std::ofstream fout(file_name);
        bt.print_as_dot(fout);
    }

    // Isn' string-building fun? Just shoot me in the cabeza
    std::string dot_cmd = "dot -Tpng -O ";
    dot_cmd += file_name;
    std::cout << "# " << dot_cmd << "\n";
    system(dot_cmd.c_str());

    std::string imgcat_cmd = "imgcat ";
    imgcat_cmd += (file_name + ".png");
    std::cout << "# " <<  imgcat_cmd << "\n";
    system(imgcat_cmd.c_str());
}


// Let's make a binary search tree (append only for now) (we'll get into balancing it later)

// This is non-balancing
struct BinarySearchTree
{
    BinaryTree bt_;

    BinarySearchTree(data_type data): bt_(data)
    {
    }

    // TODO: assume that bt_ isn't empty
    // Assume we're starting at the root
    index_type insert(data_type data, index_type current_index=0)
    {
        Node current_node = bt_.nodes_[to_size_t(current_index)];
        if (current_node.data_ == data) { return current_index; }
        if (data < current_node.data_)
        {
            if (current_node.left_child_index_)
            {
                return insert(data, current_node.left_child_index_);
            }
            index_type new_index = bt_.insert_and_get_index(current_index, BinaryTree::Direction::LEFT, data);
            return new_index;
        }
        else // (data > current_node.data_)
        {
            if (current_node.right_child_index_)
            {
                return insert(data, current_node.right_child_index_);
            }
            index_type new_index = bt_.insert_and_get_index(current_index, BinaryTree::Direction::RIGHT, data);
            return new_index;
        }
    }
};



#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
void test_tree()
{
    BinaryTree bt(1);
    using D = BinaryTree::Direction;
    auto root_r = bt.insert_and_get_index(bt.root_index, D::RIGHT, 4);
    auto root_l = bt.insert_and_get_index(bt.root_index, D::LEFT, 2);
    auto root_l_r = bt.insert_and_get_index(root_l, D::RIGHT, 3);

    std::cout << "# Printing as Vector:\n";
    bt.print_as_vector(std::cout);
    std::cout << "# Printing as Tree:\n";
    bt.print_as_tree(std::cout);

    imgcat_tree(bt, "tmp1.dot");
}

void test_binary_search_tree()
{
    BinarySearchTree bst(0);
    imgcat_tree(bst.bt_, "tmp.dot");
    auto data = {
        1,
        3,
        2,
        0,
        -10,
        4,
        -5
    };
    for(auto i: data)
    {
        bst.insert(i);
        imgcat_tree(bst.bt_, "tmp.dot");
    }

}

#pragma clang diagnostic pop

int main()
{
    test_binary_search_tree();
    // test_tree();
}
