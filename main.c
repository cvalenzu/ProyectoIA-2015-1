#include <stdio.h>
#include <stdlib.h>

#include "definitions.h"
#include "operators.h"
#include "helpers.h"
#include "operators.h"

//argv structure => ./source_name filepath population_size max_generations
int main(int argc, char* argv[]){
	BEPinstance instance;	
	solution *population;
	solution *new_population;
	solution *tmp_solution;
	solution tmp;
	int pop_size,new_pop_size;
	int generations;
	int i, index, index2;

	double mutation_prob = 0.9;
	double cross_prob = 0.7;
	double r;
	
	int j;

	if(argc < 4){
		return -1;
	}

	pop_size = atoi(argv[2]);
	new_pop_size = 0;
	population = malloc(sizeof(solution) * pop_size);
	new_population = malloc(sizeof(solution)*pop_size);

	parse_file(argv[1],&instance); 
	
	generations = atoi(argv[3]);

	//Using init_seed
	srand(INIT_SEED);
	initialize_population(instance,&population,pop_size);

	srand(GENETIC_SEED);
	printf("Generacion:0\n");
	for(i = 0; i < pop_size; i++){
		printf("Solucion %d Fitness:%f\n", i+1, population[i].fitness);
	
	}	

	for(i = 0; i < generations; i++){
		printf("Generacion:%d\n",i+1);

		tmp = elitist(population,pop_size);
		deep_copy_solution(&tmp, &(new_population[0]), instance);
		new_pop_size += 1;
		//new_population[1] = worsist(population,pop_size);
		//new_pop_size += 1;


		while(new_pop_size < pop_size){
			r = rand_double();
/*			if(cross_prob > r){
				index = select_solution(population, pop_size);
				index2 = select_solution(population, pop_size);

				tmp_solution = crossover_solutions(population[index], population[index2],instance);
				deep_copy_solution(&(tmp_solution[0]), &(new_population[new_pop_size]), instance);
				new_pop_size +=1;
				if(new_pop_size < pop_size){
					deep_copy_solution(&(tmp_solution[1]), &(new_population[new_pop_size]), instance);
					new_pop_size +=1;
				}

			}
			else{
*/				index = select_solution(population, pop_size);
				tmp = mutate_solution(population[index],instance,mutation_prob);
				deep_copy_solution(&tmp, &(new_population[new_pop_size]), instance);
				new_pop_size += 1;
//			}
		}

		tmp_solution = population;
		population = new_population;
		new_population = tmp_solution;

		update_relative_fitness(&population, pop_size);
		update_evac_time(&population, instance,pop_size);

		new_pop_size = 0;
		for(j = 0; j < pop_size; j++){
			printf("Solucion: %d, Fitness: %f, Evac time:%d\n",j+1,population[j].fitness,population[j].evac_time);
		}
	}

	printf("People Remaining:\n");
	for(j = 0; j < instance.points; j++){
		printf("%d ", population[0].people_remaining[j]);
	}
	printf("\nCapacity Remaining:\n");
	for(j = 0; j < instance.shelters; j++){
		printf("%d ", population[0].capacity_remaining[j]);
	}

	printf("\nBus Routes:\n");
	for(i = 0; i < instance.buses;i++){
		printf("Bus: %d \n", i+1);
		for(j = 0;j < population[0].bus_list[i].tour_length; j++){
			if(j==0){
				printf("S%d -> P%d -> R%d",population[0].bus_list[i].starting_tour.station,population[0].bus_list[i].starting_tour.point,population[0].bus_list[i].starting_tour.shelter); 
			}
			else{
				printf("-> R%d -> P%d -> R%d",population[0].bus_list[i].route[j].shelter1,population[0].bus_list[i].route[j].point,population[0].bus_list[i].route[j].shelter2);
			}
		}
		printf("\n");

	}
	return 0;
}
