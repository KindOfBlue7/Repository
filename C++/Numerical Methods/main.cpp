#include "interpolation.h"
#include <fstream>
#include <sstream>

using svmap = std::map<std::string, std::vector<float>>;

svmap read_csv(const std::string& filename="data.csv"){
    svmap data;
    fvec vec;
    std::vector<std::string> var_names;

    std::ifstream f;
    f.open(filename, std::ios::in);

    if(f.good()) {
        std::cout << "File successfully opened." <<std::endl;

        std::string output,temp;
        std::getline(f,output);
        for(int i=0; i!=output.size(); ++i){
            if(output[i] == ',') {
                var_names.push_back(temp);
                temp.clear();
            }else{
                temp.push_back(output[i]);
            }
        }
        var_names.push_back(temp);

        for(const auto& x : var_names){
            data[x] = {};
        }

        int i, j=0;
        temp.clear();

        while (std::getline(f, output)) {
            j=0;
            temp.clear();
            for(i=0; i!=output.size(); ++i){
                if(output[i] == ',') {
                    data[var_names[j]].push_back(std::stoi(temp));
                    temp.clear();
                    ++j;
                }else{
                    temp.push_back(output[i]);
                }
            }
            data[var_names[j]].push_back(std::stoi(temp));
        }
        f.close();
    }else{
        std::cout << "Failed to open file." <<std::endl;
    }
    return data;
}

int main()
{
    fvec x = {1,3,5,7,9,11,13,14,15,17,19,21};
    fvec y = {1,2,4,5,7,9,11,12,16,18,20};
    svmap data = read_csv();
    Interpolation interp = Interpolation(data.find("x")->second,data.find("y")->second,"linear");
    ffmap result = interp.interpolate({2,4,5,7,9,11,12});
    Interpolation::print_map(result);

    return 0;
}