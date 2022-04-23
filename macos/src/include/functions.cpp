#include "functions.h"

constexpr double FIVEDIVNINE = (5.0/9.0);
constexpr double NINEDIVFIVE = (9.0/5.0);

double FahrenheitToCelsius(double temp)
{
  return (FIVEDIVNINE*(temp-32.0));
}

double CelsiusToFahrenheit(double temp)
{
  return ((temp*NINEDIVFIVE)+32.0);
}
