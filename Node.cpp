#include "Node.h"

#define NULL nullptr

Node::Node()
{
    m_parent = NULL;
    Q1 = NULL;
    Q2 = NULL;
    Q3 = NULL;
    Q4 = NULL;
    for( int i=0; i<5; ++i)
        m_data[i] = 0;
}

Node::Node(Node* parent)
{
    m_parent = parent;
    Q1 = NULL;
    Q2 = NULL;
    Q3 = NULL;
    Q4 = NULL;
    for( int i=0; i<5; ++i)
        m_data[i] = 0;
}

Node::~Node()
{
    destroyNode(this);
}

Node* Node::destroyNode(Node* node)
{
    if(node)
    {
        destroyNode(node->Q1);
        destroyNode(node->Q2);
        destroyNode(node->Q3);
        destroyNode(node->Q4);
        delete node;
    }
    return NULL;
}

Node::Node(const Node& other)
{
    m_parent = other.m_parent;
    Q1 = other.Q1;
    Q1 = other.Q2;
    Q1 = other.Q3;
    Q1 = other.Q4;
    for(int i = 0; i < 5; ++i)
    {
        m_data[i] = other.m_data[i];
    }
}

Node* Node::findNode(long x, long y)
{
    if(m_height == 0)
    {
        if(m_x == x && m_y == y)
        {
            return this;
        }
    }
    else
    {
        if(m_x+.5 < x && m_y+.5 < y)
        {
            if(Q1)
                return Q1->findNode(x, y);
            else
                return NULL;
        }
        else if(m_x+.5 > x && m_y+.5 < y)
        {
            if(Q2)
                return Q2->findNode(x, y);
            else
                return NULL;
        }
        else if(m_x+.5 > x && m_y+.5 > y)
        {
            if(Q3)
                return Q3->findNode(x, y);
            else
                return NULL;
        }
        else if(m_x+.5 < x && m_y+.5 > y)
        {
            if(Q4)
                return Q4->findNode(x, y);
            else
                return NULL;
        }
    }
}

Node* Node::findCell(long x, long y)
{

}

int* Node::getData()
{

}

void Node::setData(int data[5])
{

}

int* Node::getDataFromTile(long x, long y)
{

}
