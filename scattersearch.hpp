#include "common.hpp"

bool check_flags(int argc, char *argv[], string &path);
void read_input_file(string path);

void print_input_file();
void print_one_solution(solution tmp);
void print_strip(solution sol);
void print_solutions(vector<solution> tmp);

void clear_strip(int **strip);
bool struct_cmp(solution i, solution j);
void sort_solutions(vector<solution> &tmp);

bool search_fit(int item, int item_w, int item_h, int &a, int &b, int &h, int **strip);
bool is_in(solution tmp, vector<solution> tmp_sols);
void place_item(int item, int item_w, int item_h, int a, int b, int **strip);

void fitness_calculation_one(solution &tmp);
void fitness_calculation(vector<solution> &tmp);

void solutions_generation(vector<solution> &soluciones);
void solutions_improvement(vector<solution> &tmp_sols);
void refset_build();
void save_best_solution(vector<solution> tmp);
void solutions_combination();
void refset_modification(vector<solution> tmp_sols);

solution shift_left(solution tmp);
vector<solution> solutions_improvement2(vector<solution> tmp);

void refset_rebuild();
int get_difference(vector<solution> a, vector<solution> b);
