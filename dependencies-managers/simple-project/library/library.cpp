#include "library.h"


double calc_integral_riemann(OneArgumentFunction function,
                             double a, double b, int n) {
    double result = 0.0;
    double h = (b - a) / n;
    for (int i = 0; i < n; i++) {
        // Calculate in mean point and add
        result += function(a + h * (i + 0.5));
    }
    result *= h;
    return result;
}


double calc_integral_trapezoidal(OneArgumentFunction function,
                                 double a, double b, int n) {
    double result = 0.0;
    if (n <= 0.0)
  	return result;
 
    double step = (b - a) / (1.0 * n);
    for (int i = 1; i < n; ++i) {
      result += function(a + i * step);
    }
    result += (function(a) + function(b)) / 2.0;
    result *= step; 
    return result;
}


double calc_integral_simpson(OneArgumentFunction function,
                             double a, double b, int n) {
    double result = 0.0;
    if (n <= 0.0)
  	return result;
 
    double step = (b - a) / (1.0 * n);
    for (int i = 1; i < n / 2 + 1; ++i) {
      result += (function(a + step * (2 * i - 2)) +
                 4.0 * function(a + step * (2 * i - 1)) +
                 function(a + step * (2 * i)));
    }
    result *= (step / 3.0); 
    return result;
}

