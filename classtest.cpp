#include "tensorclass.h"

#include <iostream>

int main(void)
{
    double s1;
    double s2;
    double s3;

    Tensor<double, 100, 100, 100> t1(-1.0, 0.0);
    Tensor<double, 100, 100, 100> t2( 0.0, 1.0);

    Tensor<double, 100, 100, 100> t3;

    t3 = t1 + t2;

    s1 = t1.sum();
    s2 = t2.sum();
    s3 = t3.sum();

    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;
    std::cout << s1 + s2 << std::endl;
    std::cout << s3 << std::endl;

    return 0;
}