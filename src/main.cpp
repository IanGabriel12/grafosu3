#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include "graph.hpp"
#include "teutz.hpp"

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
            long long dist = std::floor(std::sqrt(dsquared));
            g.addEdge(c[i].id-1, c[j].id-1, dist);
        }
    }
    g.floydWarshal();

    std::TeutzBart heuristic(g);
    int answer = heuristic.pMedian(medians);
    std::cout << "Test " << testId << " finished" << std::endl;
    std::cout << "Solution Points: ";
    for(int i=0; i<(int)heuristic.solution.size(); i++) {
        int id = heuristic.solution[i];
        int X = c[id].X, Y = c[id].Y;
        std::cout << '(' << X << ',' << Y << ')' << ' ';
    }
    std::cout << std::endl;
    std::cout << "Optimal solution: " << optimalSolution << std::endl;
    std::cout << "Returned: " << answer << std::endl;
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