// Copyright(c) 1996 Leendert Ammeraal. All rights reserved.
// This program text occurs in Chapter 7 of
//
//    Ammeraal, L. (1996) Algorithms and Data Structures in C++,
//       Chichester: John Wiley.

// btree: B-tree of order M
//  (with nodes that contain at most M links)
#include <iostream>
#include <iomanip>
#include <ctype.h>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#define M 3  // Order of B-tree: M link fields in each node

using namespace std;
typedef int dtype;

enum status {InsertNotComplete, Success, DuplicateKey,
   Underflow, NotFound};

struct node {
   int n;        // Number of items stored in a node (n < M)
   dtype k[M-1]; // Data items (only the first n in use)
   node *p[M];   // Pointers to other nodes (n+1 in use)
};

// Logical order:
//    p[0], k[0], p[1], k[1], ..., p[n-1], k[n-1], p[n]

class Btree {
public:
   Btree(): root(NULL){}
   void play();
   bool insert(dtype x);
   void print()const{cout << "Contents:\n"; pr(root, 0);}
   bool DelNode(dtype x);
   void ShowSearch(dtype x)const;

   int altura(node * root, int i);
   int numHojas(node * root, int i, int l);
   QString buscar(dtype x);
   void printTree(node * root, int x, int y);
   node *root;
   QGraphicsScene *scene;

private:
   status ins(node *r, dtype x, dtype &y, node* &u);
   void pr(const node* r, int nSpace)const;
   int NodeSearch(dtype x, const dtype *a, int n)const;
   status del(node *r, dtype x);
};

