#ifndef TSP_H
#define TSP_H

//THIS HEADER IS ALSO A DOCUMENTATION
//Solving travelling salesman problem using Little's Algorithm

//INCLUDES
#include <iostream>
#include <cmath>
#include <functional>
#include <vector>
#include <cfloat>
#include <map>
#include <algorithm>
#include <iomanip>

const double INF = DBL_MAX;

//all aliases used in code
using dvec = std::vector<double>;
using ivec = std::vector<int>;
using iivec = std::vector<std::vector<int> >;
using ddvec = std::vector<std::vector<double> >;
using ivec_int_map = std::map<ivec, int>;

//class definition
class TSP_solver{
public:
    TSP_solver(ddvec cost_m):
         cost_matrix(cost_m), lb(0) {}

    //printing functions
    void print_cost_m(); //cost matrix
    void print_ddvec(const ddvec& matrix); //m x n matrix
    void print_map_pass_m(); //possible passages with their costs
    void print_ivec(const ivec& vec); //vector<int>

    /*Finds minimum values in every row, adds it to lb
    and subtracts those values from every element in every row except INF*/
    void lb_rows();

    /*Does the same as lb_rows, except on columns*/
    void lb_cols();

    /*Checks all 0s in matrix and saves all possible passages
    in form of (x1,x2) matrix which is x1->x2 pass
    then it cuts out x1 row and x2 column and forbids x2->x1 pass*/
    void check_pass();

    /*Calculates min values in given row, checks simultaneously column
    and row without the element at the intersection*/
    int count_min(const int& iter);

    /*Clears temporary variables (pass_matrix i map_pass_m) before next passage*/
    void clear_variables();

    /*Checks if all elements are 0s, return true if that's the case*/
    bool check_matrix();

    /*Chooses last passages, picking nodes that wasn't visited before.
    Chooses a path that leads through those nodes.*/
    void last_passes();

    /*Sorts all passages to form a path.
    Chooses first saved passages as beginning of the path.
    Function return sorted path.*/
    ivec sort_passes();

    /*This function uses all the other functions to solve tsp.
    Additionally it prints chosen path and the lower bound.*/
    ivec solve_tsp();

    //GETTERS
    const ddvec& get_cost_matrix() const{return cost_matrix;}
    const iivec& get_pass_matrix() const{return pass_matrix;}
    const iivec& get_chosen_path() const{return chosen_path;}
    const ivec_int_map& get_map_pass_m() const{return map_pass_m;}
    const double& get_lb() const{return lb;}
private:
    ddvec cost_matrix; //cost matrix
    iivec pass_matrix; //matrix with possible passages
    ivec_int_map map_pass_m; //variable that pair nodes with minimum values in rows and columns
    iivec chosen_path; //chosen path is saved as pair chosen_path[0]->chosen_path[1]
    double lb; //lower bound

};
#endif // TSP_H
