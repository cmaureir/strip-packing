#include "scattersearch.hpp"

int main(int argc, char *argv[]) {

    // Configuration
    string path;
    if(!check_flags(argc,argv,path)){
        return 1;
    }
    read_input_file(path);

    // Algorithm
    //int iteration = 0;
    //int loop;
    int watch_best;
    int best_fitness;

    solutions_generation(sols);
    fitness_calculation(sols);

    solutions_improvement(sols);
    refset_build();

    save_best_solution(refset);
    refset_tmp = refset;

    do {
        // number of new solutions in the RefSet
        loop = 0;
        while (get_difference(refset_tmp,refset) != 0)
        {
            refset_tmp = refset;
            solutions_combination();
            fitness_calculation(new_set);
            new_set = solutions_improvement2(new_set);
            //solutions_improvement(new_set);
            refset_modification(new_set);
            loop++;
        }
        save_best_solution(refset);

        /* A: If we find the optimal, stop */
        if(best.p == 100){
            iteration = max_iter;
        }
        /* A: end */

        refset_rebuild();
        iteration++;
        //print_one_solution(best);
        //print_strip(best);
        //getchar();
    } while (iteration < max_iter);
    //print_one_solution(best);
    //print_strip(best);
    print_tikz(best);

    return 0;
}
