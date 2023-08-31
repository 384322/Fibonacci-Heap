#include "fibonacci.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
const int INF = 2100000000;
// const int MAX = 1001;

using namespace std;

vector<vector<pair<int, int> > > G(MAX);
vector<Node *> Ptr(MAX, nullptr);
vector<int> Dist(MAX, INF);
FibonacciHeap H;

void makeGraph(int u, int v, int w){
    G[u].push_back(make_pair(v, w));
}

void readGraph(int source, int u, int v, int w){
    if(v != source){ //source 의 indegree는 0으로 초기화
        if(u == source){
            if(Ptr[v] == nullptr){
                Ptr[v] = H.Insert(w);
                Dist[v] = w;
                Ptr[v]->vertex = v;
            }
            else{
                if(w < Ptr[v]->key){
                    H.DecreaseKey(Ptr[v],w);
                    Dist[v] = w;
                }
            }
        }
        else{
            if(Ptr[v] == nullptr){
                Ptr[v] = H.Insert(INF);
                Dist[v] = INF;
                Ptr[v]->vertex = v;
            }
        }
    }
    else{
        Ptr[v] = nullptr;
        Dist[v] = 0;
    }
}

void Dijkstra(int source){
    while(H.Min != nullptr){
        Node *Now = H.ExtractMin();
        int u = Now->vertex;
        for(int i=0;i<G[u].size();i++){
            int v = G[u][i].first;
            int w = G[u][i].second;
            int tmp = Dist[u] + w;
            if(Ptr[v] != nullptr){
                if(tmp < Dist[v]){
                    H.DecreaseKey(Ptr[v], tmp);
                    Dist[v] = tmp;
                }
            }
        }
    }
}



int main(void){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int V, E;
    int i, j;
    int u, v, w;
    int source, sink;
    cin>> V;
    cin>> E;
    for(i=0;i<E;i++){
        cin >> u >> v >> w;
        makeGraph(u, v, w);
    }
    cin >> source >> sink;
    Dist[source] = 0;
    for(i=1;i<=V;i++){
        for(j=0;j<G[i].size();j++){
            readGraph(source, i, G[i][j].first, G[i][j].second);
        }
    }
    Dijkstra(source);
    cout << Dist[sink] << "\n";
}