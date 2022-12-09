#include "tensorclass.h"

#include <iostream>
#include <functional>

int main(void)
{
    Tensor<double, 3, 3, 3> t(2.0, 2.0);
    int arg = 1;

    std::function<double(double, double, int, int*)> 
    L = [=](const double o1, const double o2, const int idx, auto *c) 
    {double t; (idx == 0) ? t = (o1*o2 + (*c)) : t = (o1*o2 + (*c))/idx; (*c) = t; return t; };

    t.iterate(1.0, L, &arg, 9, 18);

    t.print();

    return 0;
}