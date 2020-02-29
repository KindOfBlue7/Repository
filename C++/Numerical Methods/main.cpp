#include "interpolation.h"

int main()
{
    fvec x = {1,3,5,7,9};
    Interpolation interp = Interpolation(x,x);
    ffmap result = interp.interpolate({2,4,6,8});
    Interpolation::print_map(result);
    return 0;
}