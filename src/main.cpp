#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "graph.hpp"
#include "teutz.hpp"
#include "correct.hpp"

const long long inf = 1e18;
const int CNT_TESTS = 40;
const int BIG_MIN = 21;
const int BIG_MAX = 40;
typedef std::chrono::_V2::system_clock::rep  duration_t;

class Customer {
    public:
        int id, X, Y, demand;
        Customer(int id, int X, int Y, int demand): id(id), X(X), Y(Y), demand(demand) {};
        Customer() {}
};

class Result {
    public:
        long long optimalSolution, heuristicSolution, vertices, testId;
        duration_t optimalTime, heuristicTime;
        Result(
            long long a, long long b, duration_t c, duration_t d, long long v, long long id
        ) : optimalSolution(a), heuristicSolution(b), vertices(v), optimalTime(c), heuristicTime(d), testId(id) {}
        Result() {}
};

bool isBigTest(int testId) {
    return BIG_MIN <= testId && testId <= BIG_MAX;
}

Result runTest(int testId) {
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

    auto startOptimal = std::chrono::high_resolution_clock::now();
    long long optimal;
    std::BruteForcePMedian correct(g);
    if(!isBigTest(testId)) {
        optimal = correct.pMedian(medians);
    }
    auto endOptimal = std::chrono::high_resolution_clock::now();
    auto durationOptimal = std::chrono::duration_cast<std::chrono::nanoseconds>(endOptimal - startOptimal).count();

    auto startHeuristic = std::chrono::high_resolution_clock::now();
    std::TeutzBart heuristic(g);
    long long answer = heuristic.pMedian(medians);
    auto endHeuristic = std::chrono::high_resolution_clock::now();
    auto durationHeuristic = std::chrono::duration_cast<std::chrono::nanoseconds>(endHeuristic - startHeuristic).count();

    std::cout << "Test " << testId << " finished" << std::endl;
    if(!isBigTest(testId)) {
        std::cout << "Optimal solution: " << optimal << std::endl;
        std::cout << "Optimal points: ";
        for(int i=0; i<(int)correct.solution.size(); i++) {
            int id = correct.solution[i];
            int X = c[id].X, Y = c[id].Y;
            std::cout << '(' << X << ',' << Y << ')' << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "Returned solution: " << answer << std::endl;
    std::cout << "Returned Points: ";
    for(int i=0; i<(int)heuristic.solution.size(); i++) {
        int id = heuristic.solution[i];
        int X = c[id].X, Y = c[id].Y;
        std::cout << '(' << X << ',' << Y << ')' << ' ';
    }
    std::cout << std::endl;

    std::cout << "Optimal solution execution time (ms): " << (double) durationOptimal / 1e6 << std::endl;
    std::cout << "Heuristic solution execution time (ms): " << (double) durationHeuristic / 1e6 << std::endl;

    file.close();
    return Result(optimal, answer, durationOptimal, durationHeuristic, customers, testId);
}



void writeResults(std::vector<Result> &results) {
    std::ofstream file("data/output/results.txt");
    std::ofstream bigfile("data/output/bigresults.txt");

    file << "# N OptimalAnswer HeuristicAnswer Diff(%) OptimalTime(ms) HeuristicTime(ms)\n";
    file << std::setprecision(2) << std::fixed; 

    bigfile << "# N HeuristicAnswer HeuristicTime(ms)\n";
    bigfile << std::setprecision(2) << std::fixed; 

    for(int i=0; i<(int)results.size(); i++) {
        Result r = results[i];
        
        if(isBigTest(r.testId)) {
            bigfile << r.testId << ' ' << r.vertices << ' ' << r.heuristicSolution << ' ';
            bigfile << (double) r.heuristicTime / 1e6;
            bigfile << '\n';
        } else {
            file << r.testId << ' ' <<  r.vertices << ' ' << r.optimalSolution << ' ' << r.heuristicSolution << ' ';
            double diff = (double) abs(r.optimalSolution - r.heuristicSolution);
            file << (100.0 * (diff) / (double) r.optimalSolution) << ' ';
            file << (double) r.optimalTime / 1e6 << ' ' << (double) r.heuristicTime / 1e6;
            file << '\n';
        }
    }
    file.close();
    bigfile.close();
}
int main(int argc, char* argv[]) 
{

    std::cout << "1-Rodar todos os testes\n";
    std::cout << "2-Rodar testes avulsos\n";
    std::cout << "3-Rodar testes em um intervalo\n";

    int option;
    while(true) {
        std::cout << "Digite a opção:";
        std::cin >> option;
        if(option < 0 || option > 3) {
            std::cout << "\nOpção inválida\n";
        } else {
            break;
        }
    }

    if(option == 1) {
        std::vector<Result> results;
        for(int i=1; i<=CNT_TESTS; i++) {
            results.push_back(runTest(i));
        }
        writeResults(results);
    } else if (option == 2) {
        std::vector<Result> results;
        std::string input;
        std::cout << "Digite os números dos testes separados por espaço: ";
        std::cin.ignore();
        std::getline(std::cin, input);

        std::istringstream iss(input);
        int testId;
        while(iss >> testId) {
            results.push_back(runTest(testId));
        }
        writeResults(results);
    } else {
        int l, r;
        std::cout << "Digite os extremos do range [l, r]:";
        std::cin >> l >> r;
        std::vector<Result> results;
        for(int i=l; i<=r; i++) {
            results.push_back(runTest(i));
        }
        writeResults(results);
    }
    return 0;
}