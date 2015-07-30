#ifndef HELPERS_HEADER
#define HELPERS_HEADER


// Linealize 2D (x,y) index to 1D (k)
// ----Parameters-----
// x: first dimension index
// y: second dimension index
// width: width of the matrix
// return:
//	index of the (x,y) point in the linealized matrix
int get_index(int x,int y,int width);

//Generate a random value between 0 and max_value
// ----Parameters-----
//max_value: integer bigger than 0
//return:
//	integer [0,max_value-1]
int randint(int max_value);

//Generate a random value between 0 and 1
double rand_double();

//Get the biggest amount of people that can be transported from a point to a shelter
//not violating any capacity constrain (neither shelter or bus capacity).
// ----Parameters-----
// point: index of the starting point
// shelter: index of the shelter we want to evacuate people
// sol: actual solution
// instance: data of problem instance
// return:
//	Biggest value that satisfy all capacity constrains.
int get_evac(int point, int shelter,  solution sol, BEPinstance instance);


//Calculate evacuation_time, with constraint violation penalization
// ---Parameters---
// sol: current solution
// instance: data of the problem
int calculate_evac_time(solution sol, BEPinstance instance);

//Calculate the solution fitness
// ----Parameters-----
// sol: current solution
// instance: data of problem instance
// return:
//	Fitness of the solution, with constrain penalization
double calculate_fitness(solution sol, BEPinstance instance);


//Create an initial population, with a tour from a station to a shelter for every bus and a random route
//that satisfy capacity and people evacuated constraints
// ----Parameters-----
// instance: data of problem instance
// population: list of possible solutions
// pop_size: size of the list.
void initialize_population(BEPinstance instance, solution** population, int pop_size);

//Reads the instance file and initialize the struct
// ----Parameters-----
// filename: path to the file
// instance: reference to BEPinstance structure
// return:
// 	0 if the parse was a sucess
int parse_file(const char* filename, BEPinstance* instance);

//Calculate relative fitness for each solution
//----Parameters-----
// population: list of solutions
// pop_size: size of population 
void update_relative_fitness(solution** population, int pop_size);

//Update the evacuation time for every solution
//---Parameters-----
// population: list of solutions
// instance: data of the problem
void update_evac_time(solution** population, BEPinstance instance, int pop_size);

//Make a copy term by term of a solution
//---Parameters-----
// original: solution to be copied
// copy: output solution
void deep_copy_solution(solution* original, solution* copy, BEPinstance instance);

//Eliminate tours with 0 evacuees
//---Parameters-----
// sol: current solution
// instance: data of problem instance
void eliminate_void(solution* sol, BEPinstance instance);

#endif
