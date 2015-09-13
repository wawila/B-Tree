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
}*root = NULL, *np = NULL, *x = NULL;

class BPlus
{
public:
    BplusNode * init();
    void traverse(BplusNode *p);
    void sort(int *p, int n);
    int split_child(BplusNode *x, int i);
    void insert(int a);
    BPlus();
    ~BPlus();
};

#endif // BPLUS_H
