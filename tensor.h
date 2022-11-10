#ifndef _TENSOR_H_
#define _TENSOR_H_

#include <stdarg.h>

#include "utils.h"

#define get_H \
    handle_t f_ret; \
    f_ret.ret_type = ERROR; \
    f_ret.ret_val.error = SUCCESS;

typedef struct tensor {
    double *data;
    int dims;
    int *dim_size;
    bool square;
} tensor_t;

typedef union f_ret {
    err_t error;
    double doub;
    tensor_t tensor;
    void *ptr;
    int index;
} ret_t;

typedef enum types {ERROR, DOUBLE, TENSOR, POINTER, INDEX} types_t;

typedef struct handle {
    types_t ret_type;
    ret_t ret_val;
} handle_t;

typedef struct p_vars {
    bool square;
    int l;
    int max_idx;
} print_t;

handle_t init_tensor(tensor_t *T, ...);

handle_t init_tensor_R(tensor_t *T, double start, double end, ...);

handle_t get_T(tensor_t T, ...);

handle_t set_T(tensor_t *T, double val, ...);

handle_t indx(tensor_t T, int I);

handle_t get_indx(tensor_t, ...);

void clean_T(tensor_t *T);

handle_t copy_(tensor_t T);

handle_t copy(tensor_t T);

handle_t transpose(tensor_t T);

void print_T(tensor_t T);

#endif // _TENSOR_H_