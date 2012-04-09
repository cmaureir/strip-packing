#include "scattersearch.hpp"

vector<solution> sols;
vector<solution> refset;
vector<solution> refset_tmp;
vector<solution> new_set;
solution best;
vector<box> bs;
int strip_width;
int strip_height;
int iteration = 0;
int loop = 0;
/*********************
     BEGIN UTILS
 *********************/

// Check execution flags
bool check_flags(int argc, char *argv[], string &path){

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h"     , "Shows help message")
        ("input,i"    , po::value<std::string>() , "Path to the input file")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << endl;
        return false;
    }

    if (vm.count("input")) {
        path = vm["input"].as<std::string>();
        cout << "Using input file: " << path << endl;
    } else { cout << "You must set the «input file»" << endl; cout << desc << endl; return false;}


    return true;
}

void read_input_file(string path){
    ifstream file(path.c_str());
    string value;
    int line_number = 0;
    strip_height=0;
    box tmp;
    while (file.good())
    {
        if (line_number == 0)
        {
            for (int i = 0; i < 4; i++)
            {
                getline(file, value, ',');
            }
        }

        else
        {
            // ID as the line counter
            tmp.id = line_number;

            // Second element "Height"
            getline(file, value, ',');
            tmp.height = atoi(value.c_str());

            // Third element "Width"
            getline(file, value, ',');
            tmp.width = atoi(value.c_str());

            // Empty value
            getline(file, value, ',');
            if(line_number == 1)
            {
                strip_width = atoi(value.c_str());
            }
            strip_height += tmp.height;
            bs.push_back(tmp);
        }

        line_number++;
    }
}

void print_input_file(){
    int i;

    cout << "Strip width: " << strip_width << endl;
    cout << "id\theight\twidth" << endl;
    for(i = 0; i < (int)bs.size(); i++)
    {
        cout << bs.at(i).id << "\t";
        cout << bs.at(i).height << "\t";
        cout << bs.at(i).width << endl;
    }
}

// Imprime el valor de una solucion (orden de las cajas)
void print_one_solution(solution tmp){
    for (int i = 0; i < (int)tmp.items.size(); i++) {
        cout << tmp.items[i] << " ";
    }
    cout << " | " << tmp.fitness << " | " << tmp.p << "%" << endl;
}

void print_strip(solution sol){

    const string colors[] = {
            "\e[0;34m",//AZUL
            "\e[0;32m",//VERDE
            "\e[0;36m",//CYAN
            "\e[0;31m",//ROJO
            "\e[0;35m",//PURPURA
            "\e[0;33m",//MARRON
            "\e[0;37m",//GRISCLARO
            "\e[1;33m",//GRISOSCURO
            "\e[1;34m",//AZULCLARO
            "\e[1;32m",//VERDECLARO
            "\e[1;36m",//CYANCLARO
            "\e[1;31m",//ROJOCLARO
            "\e[1;35m",//PURPURACLARO
            "\e[1;33m",//AMARILLO
            "\e[0;37m"//BLANCO
    };

    vector<int> tmp = sol.items;
    int h = sol.height;

    for (int j = h-1; j >= 0; j--)
    {
        for (int i = 0; i < strip_width; i++)
        {

            if ( sol.strip[i][j] != 0)
            {
                for (int k = 0; k < (int)tmp.size(); k++)
                {
                    if(tmp[k] == sol.strip[i][j])
                    {
                        cout << colors[k%14] << sol.strip[i][j] << "\t";
                    }
                }
            }
            else
            {
                 cout << colors[14] << "#" << "\t";
            }
        }
        cout << endl;
    }
    cout << colors[14] << endl;
}

void print_solutions(vector<solution> tmp){

    for (int i = 0; i < (int)tmp.size(); i++)
    {
        cout << "Solucion " << i << ":\t";
        for (int j = 0; j < (int)tmp[i].items.size(); j++)
        {
            cout << tmp[i].items[j] <<  " ";
        }
        cout << " | " << tmp[i].height << " | " << tmp[i].p << "%" << endl;
    }

}

void clear_strip(int **strip){
    //cout << "clear_strip()" << endl;
    for (int i = 0; i < strip_width; i++) {
        for (int j = 0; j < strip_height; j++) {
            strip[i][j] = 0;
        }
    }
}

bool struct_cmp(solution i, solution j){
    return (i.fitness < j.fitness);
}

void sort_solutions(vector<solution> &tmp){
    sort(tmp.begin(), tmp.end(), struct_cmp);
}

/*********************
       END UTILS
 *********************/


// Buscar una posición para un elemento dentro del «strip»
//  Utilizando heurística Bottom-Left
bool search_fit(int item, int item_w, int item_h, int &a, int &b, int &h, int **strip){
    //cout << "search_fit()" << endl;
    bool fit = true;

    for (int y = 0; y < strip_height; y++)
    {
        for (int x = 0; x < strip_width; x++)
        {
            if (strip[x][y]  == 0 && item_w + x - 1 < strip_width) // empty space
            {
                for (a = x; a < x + item_w; a++)
                {
                    if (strip[a][y] != 0 || strip[a][y+item_h-1] != 0)
                    {
                        fit = false;
                        break;
                    }
                }
                if(fit)
                {
                    for (b = y; b < y + item_h; b++)
                    {
                        if (strip[x][b] != 0 || strip[x+item_w-1][b] != 0)
                        {
                            fit = false;
                            break;
                        }
                    }
                }

                if (fit){
                    a = x;
                    b = y;
                    if (h < b + item_h){
                        h = b + item_h;
                        cout << "Nueva altura " << h << endl;
                    }
                    return fit;
                }
            }
            else
            {
                x = x + bs[strip[x][y]-1].width - 1;
            }
            fit = true;
        }
    }
    return false;
}

// Busca si tmp está dentro de tmp_sols
bool is_in(solution tmp, vector<solution> tmp_sols)
{
    bool all = true;
    for (int i = 0; i < (int)tmp_sols.size(); i++) {
        for (int j = 0; j < (int)tmp.items.size(); j++) {
            if(tmp.items[j] != tmp_sols[i].items[j]){
                all = false;
            }
            if(!all){
                break;
            }
        }
    }
    return all;
}


// Posicionamiento de un item en el «strip»
void place_item(int item, int item_w, int item_h, int a, int b, int **strip){
    //cout << "place_item()" << endl;
    for (int i = a; i < a + item_w; i++) {
        for (int j = b; j < b + item_h; j++) {
            strip[i][j] = item;
        }
    }
}


void fitness_calculation_one(solution &tmp){

    //cout << "fitness_calculation_one()" << endl;
    int j;
    int n = (int)tmp.items.size();
    int item, item_w, item_h;
    int a, b;

    tmp.height  = 0;
    tmp.fitness = 0.0;
    tmp.p = 0.0;
    clear_strip(tmp.strip);

    for (j = 0; j < n; j++)
    {
        a = 0;
        b = 0;
        item   = tmp.items[j];
        item_w = bs.at(item-1).width;
        item_h = bs.at(item-1).height;
        if(search_fit(item,item_w,item_h, a, b, tmp.height, tmp.strip)){
            place_item(item,item_w,item_h, a, b, tmp.strip);
        }
        tmp.p += item_w * item_h;
    }
    tmp.p = (tmp.p*100)/(float)(strip_width * tmp.height);
    tmp.fitness = tmp.height;
    print_one_solution(tmp);
    print_strip(tmp);
    getchar();
}

// Calculo del fitness para un conjunto de soluciones
void fitness_calculation(vector<solution> &ss){
    //cout << "fitness_calculation()" << endl;
    for (int i = 0; i < (int)ss.size(); i++)
    {
        fitness_calculation_one(ss[i]);
    }
}



// Generar Soluciones:
//  Aplica un método de generación con diversidad del conjunto
//  de soluciones en "P" de tamaño «popsize».
void solutions_generation(vector<solution> &soluciones)
{
    //cout << "solutions_generation()" << endl;
    // Fixed seed
    srand(123456789*iteration);
    cout << 123456789+iteration << endl;
    soluciones.clear();

    int i, j, k, r;
    int ids[bs.size()];
    int tmp;

    // Sorted sequence of the boxes
    for (i = 0; i < (int)bs.size(); i++) ids[i] = i+1;


    // Shuffle of the boxes order
    for (i = 0; i < popsize; i++) {

        // filling strip with 0
        int **strip_tmp;
        strip_tmp = new int*[strip_width];
        for (j = 0; j < strip_width; j++) {
            strip_tmp[j] = new int[strip_height];
            for (k = 0; k < strip_height; k++) {
                strip_tmp[j][k] = 0;
            }
        }

        solution tmp_sol;
        for (j = 0; j < (int)bs.size(); j++) {
            r = j + (rand() % (bs.size()-j));
            tmp = ids[j];
            ids[j] = ids[r];
            ids[r] = tmp;
        }

        // Adding shuffled boxes to the solutions struct
        for (j = 0; j < (int)bs.size(); j++) {
            tmp_sol.items.push_back(ids[j]);
        }

        // Temp fitness
        tmp_sol.fitness = -1.0;
        tmp_sol.height  = 0;
        tmp_sol.p = 0.0;
        tmp_sol.strip = strip_tmp;

        soluciones.push_back(tmp_sol);
    }
}

// Mejorar Soluciones:
//  Aplica un método de búsqueda local para mejorar las
//  soluciones generadas en el paso previo.
void solutions_improvement(vector<solution> &tmp_sols)
{
    //cout << "solutions_improvement()" << endl;
    int N = (int)bs.size();
    int counter = 0;
    int a, b, tmp, before, after;
    for (int i = 0; i < (int)tmp_sols.size(); i++) {

        // Swap
        before = tmp_sols[i].fitness;
        while(counter < N/4)
        {
            a = (rand() % (N));
            b = (rand() % (N));

            tmp = tmp_sols[i].items[a];
            tmp_sols[i].items[a] = tmp_sols[i].items[b];
            tmp_sols[i].items[b] = tmp;

            fitness_calculation_one(tmp_sols[i]);

            after = tmp_sols[i].fitness;
            if ( after < before){
                counter = N;
            }

            counter++;
        }
        counter = 0;
    }
}

// Construir el conjunto de referencia RefSet:
//  Elegir las mejores «b» soluciones para construir el conjunto RefSet.
//  El conjunto RefSet es un conjunto de «b» soluciones que se usarán para
//  generar nuevas soluciones al combinarlas por algún método.
void refset_build()
{
    //cout << "refset_build()" << endl;
    int i;
    vector<solution> tmp;
    // Ordenar soluciones de la poblacion por el fitness
    for (i = 0; i < popsize; i++) {
        tmp.push_back(sols[i]);
    }
    sort_solutions(tmp);
    // Seleccionar los «b» mejores y hacer push en RefSet.
    for (i = 0; i < b; i++) {
        refset.push_back(tmp[i]);
    }
}

// Inicializar:
//  Identificar la mejor solución en RefSet como la mejor actual.
void save_best_solution(vector<solution> tmp)
{
    //cout << "save_best_solution()" << endl;
    sort_solutions(tmp);
    best = tmp[0];
}



// Combinar Soluciones:
//  Generar intentos de soluciones usando todos los pares de soluciones de RefSet,
//  tal que al menos una solucion de cada una sea nueva.
void solutions_combination()
{
    //cout << "solutions_combination()" << endl;
    int begin, end;
    int N = (int)bs.size();
    int j, tmp_value, new_sols;
    bool new_sol = false;
    vector<int> numbers(N);
    solution p1, p2;
    new_set.clear();
    for (int i = 0; i < (int)refset.size() - 1; i=i+2) {
        p1 = refset[i];
        p2 = refset[i+1];
        new_sol = false;
        new_sols = 0;
        while(!new_sol || new_sols <= b/2){
            begin = (rand() % (N-2));
            end = begin + 2 + (rand() % (N-(begin+2)));

            for (j = begin; j <= end; j++) {
                tmp_value   = p1.items[j];
                p1.items[j] = p2.items[j];
                p2.items[j] = tmp_value;
            }

            fill(numbers.begin(), numbers.end(),0);
            for (j = 0; j < N; j++) {
                if(numbers[p1.items[j]-1] == 0){
                    numbers[p1.items[j]-1] = 1;
                }
                else{
                    p1.items[j] = 0;
                }
            }
            fill(numbers.begin(), numbers.end(),0);
            for (j = 0; j < N; j++) {
                if(numbers[p2.items[j]-1] == 0){
                    numbers[p2.items[j]-1] = 1;
                }
                else{
                    p2.items[j] = 0;
                }
            }

            fill(numbers.begin(), numbers.end(),0);
            for (j = 0; j < N; j++) {
                if(p1.items[j] != 0){
                    numbers[p1.items[j]-1] = 1;
                }
            }
            for (j = 0; j < N; j++) {
                if(numbers[j] == 0){
                    for (int k = 0; k < N; k++) {
                        if(p1.items[k] == 0){
                            p1.items[k] = j + 1;
                            break;
                        }
                    }
                }
            }

            fill(numbers.begin(), numbers.end(),0);
            for (j = 0; j < N; j++) {
                if(p2.items[j] != 0){
                    numbers[p2.items[j]-1] = 1;
                }
            }
            for (j = 0; j < N; j++) {
                if(numbers[j] == 0){
                    for (int k = 0; k < N; k++) {
                        if(p2.items[k] == 0){
                            p2.items[k] = j + 1;
                            break;
                        }
                    }
                }
            }

            // Verificar si las nuevas soluciones estan presentes en refset.
            if(!is_in(p1,refset) || !is_in(p2,refset)){
                new_sol = true;
            }

            new_set.push_back(p1);
            new_set.push_back(p2);

            new_sols += 1;
        }
    }

}

// Modificar el conjunto de referencia (refset)
//  Elegir las mejores «b» soluciones de la unión entre RefSet con el conjunto
//  «b(b-1)/2» de soluciones generadas en el paso anterior.
void refset_modification(vector<solution> tmp_sols)
{
    //cout << "refset_modification()" << endl;

    int i;
    vector<solution> tmp;

    // Adding «new_set» elements
    for (i = 0; i < b*(b-1)/2; i++) {
        tmp.push_back(tmp_sols[i]);
    }

    // Adding previous «refset» elements
    for (i = 0; i < b; i++) {
        tmp.push_back(refset[i]);
    }

    // Sorting «tmp» solutions
    sort_solutions(tmp);

    // Removing all «refset« values
    refset.clear();

    // Adding new «b» «refset» values
    for (i = 0; i < b; i++) {
        refset.push_back(tmp[i]);
    }
}

solution shift_left(solution tmp)
{
    int n = (int)tmp.items.size();
    int value = tmp.items[0];
    for (int i = 0; i < n; i++) {
        tmp.items[i] = tmp.items[i+1];
    }
    tmp.items[n-1] = value;
    return tmp;
}


vector<solution> solutions_improvement2(vector<solution> tmp)
{
    //cout << "solutions_improvement2()" << endl;

    vector<solution> candidates, final;
    solution tmp_sol;
    for (int i = 0; i < tmp.size(); i++) {

        int j = 0, v = 0, diff = 0;
        candidates.clear();
        tmp_sol = tmp[i];
        while(j < (int)tmp_sol.items.size()/3)
        {
            tmp_sol = shift_left(tmp_sol);
            fitness_calculation_one(tmp_sol);
            if(abs(tmp[i].fitness - tmp_sol.fitness) > diff)
            {
                diff = abs(tmp[i].fitness - tmp_sol.fitness);
                v = j;
            }
            j++;
            candidates.push_back(tmp_sol);
        }
        final.push_back(candidates[v]);
    }
    return final;
}

// Reconstruir RefSet:
//  Eliminar las «b/2» soluciones en RefSet.
//  Generar «popsize» soluciones mejoradas aplicando "solutions_generation" y
//  "solutions_improvement".
//  Elegir «b/2» soluciones "diversas" y agregarlas a RefSet.
void refset_rebuild()
{
    //cout << "refset_rebuild()" << endl;
    // eliminamos las b/2 peores.
    sort_solutions(refset);
    refset.erase(refset.end()-(b/2), refset.end());

    // generamos popsize mejores random + busqueda local
    vector<solution> new_sols;
    solutions_generation(new_sols);
    fitness_calculation(new_sols);
    //print_solutions(new_sols);
    //getchar();
    new_sols = solutions_improvement2(new_sols);

    // elegimos las b/2 soluciones mas "malas".
    sort_solutions(new_sols);
    refset.erase(refset.begin(),refset.end()-(b/2));

    for (int i = 0; i < (b/2); i++)
    {
        refset.push_back(new_sols[i]);
    }
}

int get_difference(vector<solution> before, vector<solution> after)
{
    //cout << "get_difference()" << endl;
    int total = 0;
    for (int i = 0; i < b; i++) {
        total += abs(before[i].fitness-after[i].fitness);
    }
    return total;
}
