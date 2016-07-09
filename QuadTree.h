#ifndef QUADTREE_H
#define QUADTREE_H

#include "Node.h"

class QuadTree
{
    public:
        /** Default constructor */
        QuadTree();
        /** Default destructor */
        ~QuadTree();

        int[] getDataFromTile(long id);

        Node* getCell(long x, long y);

        Node* getRoom(long x, long y);

        Node* getTile(long x, long y);

    private:
        Node* m_head; //!< Member variable "m_head"
        Node* m_focus; //!< Member variable "m_focus"
        Node* m_focusCell; //!< Member variable "m_focusCell"
        int m_headLevel;
};

#endif // QUADTREE_H
