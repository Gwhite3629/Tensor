#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "utils.h"
#include "tensor.h"

handle_t init_tensor(tensor_t *T, ...)
{ get_H;

    va_list l;
    int temp = 1;

    va_start(l, T);
    
    T->dims = va_arg(l, int);

    MEM(T->dim_size, T->dims, int);

    T->square = 1;
 
    for (int i = 0; i < T->dims; i++) {
        T->dim_size[i] = va_arg(l, int);
        temp = temp * T->dim_size[i];
        if (i & (T->dim_size[i-1] != T->dim_size[i]))
            T->square = 0;
    }

    MEM(T->data, temp, double);

exit:
    if (f_ret.ret_val.error > 0) {
        if (T->dim_size)
            free(T->dim_size);
        if (T->data)
            free(T->data);
    }
    return f_ret;
}

handle_t init_tensor_R(tensor_t *T, double start, double end, ...)
{ get_H;

    va_list l;
    int temp = 1;
    time_t t;

    srand((unsigned int) time(&t));

    va_start(l, end);
    
    T->dims = va_arg(l, int);


    MEM(T->dim_size, T->dims, int);

    T->square = 1;
 
    for (int i = 0; i < T->dims; i++) {
        T->dim_size[i] = va_arg(l, int);
        temp = temp * T->dim_size[i];
        if (i & (T->dim_size[i-1] != T->dim_size[i]))
            T->square = 0;
    }

    MEM(T->data, temp, double);

    for (int i = 0; i < temp; i++) {
        T->data[i] = ((double)rand()/(double)(RAND_MAX)) * (end - start) + start;
    }

exit:
    if (f_ret.ret_val.error > 0) {
        if (T->dim_size)
            free(T->dim_size);
        if (T->data)
            free(T->data);
    }
    return f_ret;
}

handle_t get_T(tensor_t T, ...)
{ get_H;

    va_list l;
    va_start(l, T);
    int t;

    int index = 0;

    for (int i = 0; i < T.dims; i++) {
        t = va_arg(l, int);
        HANDLE(t, >=, T.dim_size[i], BAD_ARGUMENT);
        for (int j = (i+1); j < T.dims; j++) {
            t = t * T.dim_size[j];
        }
        index = index + t;
    }

    f_ret.ret_type = DOUBLE;
    f_ret.ret_val.doub = T.data[index];

exit:
    return f_ret;
}

handle_t set_T(tensor_t *T, double val, ...)
{ get_H;

    va_list l;
    va_start(l, val);
    int t;

    int index = 0;

    for (int i = 0; i < T->dims; i++) {
        t = va_arg(l, int);
        HANDLE(t, >=, T->dim_size[i], BAD_ARGUMENT);
        for (int j = (i+1); j < T->dims; j++) {
            t = t * T->dim_size[j];
        }
        index = index + t;
    }

    T->data[index] = val;

exit:
    return f_ret;
}

handle_t indx(tensor_t T, int I)
{ get_H;

    int *idx = NULL;
    int tmp1 = 1;
    int tmp2 = 0;
    int tmp3 = 1;
    int i = 0;
    int j = 0;
    int k = 0;

    MEM(idx, T.dims, int);

    // Calculate indexes in reverse
    for (i = T.dims - 1; i >= 0; i--) {
        for (j = i + 1; j < T.dims; j++) {
            for (k = j + 1; k < T.dims; k++) {
                tmp1 *= T.dim_size[k];
            }

            tmp2 += tmp1 * idx[j];

            tmp1 = 1;
        }

        tmp2 = I - tmp2;

        for (j = i + 1; j < T.dims; j++) {
            tmp3 *= T.dim_size[j];
        }

        tmp2 /= tmp3;

        idx[i] = tmp2 % T.dim_size[i];

        tmp1 = 1;
        tmp2 = 0;
        tmp3 = 1;
    }

    f_ret.ret_type = POINTER;
    f_ret.ret_val.ptr = (void *)idx;

exit:
    return f_ret;
}

handle_t get_indx(tensor_t T, ...)
{ get_H;

    va_list l;
    va_start(l, T);
    int t;

    int index = 0;

    for (int i = 0; i < T.dims; i++) {
        t = va_arg(l, int);
        HANDLE(t, >=, T.dim_size[i], BAD_ARGUMENT);
        for (int j = (i+1); j < T.dims; j++) {
            t = t * T.dim_size[j];
        }
        index = index + t;
    }

    f_ret.ret_type = INDEX;
    f_ret.ret_val.index = index;

exit:
    return f_ret;
}

void clean_T(tensor_t *T)
{
    SFREE(T->dim_size);
    SFREE(T->data);
}

handle_t transpose(tensor_t T)
{ get_H;
    tensor_t t;
    int temp = 1;
    double R = 0;

    HANDLE(T.dims, >, 2, BAD_ARGUMENT);

    t.dims = T.dims;

    MEM(t.dim_size, t.dims, int);

    int j = 0;
    for (int i = (t.dims-1); i >= 0; i--) {
        t.dim_size[i] = T.dim_size[j];
        temp = temp * t.dim_size[i];
        j++;
    }

    MEM(t.data, temp, double);

    for (int i = 0; i < T.dim_size[0]; i++) {
        for (int j = 0; j < T.dim_size[1]; j++) {
            CHECK(get_T(T, i, j));
            R = f_ret.ret_val.doub;
            CHECK(set_T(&t, R, j, i));
        }
    }

    t.square = T.square;

    f_ret.ret_type = TENSOR;
    f_ret.ret_val.tensor = t;

exit:
    return f_ret;
}

handle_t copy_(tensor_t T)
{ get_H;
    tensor_t t;
    int temp = 1;

    t.dims = T.dims;

    MEM(t.dim_size, t.dims, int);

    for (int i = 0; i < t.dims; i++) {
        t.dim_size[i] = T.dim_size[i];
        temp = temp * t.dim_size[i];
    }

    MEM(t.data, temp, double);

    t.square = T.square;

    f_ret.ret_type = TENSOR;
    f_ret.ret_val.tensor = t;

exit:
    return f_ret;
}

handle_t copy(tensor_t T)
{ get_H;
    tensor_t t;
    int temp = 1;

    t.dims = T.dims;

    MEM(t.dim_size, t.dims, int);

    for (int i = 0; i < t.dims; i++) {
        t.dim_size[i] = T.dim_size[i];
        temp = temp * t.dim_size[i];
    }

    MEM(t.data, temp, double);

    memcpy(t.data, T.data, temp*sizeof(double));

    t.square = T.square;

    f_ret.ret_type = TENSOR;
    f_ret.ret_val.tensor = t;

exit:
    return f_ret;
}

void print_T(tensor_t T)
{
    int i, j = 0;

    if (T.square) {
        // Generate string;
        print_t vars;
        vars.square = true;
        vars.l = T.dim_size[0];
        int m = vars.l*vars.l;
        vars.max_idx = (int)pow(vars.l,T.dims);

        for (i = 0; i < vars.max_idx; i++) {
            if ((i%vars.l) == 0)
                printf("[%2.2lf, ", T.data[i]);
            else if (i%vars.l == (vars.l-1)) {
                printf("%2.2lf]\n", T.data[i]);
                j += 1;
            } else
                printf("%2.2lf, ", T.data[i]);

            

            if ((i+1) == m*vars.l) {
                m += m;
                printf("\n");
            }

            if ((j) == vars.l) {
                printf("\n");
                j = 0;
            }
        }

        return;
    } else {
        return;
    }
}