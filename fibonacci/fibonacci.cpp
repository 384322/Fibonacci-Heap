#include "fibonacci.hpp"
#include <iostream>
#include <cmath>
#include <vector>
const int INF = 2100000000;

Node* FibonacciHeap::Insert(int newKey){
    Node *x = new Node;
    x->key = newKey;
    x->degree = 0;
    x->parent = nullptr;
    x->child = nullptr;
    x->mark = false;
    x->right = x;
    x->left = x;
    x->vertex = 0;
    if(this->Min == nullptr){ // 힙이 비어있는 경우
        this->Min = x;
    }
    else{
        x->left = this->Min;
        x->right = this->Min->right;
        this->Min->right->left = x;
        this->Min->right = x;
        if(x->key < this->Min->key){ //새로 삽입된 원소가 최소일 경우
            this->Min = x;
        }
    }
    this->Num++; //힙 전체의 개수 증가
    this->Cnt++; //새로운 원소 삽입 시 루트 개수 증가
    return x;
}

void FibonacciHeap::Link(Node *y, Node *x){
    y->left->right = y->right;
    y->right->left = y->left;
    y->parent = x;
    if(y == x){
        return;
    }
    if(x->child == nullptr){
        y->left = y;
        y->right = y;
        x->child = y;
    }
    else{
        y->right = x->child->right;
        y->left = x->child;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    this->Cnt--; //링크 연산 시 루트에 원소 한개 삭제
    // std::cout << "Link y:" << " " << y->key << "Link x:" << " " << x->key << "\n";

}

void FibonacciHeap::Consolidate(void){
    int D = (int)(log(this->Num)/log(2));
    D = (int)MAX/log(2);
    int i;
    std::vector<Node *> A(D+1, nullptr);
    Node *w = this->Min;
    int rootcnt = 0; //몇번이나 돌았는지
    int tmpcnt = this->Cnt; //현재 루트의 노드 개수만큼
    do{
        rootcnt++;
        Node *x = w;
        w = w->right; //다음 노드를 미리 기억해놔야 삭제 후에도 다음 노드를 순차적으로 확인
        int d = x->degree;
        while(A[d] != nullptr){
            Node *y = A[d];
            if(x->key > y->key){
                Node *tmp = y;
                y = x;
                x = tmp;
            }
            Link(y, x);
            A[d] = nullptr;
            d++;
        }
        A[d] = x;
    }while(rootcnt < tmpcnt);
    this->Min = nullptr;

    for(i=0;i<D+1;i++){
        if(A[i] != nullptr){
            if(this->Min == nullptr || A[i]->key < this->Min->key){
                this->Min = A[i];
            }
        }
    }
}

Node *FibonacciHeap::Minimum(void){
    Node *w = this->Min;
    Node *x = w;
    for(int i= 0;i<this->Num;i++){
        if(x->key < this->Min->key){
            this->Min = x;
        }
        x = x->right;
    }
    return this->Min;
}


Node* FibonacciHeap::ExtractMin(void){
    Node *z = this->Min;
    if(z == nullptr){
        return nullptr;
    }
    else{
        if(z->child != nullptr){
            Node *w = z->child;
            do{
                this->Cnt++; //삭제하는 원소의 자식의 개수만큼 루트에 추가
                Node *x = w;
                w = w->right;  //w를 오른쪽으로 보내놓고 x 삭제

                x->right = z->right;
                x->left = z;
                z->right->left = x;
                z->right = x;
                if(x->key < z->key){
                    this->Min = x;
                }
                x->parent = nullptr;
            }while(w != z->child); //마지막 x삭제한 후 w가 z->child
        }
        z->left->right = z->right; //z만 원소로 존재하면 z를 가리킴, 그 외에는 z삭제
        z->right->left = z->left;
        z->child = nullptr;
        this->Cnt--; //Min원소 삭제
        if(z->right == z){
            this->Min = nullptr;
        }
        else{
            this->Min = z->right;
            Consolidate();
        }
        this->Num--;
        return z;
    }
}

void FibonacciHeap::DecreaseKey(Node *x, int newKey){
    if(newKey > x->key){
        std::cout <<"new key is greater than current key" << "\n";
        return;
    }
    x->key = newKey;
    Node *y = x->parent;
    if(y != nullptr && x->key < y->key){
        Cut(x, y);
        CascadingCut(y);
    }
    if(x->key < this->Min->key){
        this->Min = x;
    }
}

void FibonacciHeap::Cut(Node *x, Node *y){
    y->degree--; //부모 차수 감소

    x->right->left = x->left;
    x->left->right = x->right; //y의 child에서 삭제
    
    if(x->right == x){ // 자식이 한개인 경우
        y->child = nullptr;
    }
    else{ //자식이 둘 이상인 경우
        y->child = x->right; 
    }

    x->right = this->Min->right;
    x->left = this->Min;
    this->Min->right->left = x;
    this->Min->right = x; //루트에 x 삽입
    this->Cnt++; //루트 원소 개수 증가
    
    x->parent = nullptr;
    x->mark = false;
}

void FibonacciHeap::CascadingCut(Node *y){
    Node *z = y->parent;
    if(z != nullptr){
        if(y->mark == false){
            y->mark = true;
        }
        else{
            Cut(y, z);
            CascadingCut(z);
        }
    }
}

void FibonacciHeap::Delete(Node *x){
    DecreaseKey(x, -INF);
    Node* del = this->ExtractMin();
    delete del;
}


FibonacciHeap HeapUnion(FibonacciHeap H1, FibonacciHeap H2){
    FibonacciHeap H;
    H.Min = H1.Min;
    H2.Min->left->right = H.Min->right;
    H.Min->right->left = H2.Min->left;
    H2.Min->left = H.Min;
    H.Min->right = H2.Min; //concatenate H1, H2
    if(H1.Min == nullptr || (H2.Min != nullptr && H2.Min->key < H1.Min->key)){
        H.Min= H2.Min;
    }
    H.Num = H1.Num + H2.Num;
    H.Cnt = H1.Cnt + H2.Cnt;
    return H;
}