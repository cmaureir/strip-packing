#ifndef COMMON_HPP
#define COMMON_HPP
#include  <iostream>
#include  <fstream>
#include  <vector>
#include  <ctime>
#include  <cstdlib>
#include  <boost/program_options.hpp>

namespace po = boost::program_options;
using namespace std;

struct box{
    int id;
    int height;
    int width;
};

struct solution {
    // items chain
    vector<int> items;

    // Bottom Left corner coordinates per item
    vector<int> a;
    vector<int> b;

    // used percentage
    float p;

    // max height
    int height;

    // fitness of the solution
    float fitness;
    int **strip;
};

extern vector<box> bs;
extern vector<solution> sols;
extern vector<solution> refset;
extern vector<solution> refset_tmp;
extern vector<solution> new_set;
extern solution best;

extern int strip_width;
extern int strip_height;
extern int iteration;
extern int loop;

// Evaluation function evaluations
#define max_iter 100
// Number of solutions in RefSet
static int b = 10; //2
static int popsize = 5*b;

#endif
