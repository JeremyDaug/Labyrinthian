#ifndef NODE_H
#define NODE_H


class Node
{
    public:
        /** Default constructor */
        Node();
        /** Default destructor */
        ~Node();
        /** Copy Constructor **/
        Node::Node(const Node& other);

        Node(Node* parent);

        Node* findNode(long x, long y);

        Node* findCell(long x, long y);

        int* getData();

        void setData(int data[5]);

        int* getDataFromTile(long x, long y);

        Node* destroyNode(Node* node);

        void destroyQ1() {Q1=destroyNode(Q1);}

        void destroyQ2() {Q2=destroyNode(Q2);}

        void destroyQ3() {Q3=destroyNode(Q3);}

        void destroyQ4() {Q4=destroyNode(Q4);}


    private:
        // The parent node of this node. If null then the node is the head.
        Node* m_parent;
        // The child nodes of this node. If null then the child does not exist. If all are null then this node is a leaf.
        Node* Q1, *Q2, *Q3, *Q4;
        // The coordinates of the node used for searching. Unless the height is 0 then this is actually a whole number, if it's not then it's the average of it's children
        long m_x, m_y;
        // The height of the node in the tree. 0 is a tile. 3 is a room. 6 is a Cell.
        int m_height;
        // The data (if any) in the tile. Has a slot for each thing that can occupy the room.
        int m_data[5];
};

#endif // NODE_H
