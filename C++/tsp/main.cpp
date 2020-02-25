#include "tsp.h"

std::vector<int> tsp(std::vector<std::vector<double>> cost_matrix){
    TSP_solver tsp(cost_matrix);
    return tsp.solve_tsp();
}

int main(){
    ddvec cost_matrix{
    {INF, 10, 8, 19, 12},
    {10, INF, 20, 6, 3},
    {8, 20, INF, 4, 2},
    {19, 6, 4, INF, 7},
    {12, 3, 2, 7, INF}
    };

    std::vector<int> best_path = tsp(cost_matrix);
    return 0;
}
