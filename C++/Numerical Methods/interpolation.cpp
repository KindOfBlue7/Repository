#include "interpolation.h"

void Interpolation::print_map(const ffmap& fmap) {
    std::cout << "x : y" <<std::endl;
    for(auto & it : fmap)
        std::cout << it.first << " : "<< it.second <<std::endl;
}

void Interpolation::print_fvec(const fvec& vec) {
    std::cout << "Vector of " << vec.size() << " numbers:" <<std::endl;
    for(auto & it : vec){
        std::cout << it << " ";
    }
    std::cout <<std::endl;
}

ffmap Interpolation::linear(fvec& x_vec) {
    ffmap result;
    int j=0;
    for(int i=0; i<x_vec.size(); ++i){
        if(i!=0 && x_vec[i]<x_vec[i-1])
            throw std::invalid_argument("Given vector is not incrementing.");
        if(x_vec[i]>_x[j])
            ++j;
        result[x_vec[i]] = (_y[j+1]-_y[j])/(_x[j+1]-_x[j])*(x_vec[i]-_x[j]) + _y[j];
    }
    return result;
}

//NOT OPTIMISED YET
ffmap Interpolation::lagrange(fvec& x_vec) {
    ffmap result;
    float temp1, temp2;
    for(auto& x: x_vec){
        temp1 = 0;
        for(int i=0; i<_x.size(); ++i){
            temp2=1;
            for(int j=0; j<_x.size(); ++j){
                if(i!=j){
                    temp2 *= (x-_x[j])/(_x[i]-_x[j]);
                }
            }
            temp1 += _y[i]*temp2;
        }
        result[x] = temp1;
    }
    return result;
}

//NOT IMPLEMENTED
ffmap Interpolation::newton(fvec& x_vec) {

}

//NOT IMPLEMENTED
ffmap Interpolation::spline(fvec& x_vec) {

}

float Interpolation::calculate_error(ffvec correct_data) {

}

ffmap Interpolation::interpolate(fvec x) {
    if(x.front()<_x.front() || x.back()>_x.back())
        throw std::out_of_range("Given vector is bigger than data vector.");

    if(_method == "linear"){
        return linear(x);
    }else if(_method == "lagrange"){
        return lagrange(x);
    }else if(_method == "newton"){
        return newton(x);
    }else if(_method == "spline"){
        return spline(x);
    }
}