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
        // número de nuevas soluciones en RefSet
        loop = 0;
        while (get_difference(refset_tmp,refset) != 0)
        {
            refset_tmp = refset;
            solutions_combination();
            fitness_calculation(new_set);
            //new_set = solutions_improvement2(new_set);
            solutions_improvement(new_set);
            refset_modification(new_set);
            loop++;
            cout << "\t" << loop << endl;
        }
        save_best_solution(refset);

        /* A: Si encontramos el óptimo, terminamos */
        if(best.p == 100){
            iteration = max_iter;
        }
        /* A: end */

        refset_rebuild();
        iteration++;
        cout << iteration << endl;
        print_one_solution(best);
        //print_strip(best);
    } while (iteration < max_iter);
    print_one_solution(best);
    //print_strip(best);

    return 0;
}
