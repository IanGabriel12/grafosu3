#include <iostream>

int main()
{
    int tests;
    std::cin >> tests;

    while (tests--)
    {
        int problemNumber, optimalSolution, customers, medians, capacity;

        std::cin >> problemNumber >> optimalSolution
                 >> customers >> medians >> capacity;

        while (customers--)
        {
            int id, cordX, cordY, demand;
            std::cin >> id >> cordX >> cordY >> demand; 
        }
    }

    return 0;
}