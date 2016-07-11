#include "Node.h"
#include<math.h>

Node::Node()
{
    Parent = nullptr;
    nw = nullptr;
    ne = nullptr;
    sw = nullptr;
    se = nullptr;
    data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
    height = 0;
    pos = Point(0, 0);
}



Node::Node(Node* par, Point loc)
{
    Parent = par;
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	height = par->getHeight()-1;

	nw = nullptr;
	ne = nullptr;
	sw = nullptr;
	se = nullptr;
}

Node::Node(int Height=0)
{
	// if height is at or lower then cell level, raise it to above cell level.
	if (Height < 4) Height = 4;
	height = Height;
	Parent = nullptr;
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 0;
	pos = Point(-0.5, -0.5);

	// create the children
	int maxBound = pow(2, Height) - 1;
	int minBound = -pow(2, Height);
	Point tne = Point(maxBound, maxBound);
	Point tnw = Point(minBound, maxBound);
	Point tsw = Point(minBound, minBound);
	Point tse = Point(maxBound, minBound);
	ne = new Node(Parent, tne);
}

Node::~Node()
{
    Parent = nullptr;
    delete nw;
    nw = nullptr;
    delete ne;
    ne = nullptr;
    delete sw;
    sw = nullptr;
    delete se;
    se = nullptr;
}

long * Node::getPointData(Point targ)
{
	return getNodeAtPoint(targ)->data;
}

Node * Node::getNodeAtPoint(Point targ)
{
	// delegate off to getNodeAt with Level set to 0
	return getNodeAt(targ, 0);
}


Node* Node::getNodeAt(Point targ, int level)
{
	if (height == level)
	{
		if (isChild(targ)) return this;
	}
	// search through the tree following the position
	// also checking if the child exists.
	if (targ.x > pos.x && targ.y > pos.y && ne)
	{
		// NE
		return ne->getNodeAt(targ, level);
	}
	else if (targ.x < pos.x && targ.y > pos.y && nw)
	{
		// NW
		return nw->getNodeAt(targ, level);
	}
	else if (targ.x < pos.x && targ.y < pos.y && sw)
	{
		// SW
		return sw->getNodeAt(targ, level);
	}
	else if (targ.x > pos.x && targ.y < pos.y && se)
	{
		// SE
		return se->getNodeAt(targ, level);
	}

	// if no children, return nullptr and be sure to catch it.
	return nullptr;
}

bool Node::isChild(Point targ)
{
	if (targ.x == pos.x && targ.y == pos.y)
		return true; // found the node
	if (targ.x > pos.x && targ.y > pos.y && ne)
	{
		// NE
		return ne->isChild(targ);
	}
	else if (targ.x < pos.x && targ.y > pos.y && nw)
	{
		// NW
		return nw->isChild(targ);
	}
	else if (targ.x < pos.x && targ.y < pos.y && sw)
	{
		// SW
		return sw->isChild(targ);
	}
	else if (targ.x > pos.x && targ.y < pos.y && se)
	{
		// SE
		return se->isChild(targ);
	}

	// if we get here then there are no children, we are at height 0 and must return false.
	return false;
}

bool Node::setData(long arr[])
{
	for (int i = 0; i < 5; ++i)
	{
		data[i] = arr[i];
	}
	return true;
}

bool Node::setDataAtPointAndIndex(Point targ, long var, int index)
{
	Node* check = getNodeAtPoint(targ);
	if (check)
		return check->setDataAtIndex(var, index);
	else
		return false;
}

bool Node::setDataAtIndex(long var, int index)
{
	data[index] = var;
	return true;
}

void Node::fillChildren()
{
	if (height == 0)
		return;

	ne = new Node();
}

bool Node::setDataAtPoint(Point targ, long arr[])
{
	Node* check = getNodeAtPoint(targ);
	if (check)
		return check->setData(arr);
	else
		return false;
}
