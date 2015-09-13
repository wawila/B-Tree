#ifndef BPLUS_H
#define BPLUS_H

#include<stdio.h>
#include<conio.h>
#include<iostream>

using namespace std;

struct BplusNode
{
    int *data;
    BplusNode **child_ptr;
    bool leaf;
    int n;
};

class BPlus
{
public:
    BplusNode *root = NULL, *np = NULL, *x = NULL;
    BplusNode * init();
    void traverse(BplusNode *p);
    void sort(int *p, int n);
    int split_child(BplusNode *x, int i);
    void insert(int a);
    void play();
    BPlus();
    ~BPlus();
};

#endif // BPLUS_H
