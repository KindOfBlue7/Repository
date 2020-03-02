#include "interpolation.h"
#include <fstream>

void read_csv(const std::string& filename="data.txt"){
    std::string output;
    std::fstream f;
    f.open(filename, std::ios::in);
    if(f.is_open()) {
        std::cout << "File successfully opened." <<std::endl;
        while (std::getline(f, output)) {
            std::cout << output << std::endl;
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