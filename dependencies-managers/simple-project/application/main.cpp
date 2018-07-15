#include <iostream>
#include <math.h>
#include <cstdlib>

#include "library.h"


int main(int argc, char **argv) {
    if (argc != 4) {
      std::cout << "Command line args should be exactly 3: a and b for ellipse and n"
	        << std::endl;
      return -1;
    }
    double a = std::atof(argv[1]);
    double b = std::atof(argv[2]);
    int n = std::atoi(argv[3]);

    auto function = [a, b](double x) {
        double eps_sq = (a * a - b * b) / (a * a);
        return sqrt(1.0 - eps_sq * sin(x));
    };

    double l_riemann = calc_integral_riemann(function, 0, M_PI / 2.0, n);
    double l_trapezoidal = calc_integral_trapezoidal(function, 0, M_PI / 2.0, n);
    double l_simpson = calc_integral_simpson(function, 0, M_PI / 2.0, n);

    printf("Eps length with riemann rule = %f\n", l_riemann);
    printf("Eps length with trapezoidal rule = %f\n", l_trapezoidal);
    printf("Eps length with simpson rule = %f\n", l_simpson);

    return 0;
}
