#ifndef NODE_H
#define NODE_H

struct Tile
{
    int x;
    int y;

    Tile(int nx, int ny) : x(nx), y(ny) {}
}

template <typename T>
struct Data
{
    Tile pos;
    T Data;

    Data(Tile position = Tile(), T* data = nullptr) : pos(position), Data(data) {}
}

template <typename T>
class Node
{
private:
    // The number of children this can have.
    const int NODE_CAPACITY = 4;

    // Parent of the Node, if the node is empty, then the node is the head.
    Node* Parent;

    // The Quadrants beneath the tree.
    Node* nw;
    Node* ne;
    Node* sw;
    Node* se;

    // The Data, if any, in the node.
    Data data;

    // Constructors
    Node();
    Node(const Node& Parent);
    
    // Destructor
    ~Node();
}

#endif // NODE_H
