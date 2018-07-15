#ifndef LIBRARY_H
#define LIBRARY_H

#include <functional>

typedef std::function<double(double)> OneArgumentFunction;


/**
 * Calculates defined integral using rule:
 * https://en.wikipedia.org/wiki/Riemann_sum
 *
 */
double calc_integral_riemann(OneArgumentFunction function,
                            double a, double b, int n);


/**
 * Calculate defined integral usign rule:
 * https://en.wikipedia.org/wiki/Trapezoidal_rule
 *
 */
double calc_integral_trapezoidal(OneArgumentFunction function,
                                 double a, double b, int n);


/**
 * Calculate defined integral usign rule:
 * https://en.wikipedia.org/wiki/Simpson%27s_rule
 *
 */
double calc_integral_simpson(OneArgumentFunction function,
                             double a, double b, int n);

#endif /* LIBRARY_H */
