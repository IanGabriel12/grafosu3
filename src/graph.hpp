#include<vector>
#ifndef GRAPH_STRUCT
#define GRAPH_STRUCT
namespace std {
    const long long inf = 1e18;
    struct Graph {
        vector<vector<long long>> edg; // Matriz de adjascências
        vector<vector<long long>> dist; // Matriz de distâncias
        vector<long long> demand; // Demanda dos vértices
        int nVertices;

        // Os vértices estão 0-indexados
        Graph(int n) : nVertices(n) {
            edg.assign(n, vector<long long>(n, inf));
            dist.assign(n, vector<long long>(n, inf));
            demand.assign(n, 0);
        }
        void addEdge(int a, int b, long long w) {
            edg[a][b] = w;
            edg[b][a] = w;
        }
        void setDemand(int v, int d) {
            demand[v] = d;
        }
        void floydWarshal() {
            dist = edg;
            for(int i=0; i<nVertices; i++) {
                dist[i][i] = 0;
            }
            for(int k=0; k<nVertices; k++) {
                for(int i=0; i<nVertices; i++) {
                    for(int j=0; j<nVertices; j++) {
                        if(dist[i][k] == inf || dist[k][j] == inf) continue;
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }
    };
}
#endif