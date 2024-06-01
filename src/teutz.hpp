#include<vector>
#include "graph.hpp"
#ifndef TEUTZ_H
#define TEUTZ_H

namespace std {
    struct TeutzBart {
        Graph g;
        vector<int> solution;
        TeutzBart(Graph _g): g(_g) {}

        /**
         * Número de transmissão dos vértices
         * trasmission[i] = Somatório(Demanda(j) * Distancia(i, j)) para todo j vértice de G
        */

        long long pMedian(int p) {
            vector<int> currentSolution(p);
            vector<int> toBeAnalysed(g.nVertices-p);
            vector<int> transmission(g.nVertices);

            for(int i=0; i<p; i++) currentSolution[i] = i;
            for(int i=0; i<g.nVertices-p; i++) toBeAnalysed[i] = p+i;

            g.floydWarshal();
            for(int i=0; i<g.nVertices; i++) {
                for(int j=0; j<g.nVertices; j++) {
                    transmission[i] += g.demand[j] * g.dist[i][j];
                }
            }

            while(!toBeAnalysed.empty()) {
                int diff = 0;
                int bestIndexOfSolution = -1, bestIndexOfToBeAnalysed = -1;
                for(int i=0; i<p; i++) {
                    for(int j=0; j<(int)toBeAnalysed.size(); j++) {
                        int x = currentSolution[i], y = toBeAnalysed[j];
                        if(transmission[x] - transmission[y] < diff) {
                            bestIndexOfSolution = i;
                            bestIndexOfToBeAnalysed = j;
                            diff = transmission[x] - transmission[y];
                        }
                    }
                }
                if(bestIndexOfSolution == -1) break;
                currentSolution[bestIndexOfSolution] = toBeAnalysed[bestIndexOfToBeAnalysed];
                toBeAnalysed.erase(toBeAnalysed.begin() + bestIndexOfToBeAnalysed);
            }

            solution = currentSolution;

            long long median = 0;
            for(int i=0; i<g.nVertices; i++) {
                // encontrar o vértice mais próximo de i que está na solução
                int bestVertex = solution[0];
                for(int j=1; j<p; j++) {
                    if(g.dist[i][solution[j]] < g.dist[i][bestVertex]) {
                        bestVertex = solution[j];
                    }
                }
                median += g.demand[i] * g.dist[i][bestVertex];
            }

            return median;
        }
    };
}
#endif