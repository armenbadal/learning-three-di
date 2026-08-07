#pragma once

#include <cmath>
#include <iostream>

#define EQ(a, b) if((a) != (b)) std::cerr << "FAILED: " << #a << " != " << #b << std::endl 

#define FLOAT_EQ(a, b) if(std::fabs((a) - (b)) >= 1e-5f) std::cerr << "FAILED: " << #a << " != " << #b << std::endl 
