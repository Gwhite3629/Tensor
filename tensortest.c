#include "utils.h"
#include "tensor.h"

#include <stdio.h>

int main(void)
{ get_H;
    
    tensor_t T;
    int v = 2;

    CHECK(init_tensor(&T, 5, v, v, v, v, v));

    for (int i = 0; i < v; i++) {
        for (int j = 0; j < v; j++) {
            for (int k = 0; k < v; k++) {
                for (int q = 0; q < v; q++) {
                    for (int g = 0; g < v; g++) {
                        CHECK(set_T(&T, i+j+k+q+g, i, j, k, q, g));
                    }
                }
            }
        }
    }

    print_T(T);

    CHECK(copy(T));
    tensor_t t = f_ret.ret_val.tensor;

    CHECK(get_T(t, 1, 1, 1, 1, 1));
    printf("Copied value at [1, 1, 1, 1, 1] = %lf\n", f_ret.ret_val.doub);

    clean_T(&T);
    clean_T(&t);

    tensor_t a, b;

    CHECK(init_tensor(&a, 2, 2, 3));
    CHECK(set_T(&a, 4.5, 1, 2));

    CHECK(transpose(a));
    b = f_ret.ret_val.tensor;
    CHECK(get_T(b, 1, 1));
    printf("b[1, 1] = %lf\n", f_ret.ret_val.doub);
    CHECK(get_T(b, 2, 1));
    printf("b[2, 1] = %lf\n", f_ret.ret_val.doub);

    clean_T(&a);
    clean_T(&b);

    tensor_t r;

    CHECK(init_tensor_R(&r, 4.2, 5.1, 2, 10, 10));
    print_T(r);

    double TrueValue;
    int idx;
    double CheckValue;
    int *tup;
    CHECK(get_T(r, 8, 9));
    TrueValue = f_ret.ret_val.doub;
    CHECK(get_indx(r, 8, 9));
    idx = f_ret.ret_val.index;
    CHECK(indx(r, idx));
    tup = (int *)f_ret.ret_val.ptr;
    CheckValue = r.data[idx];

    printf("Correct value: %lf\nIdx: %d\nCheck value: %lf\n", TrueValue, idx, CheckValue);

    for (int i = 0; i < r.dims; i++) {
        printf("index[%d]: %d\n", i, tup[i]);
    }

    clean_T(&r);

exit:
    SFREE(tup);
    if (f_ret.ret_type == ERROR)
        return f_ret.ret_val.error;
    else
        return 0;
}