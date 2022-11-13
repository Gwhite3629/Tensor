#ifndef _TENSOR_H_
#define _TENSOR_H_

#include <cassert>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>

template <class T, size_t... I>
class Tensor
{
public:
    int ret = 0;
    Tensor(void);
    ~Tensor(void);
    Tensor(const T, const T);

    /*
     * INDEX
     */

    template<typename... Args>
    T& operator()(Args &&...);
    template<typename... Args>
    T operator()(Args &&...) const;

    T& operator()(std::vector<int>);
    T operator()(std::vector<int>) const;

    /*
     * OPERATORS
     */

    // Assignment
    void operator=(const Tensor<T, I...>&);
    void operator=(const T);

    // Addition
    Tensor operator+(const Tensor<T, I...>&) const;
    Tensor operator+(const T) const;
    void operator+=(const Tensor<T, I...>&) const;
    void operator+=(const T) const;

    // Subtraction
    Tensor operator-(const Tensor<T, I...>&) const;
    Tensor operator-(const T) const;
    void operator-=(const Tensor<T, I...>&) const;
    void operator-=(const T) const;

    // Multiplication
    Tensor operator*(const Tensor<T, I...>&) const;
    Tensor operator*(const T) const;
    void operator*=(const Tensor<T, I...>&) const;
    void operator*=(const T) const;

    // Division
    Tensor operator/(const Tensor<T, I...>&) const;
    Tensor operator/(const T) const;
    void operator/=(const Tensor<T, I...>&) const;
    void operator/=(const T) const;

    // Adds all elements of tensor
    T sum(void);

    // Modest attempt to print
    void print(void);

private:
    // Raw data
    T *data = NULL;

    // Number of dims
    int dims;

    // Determined at init
    bool square;
    
    // Cached on first use of dynamic time calculations
    std::vector<int> dimensions;

    // Cached
    int max_idx;

    // Compile time internal index calculation
    template<typename... Args>
    int calc_index(Args &&...) const;

    // Dynamic time index calculation
    int calc_index(std::vector<int>) const;

    std::vector<int> index(int) const;
};

template <class T, size_t... I>
Tensor<T, I...>::Tensor(void)
{
    int temp = 1;

    int dim_size[] = {I...};

    dims = sizeof(dim_size)/sizeof(int);

    square = 1;

    for (int i = 0; i < dims; i++) {
        temp = temp * dim_size[i];
        dimensions.push_back(dim_size[i]);
        if (i & (dim_size[i-1] != dim_size[i]))
            square = 0;
    }


    data = (T*)malloc(temp*sizeof(T));
    assert(data != NULL);
    max_idx = temp;
}

template <class T, size_t... I>
Tensor<T, I...>::~Tensor(void)
{
    if (data) {
        free(data);
        data = NULL;
    }
}

template <class T, size_t... I>
Tensor<T, I...>::Tensor(const T start, const T end)
{
    int temp = 1;
    time_t t;

    srand((unsigned int) time(&t));

    int dim_size[] = {I...};

    dims = sizeof(dim_size)/sizeof(int);
    square = 1;

    for (int i = 0; i < dims; i++) {
        temp *= dim_size[i];
        dimensions.push_back(dim_size[i]);
        if (i & (dim_size[i-1] != dim_size[i]))
            square = 0;
    }

    data = (T*)malloc(temp*sizeof(T));
    assert(data != NULL);

    for (int i = 0; i < temp; i++) {
        data[i] = ((T)rand()/(T)(RAND_MAX)) * (end - start) + start;
    }
    max_idx = temp;
}

template <class T, size_t... I>
void Tensor<T, I...>::print(void)
{
    int i, j = 0;
    std::string s;
    char buf[10];

    int dim_size[] = {I ...};

    for (int x : dim_size) {
        s += itoa(x, buf, 10);
        s += 'x';
    }

    s.pop_back();

    std::cout << "Dimensions: " << s << std::endl;

    if (square) {
        // Generate string;

        int l = dim_size[0];
        int m = l*l;

        for (i = 0; i < max_idx; i++) {
            if ((i%l) == 0)
                printf("[%2.2lf, ", data[i]);
            else if (i%l == (l-1)) {
                printf("%2.2lf]\n", data[i]);
                j += 1;
            } else
                printf("%2.2lf, ", data[i]);

            if ((i+1) == m*l) {
                m += m;
                printf("\n");
            }

            if ((j) == l) {
                printf("\n");
                j = 0;
            }
        }

        return;
    } else {
        return;
    }
}

template <class T, size_t... I>
template<typename... Args>
T& Tensor<T, I...>::operator()(Args &&... idx)
{
    return data[calc_index(idx ...)];
}

template <class T, size_t... I>
template<typename... Args>
T Tensor<T, I...>::operator()(Args &&... idx) const
{
    return data[calc_index(idx ...)];
}

template <class T, size_t... I>
T& Tensor<T, I...>::operator()(std::vector<int> i)
{
    return data[calc_index(i)];
}

template <class T, size_t... I>
T Tensor<T, I...>::operator()(std::vector<int> i) const
{
    return data[calc_index(i)];
}

template <class T, size_t... I>
void Tensor<T, I...>::operator=(const Tensor<T, I...>& t)
{
    memcpy(data, t.data, max_idx*sizeof(T));
}

template <class T, size_t... I>
void Tensor<T, I...>::operator=(const T v)
{
    memset(data, v, max_idx*sizeof(T));
}

template <class T, size_t... I>
Tensor<T, I...> Tensor<T, I...>::operator+(const Tensor<T, I...>& t) const
{
    Tensor<T, I...> r;
    for (int i = 0; i < max_idx; i++) {
        r.data[i] = data[i] + t.data[i];
    }
    return r;
}

template <class T, size_t... I>
Tensor<T, I...> Tensor<T, I...>::operator+(const T v) const
{
    Tensor<T, I...> r;
    for (int i = 0; i < max_idx; i++) {
        r.data[i] = data[i] + v;
    }
    return r;
}

template <class T, size_t... I>
void Tensor<T, I...>::operator+=(const Tensor<T, I...>&) const
{
    for (int i = 0; i < max_idx; i++) {
        data[i] = data[i] + t.data[i];
    }
}

template <class T, size_t... I>
void Tensor<T, I...>::operator+=(const T) const
{
    for (int i = 0; i < max_idx; i++) {
        data[i] = data[i] + v;
    }
}

template <class T, size_t... I>
Tensor<T, I...> Tensor<T, I...>::operator-(const Tensor<T, I...>& t) const
{
    Tensor<T, I...> r;
    for (int i = 0; i < max_idx; i++) {
        r.data[i] = data[i] - t.data[i];
    }
    return r;
}

template <class T, size_t... I>
Tensor<T, I...> Tensor<T, I...>::operator-(const T v) const
{
    Tensor<T, I...> r;
    for (int i = 0; i < max_idx; i++) {
        r.data[i] = data[i] - v;
    }
    return r;
}

template <class T, size_t... I>
void Tensor<T, I...>::operator-=(const Tensor<T, I...>& t) const
{
    for (int i = 0; i < max_idx; i++) {
        data[i] = data[i] - t.data[i];
    }
}

template <class T, size_t... I>
void Tensor<T, I...>::operator-=(const T v) const
{
    for (int i = 0; i < max_idx; i++) {
        data[i] = data[i] - v;
    }
}

template <class T, size_t... I>
Tensor<T, I...> Tensor<T, I...>::operator*(const Tensor<T, I...>& t) const
{
    Tensor<T, I...> r;
    for (int i = 0; i < max_idx; i++) {
        r.data[i] = data[i] * t.data[i];
    }
    return r;
}

template <class T, size_t... I>
Tensor<T, I...> Tensor<T, I...>::operator*(const T v) const
{
    Tensor<T, I...> r;
    for (int i = 0; i < max_idx; i++) {
        r.data[i] = data[i] * v;
    }
    return r;
}

template <class T, size_t... I>
void Tensor<T, I...>::operator*=(const Tensor<T, I...>& t) const
{
    for (int i = 0; i < max_idx; i++) {
        data[i] = data[i] * t.data[i];
    }
}

template <class T, size_t... I>
void Tensor<T, I...>::operator*=(const T v) const
{
    for (int i = 0; i < max_idx; i++) {
        data[i] = data[i] * v;
    }
}

template <class T, size_t... I>
Tensor<T, I...> Tensor<T, I...>::operator/(const Tensor<T, I...>& t) const
{
    Tensor<T, I...> r;
    for (int i = 0; i < max_idx; i++) {
        r.data[i] = data[i] * t.data[i];
    }
    return r;
}

template <class T, size_t... I>
Tensor<T, I...> Tensor<T, I...>::operator/(const T v) const
{
    Tensor<T, I...> r;
    for (int i = 0; i < max_idx; i++) {
        r.data[i] = data[i] / v;
    }
    return r;
}

template <class T, size_t... I>
void Tensor<T, I...>::operator/=(const Tensor<T, I...>& t) const
{
    for (int i = 0; i < max_idx; i++) {
        data[i] = data[i] / t.data[i];
    }
}

template <class T, size_t... I>
void Tensor<T, I...>::operator/=(const T v) const
{
    for (int i = 0; i < max_idx; i++) {
        data[i] = data[i] / v;
    }
}

template <class T, size_t... I>
T Tensor<T, I...>::sum(void)
{
    T r = 0;
    for (int i = 0; i < max_idx; i++) {
        r += data[i];
    }
    return r;
}

template <class T, size_t... I>
template <typename... Args>
int Tensor<T, I...>::calc_index(Args &&... idx) const
{
    int t;
    int index = 0;

    int dim_size[] = {I ...};
    int id[] = {idx ...};

    for (int i = 0; i < dims; i++) {
        t = id[i];
        assert(t < dim_size[i]);
        for (int j = (i +  1); j < dims; j++) {
            t = t * dim_size[i];
        }
        index += t;
    }

    return index;
}

template <class T, size_t... I>
int Tensor<T, I...>::calc_index(std::vector<int> id) const
{
    int t;
    int index = 0;

    int dim_size[] = {I ...};

    for (int i = 0; i < dims; i++) {
        t = id[i];
        assert(t < dim_size[i]);
        for (int j = (i +  1); j < dims; j++) {
            t = t * dim_size[i];
        }
        index += t;
    }

    return index;
}

template <class T, size_t... I>
std::vector<int> Tensor<T, I...>::index(int idx) const
{
    std::vector<int> l;
    int tmp1 = 1;
    int tmp2 = 0;
    int tmp3 = 1;
    int i = 0;
    int j = 0;
    int k = 0;

    int dim_size[] = {I ...};

    // Calculate indexes in reverse
    for (i = dims - 1; i >= 0; i--) {
        for (j = i + 1; j < dims; j++) {
            for (k = j + 1; k < dims; k++) {
                tmp1 *= dim_size[k];
            }

            tmp2 += tmp1 * l[j];

            tmp1 = 1;
        }

        tmp2 = idx - tmp2;

        for (j = i + 1; j < dims; j++) {
            tmp3 *= dim_size[j];
        }

        tmp2 /= tmp3;

        l.front(tmp2 % dim_size[i]);

        tmp1 = 1;
        tmp2 = 0;
        tmp3 = 1;
    }

    return l;
}

#endif // _TENSOR_H_