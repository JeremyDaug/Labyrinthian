#ifndef NODE_H
#define NODE_H

class Point
{
    public:
    float x;
    float y;

	Point() : x(0), y(0) {}
    Point(float nx, float ny) : x(nx), y(ny) {}
};

class Node
{
	/*
	Node should always be filled out to level 0. If it isn't then something is probably wrong.
	*/
    private:
    // Parent of the Node
    Node* Parent;

    // Children Nodes
	// Notes: NE holds 0,0; +,0; and 0,+
	// NW holds -,0
	// SE holds 0,-
    Node* nw;
    Node* sw;
    Node* ne;
    Node* se;

    // position of the node
    Point pos;

    // Level of the node 0 is a full tile.
    int height;

    // Data in the node
	long data[5];
    // getters / Finders
    Node* getNodeAtPoint(Point targ);
	Node* getNodeAt(Point targ, int level);
    Point getPoint() {return pos;}

	// Bool Finder to see if it exists.
	bool isChild(Point targ);

	// filling constructor to recursively fill the tree.
	Node(Node* par, Point loc);

public:
    // constructor
    Node();
	// Demarked Constructor to allow quick creation up to a certain height
	// This should be used for the Head in the QuadTree Class.
	Node(int Height);
    // destructor
    ~Node();

    // Getters / finders
	  // Getters that try to find points specifically.
    long* getPointData(Point targ);
    Node* getNodeAtPoint(Point targ);
	  // Generic Finder for node at any level.
    Node* getNodeAt(Point targ, int level);

	int getHeight() { return height };

    // fillers to fill out the children beneath a node.
    void fillChildren();

    // setter functions
	bool setDataAtPoint(Point targ, long arr[]);
	bool setDataAtPointAndIndex(Point targ, long var, int index);
	// setter function
	bool setData(long arr[]);
	bool setDataAtIndex(long var, int index);



}
#endif // NODE_H
