#ifndef OPERATORS_HEADER
#define OPERATORS_HEADER

// Have a probability to swap two random tours from two random selected buses and 
// a probability to randomly change the arrival shelter to each tour. 
// ------Variables ------
// sol: Current solution
// instance: data of the problem
// mutate_prob: probability of an tour to be mutated. Number between 0 and 1.

// SOURCE CODE: mutate.c 
solution mutate_solution(solution sol,BEPinstance instance,double mutate_prob);


//Uses one_point_crossover
//(The function below)

// SOURCE CODE: crossover.c
solution* crossover_solutions(solution sol1, solution sol2, BEPinstance instance);

//One point crossover
//Select one point for every bus, creating 
//a random crossover between the solutions
//------ Variables---
// sol1: first solution
// sol2: second solution
// instance: data of the problem
// return:
// 		list with two new solutions

// SOURCE CODE: crossover.c
solution* one_point_crossover(solution sol1, solution sol2, BEPinstance instance);

//Select the best solution
// -----Variables -------
// populations: list of solutions

// SOURCE CODE: selection.c
solution elitist(solution* population, int pop_size);

//Select a solution using roulette
//------Variables ------
// population: list of solutions
// return:
//	Index of selected solution

// SOURCE CODE: selection.c 
int select_solution(solution* population, int pop_size);

#endif

