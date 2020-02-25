#include "tsp.h"

void TSP_solver::print_cost_m(){
    for(int i=0; i<cost_matrix[0].size();++i){
        for(double el: cost_matrix[i]){
            if(el!=INF)
                std::cout << std::setw(5)<< el;
            else
                std::cout << std::setw(5) << "-";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

void TSP_solver::print_ddvec(const ddvec& matrix){
    for(auto& i: matrix){
        for(auto& j: i){
            std::cout << j+1 << " ";
        }
        std::cout << std::endl;
    }
}

void TSP_solver::print_map_pass_m(){
    for(auto& i: pass_matrix){
        std::cout << "(" << i[0]+1 << "," << i[1]+1 << ") -> " << map_pass_m[i] <<std::endl;
    }
}

void TSP_solver::print_ivec(const ivec& vec){
    for(auto &i: vec)
        std::cout << i+1 << " ";
    std::cout<<std::endl;
}

void TSP_solver::lb_rows(){
    dvec _min;
    double minim;
    for(int i=0;i<cost_matrix[0].size();++i){
        minim = INF;
        for(int j=0;j<cost_matrix[0].size();++j){
            if(cost_matrix[i][j]<minim && cost_matrix[i][j] != INF){
                minim = cost_matrix[i][j];
            }
        }
        if(minim==INF){
            _min.push_back(0);
            lb += 0;
        }else{
            _min.push_back(minim);
            lb += minim;
        }
    }
    for(int i=0;i<cost_matrix[0].size();++i){
        for(int j=0;j<cost_matrix[0].size();++j){
            if(cost_matrix[i][j]!=INF){
                cost_matrix[i][j] -= _min[i];
            }
        }
    }
}

void TSP_solver::lb_cols(){
    dvec _min;
    double minim;
    for(int i=0;i<cost_matrix[0].size();++i){
        minim = INF;
        for(int j=0;j<cost_matrix[0].size();++j){
            if(cost_matrix[j][i]<minim && cost_matrix[j][i] != INF){
                minim = cost_matrix[j][i];
            }
        }
        if(minim==INF){
            _min.push_back(0);
            lb += 0;
        }else{
            _min.push_back(minim);
            lb += minim;
        }
    }
    for(int i=0;i<cost_matrix[0].size();++i){
        for(int j=0;j<cost_matrix[0].size();++j){
            if(cost_matrix[j][i]!=INF){
                cost_matrix[j][i] -= _min[i];
            }
        }
    }
}

void TSP_solver::check_pass(){
    for(int i=0; i<cost_matrix[0].size(); ++i){
        for(int j=0; j<cost_matrix[0].size(); ++j){
            ivec temp;
            if(cost_matrix[i][j]==0){
                temp.push_back(i);
                temp.push_back(j);
                pass_matrix.push_back(temp);
            }
        }
    }

    for(int i=0; i<pass_matrix.size(); ++i)
        map_pass_m[pass_matrix[i]] = count_min(i);

    ivec max_pass = pass_matrix[0];

    for(auto& i: pass_matrix)
        if(map_pass_m[i]>map_pass_m[max_pass])
            max_pass = i;

    chosen_path.push_back(max_pass);

    for(int i=0; i<cost_matrix[0].size(); ++i){
        cost_matrix[max_pass[0]][i] = INF;
        cost_matrix[i][max_pass[1]] = INF;
    }
    cost_matrix[max_pass[1]][max_pass[0]] = INF;
}

int TSP_solver::count_min(const int& iter){
    const int u = pass_matrix[iter][0];
    const int v = pass_matrix[iter][1];
    double min_row = INF;
    double min_col = INF;
    for(int i=0; i<cost_matrix[0].size(); ++i){
        if(v!=i)
            if(cost_matrix[u][i]<min_row)
                min_row = cost_matrix[u][i];
        if(u!=i)
            if(cost_matrix[i][v]<min_col)
                min_col = cost_matrix[i][v];
    }
    return min_col+min_row;
}

void TSP_solver::clear_variables(){
    pass_matrix.clear();
    map_pass_m.clear();
}

bool TSP_solver::check_matrix(){
    for(auto &i: cost_matrix){
        for(auto &j: i){
            if(j!=0 && j!=INF)
                return false;
        }
    }
    return true;
}

void TSP_solver::last_passes(){
    iivec possible_passes;
    ivec all_nodes;

    for(int i=0; i<cost_matrix[0].size();++i){
        all_nodes.push_back(i);
        for(int j=0; j<cost_matrix[0].size(); ++j){
            ivec temp;
            if(cost_matrix[i][j]==0){
                temp.push_back(i);
                temp.push_back(j);
                possible_passes.push_back(temp);
            }
        }
    }

    ivec unvisited_nodes;
    for(auto &i: all_nodes){
        bool is_in = false;
        for(auto &j: chosen_path){
            if(j[0]==i){
                is_in = true;
            }
        }
        if(!is_in){
            unvisited_nodes.push_back(i);
        }
    }

    for(auto &i: possible_passes){
        if(i[0]==unvisited_nodes[0] && i[1]==unvisited_nodes[1]){
            chosen_path.push_back(i);
            unvisited_nodes.erase(unvisited_nodes.begin());
        }else if(i[0]==unvisited_nodes[1] && i[1]==unvisited_nodes[0]){
            chosen_path.push_back(i);
            unvisited_nodes.erase(unvisited_nodes.begin()+1);
        }
    }

    for(auto &i: possible_passes){
        if(i[0]==unvisited_nodes[0]){
            chosen_path.push_back(i);
        }
    }
}

ivec TSP_solver::sort_passes(){
    ivec tsp_output;
    tsp_output.push_back(chosen_path[0][0]);
    tsp_output.push_back(chosen_path[0][1]);

    int szuk = chosen_path[0][1];

    for(int i=0; i<chosen_path.size()-1; ++i){
        for(int j=1; j<chosen_path.size(); ++j){
            if(chosen_path[j][0]==szuk){
                szuk = chosen_path[j][1];
                tsp_output.push_back(szuk);
                break;
            }
        }
    }
    return tsp_output;
}

ivec TSP_solver::solve_tsp(){
    bool rows;
    bool cols;

    while(1){
        lb_rows();
        lb_cols();
        if(check_matrix())
            break;
        check_pass();
        clear_variables();
    }

    last_passes();
    ivec tsp_output = sort_passes();

    std::cout<<"Result of tsp(): ";
    print_ivec(tsp_output);
    std::cout<<"Lower Bound = "<<lb<<std::endl;
    return tsp_output;
}
