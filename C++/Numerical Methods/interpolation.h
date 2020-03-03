#ifndef NUMERICAL_METHODS_INTERPOLATION_H
#define NUMERICAL_METHODS_INTERPOLATION_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <exception>

//aliases used in the code
using fvec = std::vector<float>;
using ffvec = std::vector<std::vector<float>>;
using ffmap = std::map<float,float>;


class Interpolation{
public:
    Interpolation(fvec x, fvec y, std::string method="linear"): _x(x), _y(y), _method(method){
        for(auto i = 0; i<x.size(); ++i)
            _data[x[i]] = y[i];
    }

    //implementation of different methods
    ffmap linear(fvec& x_vec);
    ffmap lagrange(fvec& x_vec);
    ffmap newton(fvec& x_vec);
    ffmap spline(fvec& x_vec);
    float calculate_error(ffvec correct_data);

    //printing functions
    static void print_fvec(const fvec& vec);
    static void print_map(const ffmap& fmap);

    //sort of main function
    ffmap interpolate(fvec x);

    //setters
    void set_method(std::string method){_method = method;};
    void set_x(fvec x){_x = x;};
    void set_y(fvec y){_y = y;};

    //getters
    [[nodiscard]] const std::string& get_method() const{return _method;};
    [[nodiscard]] const ffmap& get_data() const{return _data;};
    [[nodiscard]] const fvec& get_x() const{return _x;};
    [[nodiscard]] const fvec& get_y() const{return _y;};
private:
    fvec _x, _y;
    ffmap _data;
    std::string _method;
};

#endif //NUMERICAL_METHODS_INTERPOLATION_H