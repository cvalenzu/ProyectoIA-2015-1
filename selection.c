#include "definitions.h"
#include "helpers.h"
#include "operators.h"


//Select the fittest solution
solution elitist(solution* population, int pop_size){
	int i;
	double max_fitness = 0.0;
	solution sol;
	for(i = 0; i < pop_size; i++){
		if(population[i].fitness > max_fitness){
			max_fitness = population[i].fitness;
			sol = population[i];
		}
	}
	return sol;
}

//Select the fattiest solution
solution worsist(solution* population, int pop_size){
	int i;
	double min_fitness = 1000000000000000;
	solution sol;
	for(i = 0; i < pop_size; i++){
		if(population[i].fitness < min_fitness){
			min_fitness = population[i].fitness;
			sol = population[i];
		}
	}
	return sol;
}


//Select a solution to use mutation or crossover
//using roulette
int select_solution(solution* population, int pop_size){
	int i;
	double offset = 0.0;
	double r = rand_double();
	int pick = 0;
	
	for (i = 0; i < pop_size; i++) {
    	offset += population[i].relative_fitness;
    	if (r < offset) {
        	pick = i;
        	break;
    	}
	}
	return pick;
}

