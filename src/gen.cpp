#include <iostream>
#include <fstream>
int main(int argc, char* argv[]) 
{
    int tests; std::cin >> tests;
    for(int i=0; i<tests;i++) {
        std::ofstream file("data/input/" + std::to_string(i+1) + ".txt");
        std::ofstream bigfile("data/input/" + std::to_string(i+21) + ".txt");

        int problemNumber, optimalSolution, customers, medians, capacity;
        std::cin >> problemNumber >> optimalSolution
                >> customers >> medians >> capacity;

        file << problemNumber << ' ' << optimalSolution << '\n';
        file << std::min(customers, 20) << ' ' << medians << ' ' << capacity << '\n';

        bigfile << problemNumber << ' ' << optimalSolution << '\n';
        bigfile << customers << ' ' << medians << ' ' << capacity << '\n';

        for(int j=0; j<customers; j++) {
            int id, cordX, cordY, demand;
            std::cin >> id >> cordX >> cordY >> demand; 

            // Truncar testes
            if(j < 20) {
                file << id << ' ' << cordX << ' ' << cordY << ' ' << demand << '\n';
            }
            bigfile << id << ' ' << cordX << ' ' << cordY << ' ' << demand << '\n';
        }
        file.close();
        bigfile.close();
    }
}