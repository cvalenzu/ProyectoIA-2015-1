#ifndef OPERATORS_HEADER
#define OPERATORS_HEADER

//Mutate solution, adding a new tour to a bus or changing a assigned tour to 
//a random one.
// ------Variables ------
// sol: Current solution
// instance: data of the problem
// mutate_prob: probability of an tour to be mutated. Number between 0 and 1. 
solution mutate_solution(solution sol,BEPinstance instance,double mutate_prob);



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
solution* one_point_crossover(solution sol1, solution sol2, BEPinstance instance);

solution* bus_tour_swapping(solution sol1, solution sol2, BEPinstance instance);

//Select the best solution
// -----Variables -------
// populations: list of solutions
solution elitist(solution* population, int pop_size);

//Select the worst solution
// -----Variables -------
// populations: list of solutions
solution worsist(solution* population, int pop_size);

//Select using roulette
//------Variables ------
// population: list of solutions
// return:
//	Index of selected solution
int select_solution(solution* population, int pop_size);

#endif

