#ifndef fibonacci_hpp
#define fibonacci_hpp
#pragma once
#include <iostream>
const int MAX = 1001;
class Node{
public:
    int key;
    int degree;
    Node *child;
    Node *parent;
    Node *left;
    Node *right;
    bool mark;
    int vertex;
};
class FibonacciHeap{ 
public:
    FibonacciHeap() : Num(0), Cnt(0), Min(nullptr){};
    int Num;
    int Cnt;
    Node* Min;
    Node* Insert(int newKey);
    void Link(Node *y, Node *x);
    void Consolidate(void);
    Node *ExtractMin(void);
    Node *Minimum(void);
    void DecreaseKey(Node *x, int newKey);
    void Delete(Node *x);
    void Cut(Node *x, Node *y);
    void CascadingCut(Node *y);

};
FibonacciHeap HeapUnion(FibonacciHeap H1, FibonacciHeap H2);
#endif