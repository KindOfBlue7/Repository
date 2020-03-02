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

        std::vector<std::vector<float>> var_values[var_names.size()];
        int i, j=0;

        while (std::getline(f, output)) {
            j=0;
            for(i=0; i!=output.size(); ++i){
                if(output[i] == ',') {
                    ++j;
                }else{
                    var_values->at(j).push_back(output[i]);
                }
            }
        }
        f.close();
    }else{
        std::cout << "Failed to open file." <<std::endl;
    }
}

int main()
{
    fvec x = {1,3,5,7,9,11,13,14,15,17,19,21};
    fvec y = {1,2,4,5,7,9,11,12,16,18,20};
    //Interpolation interp = Interpolation(x,x,"linear");
    //ffmap result = interp.interpolate({1,2,4,5,7,9,11,12,16,18,20});
    //Interpolation::print_map(result);
    read_csv();
    return 0;
}