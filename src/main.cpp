#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <chrono>
#include "graph.hpp"
#include "teutz.hpp"
#include "correct.hpp"

const long long inf = 1e18;
const int CNT_TESTS = 20;

class Customer {
    public:
        int id, X, Y, demand;
        Customer(int id, int X, int Y, int demand): id(id), X(X), Y(Y), demand(demand) {};
        Customer() {}
};

void runTest(int testId) {
    std::ifstream file("data/input/" + std::to_string(testId) + ".txt");

    int problemNumber, optimalSolution, customers, medians, capacity;
    file >> problemNumber >> optimalSolution
                >> customers >> medians >> capacity;

    std::vector<Customer> c(customers);
    std::Graph g(customers);
    for(int i=0; i<customers; i++) {
        int id, cordX, cordY, demand;
        file >> id >> cordX >> cordY >> demand; 
        c[i] = Customer(id, cordX, cordY, demand);
    }

    for(int i=0; i<customers; i++) {
        g.setDemand(i, c[i].demand);
        for(int j=i+1; j<customers; j++) {
            long long dsquared = (c[i].X - c[j].X)*(c[i].X - c[j].X) + (c[i].Y - c[j].Y)*(c[i].Y - c[j].Y);
            long long dist = (long long)std::sqrt(dsquared);
            g.addEdge(c[i].id-1, c[j].id-1, dist);
        }
    }
    g.floydWarshal();

    std::TeutzBart heuristic(g);
    std::BruteForcePMedian correct(g);

    auto startOptimal = std::chrono::high_resolution_clock::now();
    long long optimal = correct.pMedian(medians);
    auto endOptimal = std::chrono::high_resolution_clock::now();
    auto durationOptimal = std::chrono::duration_cast<std::chrono::nanoseconds>(endOptimal - startOptimal).count();

    auto startHeuristic = std::chrono::high_resolution_clock::now();
    long long answer = heuristic.pMedian(medians);
    auto endHeuristic = std::chrono::high_resolution_clock::now();
    auto durationHeuristic = std::chrono::duration_cast<std::chrono::nanoseconds>(endHeuristic - startHeuristic).count();

    std::cout << "Test " << testId << " finished" << std::endl;
    std::cout << "Optimal solution: " << optimal << std::endl;
    std::cout << "Optimal points: ";
    for(int i=0; i<(int)correct.solution.size(); i++) {
        int id = correct.solution[i];
        int X = c[id].X, Y = c[id].Y;
        std::cout << '(' << X << ',' << Y << ')' << ' ';
    }
    std::cout << std::endl;
    std::cout << "Returned solution: " << answer << std::endl;
    std::cout << "Returned Points: ";
    for(int i=0; i<(int)heuristic.solution.size(); i++) {
        int id = heuristic.solution[i];
        int X = c[id].X, Y = c[id].Y;
        std::cout << '(' << X << ',' << Y << ')' << ' ';
    }
    std::cout << std::endl;

    std::cout << "Optimal solution execution time (ms): " << durationOptimal / 1e6 << std::endl;
    std::cout << "Heuristic solution execution time (ms): " << durationHeuristic / 1e6 << std::endl;
}

int main(int argc, char* argv[]) 
{
    if(argc == 1) {
        for(int i=1; i<=CNT_TESTS; i++) {
            runTest(i);
        }
    } else {
        runTest(std::stoi(argv[1]));
    }
    return 0;
}