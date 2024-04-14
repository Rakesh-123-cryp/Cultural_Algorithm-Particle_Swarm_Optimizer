#include <iostream>
#include <vector>
#include <omp.h>
#include <functional>
#include <cmath>
#include <math.h>
#include <map>
#include <ctime>

typedef std::vector<std::vector<float>> Matrix;
typedef std::vector<float> cost_type;

struct pos_value{
    cost_type cost;
    std::vector<size_t> elite;
};

typedef struct pos_value pos_value;


typedef struct elite_return{
    pos_value* elite_indv;
    std::vector<int> elite_indices;
} elite_return;

typedef struct elite_return elite_return;

#include "../utils.cpp"
#include "../CAPSO.cpp"
#include "../PSO.cpp"
#include "../Pareto.cpp"

struct PopSpace_return{
    bool status;
    Pareto::Front* pareto_front;
};

typedef struct PopSpace_return PopSpace_return;