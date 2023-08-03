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

// static inline?
void softmax(uint32_t length, double *x, double *y, double *z) {
    //int core_idx = snrt_cluster_core_idx();
    //int offset = core_idx * l;
    double sum = 0.0;

    for(int i = 0; i < length; i++)
    {
        y[i] = expf(x[i]);
        sum += y[i];
    }

    if(sum == 0.0) 
    {
        sum = 0.001;
    }

    for(int i = 0; i < length; i++)
    {
        z[i] = y[i] / sum;
    }
}

int main() {
    uint32_t errors = 0;

    // Read the mcycle CSR (this is our way to mark/delimit a specific code region for benchmarking)
    uint32_t start_cycle = mcycle();

    // DM core does not participate in the computation
    //if(snrt_is_compute_core())
    softmax(l, x, y, z);
    snrt_cluster_hw_barrier();

    // Read the mcycle CSR
    uint32_t end_cycle = mcycle();

    // Check if computation is correct
    for (int i = 0; i < l; i++) {
        //if (z[i] != result[i]) errors++;
    
        if (my_fabs(z[i] - result[i]) > 0.001) errors++;
    }

    return errors;
}