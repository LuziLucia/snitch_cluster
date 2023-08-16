#include "snrt.h"
#include "data.h"
//#include <math.h>
//#include "math.h"

// Define your kernel

// INFO: Custom absolute value function for floating point numbers, since it is also not supported.
double my_fabs(double x) {
    if(x < 0) {
        return -x;
    } else {
        return x;
    }
}

// INFO: This is a custom function to determine the expponential of a floating point number.
//       We assume here the sum representation of an exponential: exp_n(x) = sum_{i=0}^n (x^i/i!).
//       If two partial sums differ less than epsilon, we can stop the summing.
inline double my_exp(double x) 
{ 
    const double epsilon = 1e-7; 
    double e = 0.0; 
    int n = 0; 
    double factorial = 1; 
    double power = 1.0; 
    double term; 
    do { 
        term = power/factorial; 
        e += term; 
        n += 1; 
        power *= x; 
        factorial *= n; 
    } while (my_fabs(term) >= epsilon); 
    return e; 
} 

static inline double naive_exponential(double x) {
    double result = 1.0f;
    double term = 1.0f;
    for (int i = 1; i < 10; i++) {
        term *= x / i;
        result += term;
    }
    return result;
}

double my_log(double x, double n)
{
    double alpha = (x-1)/(x+1), ans = alpha;
    double save = ans * alpha * alpha;

    for (int i = 2 ; i <= n ; i++)
    {
        ans += (1.0/(2*i-1)) * save;
        save = save * alpha * alpha;
    }

    return 2.0*ans;
}


void softmax(uint32_t length, double *x, double *y, double *z) {
    double sum = 0.0;
    double max_x = 0.0;

    // get max value of x
    for (int i = 0; i < length; ++i)
    {
        if (x[i] > max_x) max_x = x[i];
    }

    for (int i = 0; i < length; i++)
    {
        //y[i] = expf(x[i] - max_x);
        y[i] = my_exp(x[i] - max_x);
        //y[i] = naive_exponential(x[i]);

        sum += y[i];
    }

    for(int i = 0; i < length; i++)
    {
        z[i] = y[i] / sum;
    }
}

int main() {
    double cycles;
    double rel_err = 0.0;

    // Read the mcycle CSR (this is our way to mark/delimit a specific code region for benchmarking)
    uint32_t start_cycle = mcycle();

    softmax(l, x, y, z);
    snrt_cluster_hw_barrier();

    // Read the mcycle CSR
    uint32_t end_cycle = mcycle();

    // get #cycles and save in reg
    cycles = end_cycle - start_cycle;
    asm volatile (
        "fmv.d    ft9, %[cycles]\n"
        : [cycles] "+f"(cycles)
    );

    // get the relative error
    for (int i = 0; i < l; i++) {
        rel_err += my_fabs(z[i] - result[i]) / (result[i]);
    }    
    rel_err /= l;
    asm volatile (
        "fmv.d    ft10, %[rel_err]\n"
        : [rel_err] "+f"(rel_err)
    );

    return 0;
}