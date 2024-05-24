#include<vector>
#include "graph.hpp"
#ifndef TEUTZ_H
#define TEUTZ_H

namespace std {
    struct TeutzBart {
        Graph g;
        /**
         * Número de transmissão dos vértices
         * trasmission[i] = Somatório(Demanda(j) * Distancia(i, j)) para todo j vértice de G
        */
        vector<long long> transmission; 
        vector<bool> analysed;
        vector<int> solution; // Vértices pertencentes à solução da P-mediana
        TeutzBart(Graph _g): g(_g) {
            g.floydWarshal();
            transmission.assign(g.nVertices, 0);
            analysed.assign(g.nVertices, false);
            for(int i=0; i<g.nVertices; i++) {
                for(int j=0; j<g.nVertices; j++) {
                    transmission[i] += _g.demand[j] * _g.dist[i][j];
                }
            }
        }

        long long pMedian(int p) {
            solution.resize(p);
            fill(analysed.begin(), analysed.end(), false);
            for(int i=0; i<p; i++) {
                analysed[i] = true;
                solution[i] = i;
            }

            while(1) {
                int next = -1;
                for(int i=0; i<g.nVertices; i++) {
                    if(!analysed[i]) {
                        next = i;
                        break;
                    }
                }

                if(next == -1) break;
                analysed[next] = true;

                int bestDifference = 0;
                int bestIndexToChange = -1;
                for(int i=0; i<p; i++) {
                    int difference = transmission[next] - transmission[solution[i]];
                    if(difference < bestDifference) {
                        bestDifference = difference;
                        bestIndexToChange = i;
                    }
                }

                if(bestIndexToChange != -1) {
                    solution[bestIndexToChange] = next;
                }
            }

            long long median = 0;

            for(int i=0; i<g.nVertices; i++) {
                // encontrar o vértice mais próximo de i que está na solução
                int bestVertex = solution[0];
                for(int j=1; j<p; j++) {
                    if(g.dist[i][solution[j]] < g.dist[i][bestVertex]) {
                        bestVertex = j;
                    }
                }
                median += g.demand[i] * g.dist[i][bestVertex];
            }

            return median;
        }
    };
}
#endif