#include<vector>
#include<bit>
#include "graph.hpp"
#ifndef CORRECT_H
#define CORRECT_H

namespace std {
    struct BruteForcePMedian {
        Graph g;
        vector<int> solution;
        BruteForcePMedian(Graph _g): g(_g) {
            
        }

        long long pMedian(int p) {
            g.floydWarshal();
            vector<int> currentSolution(p);
            vector<int> bestSolution(p);
            long long bestMedian = inf;
            for(int mask=0; mask<(1 << g.nVertices); mask++) {
                if(std::__popcount(mask) != p) continue;
                int cnt = 0;
                for(int i=0; i<g.nVertices; i++) {
                    bool isSet = (mask >> i) & 1;
                    if(!isSet) continue;
                    currentSolution[cnt++] = i;
                }

                long long median = 0;
                for(int i=0; i<g.nVertices; i++) {
                    int best = currentSolution[0];
                    for(int j=1; j<p; j++) {
                        if(g.dist[i][currentSolution[j]] < g.dist[i][best]) {
                            best = currentSolution[j];
                        }
                    }
                    median += g.demand[i] * g.dist[i][best];
                }
                if(median < bestMedian) {
                    bestSolution = currentSolution;
                    bestMedian = median;
                }
            }

            solution = bestSolution;
            return bestMedian;
        }
    };
}
#endif