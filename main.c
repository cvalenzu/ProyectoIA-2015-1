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

	double mutation_prob = 0.5;
	double cross_prob = 0.3;
	double MAX_CROSS_PROB = 0.7;
		
	int j,bus, solut;

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


		while(new_pop_size < pop_size){
			if(cross_prob > rand_double()){
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
				index = select_solution(population, pop_size);
				tmp = mutate_solution(population[index],instance,mutation_prob);
				deep_copy_solution(&tmp, &(new_population[new_pop_size]), instance);
				new_pop_size += 1;
			}
		}

		if(cross_prob < MAX_CROSS_PROB) cross_prob += 0.05;

		tmp_solution = population;
		population = new_population;
		new_population = tmp_solution;

		update_relative_fitness(&population, pop_size);
		update_evac_time(&population, instance,pop_size);

		new_pop_size = 0;


		//////// DELETE
		
		for(j = 0; j < pop_size; j++){
				printf("Solucion: %d, Fitness: %f, Evac time:%d\n",j+1,population[j].fitness,population[j].evac_time);
		}

	}


solut = 0;


printf("Solucion: %d\n", solut);
printf("People Remaining:\n");
for(j = 0; j < instance.points; j++){
	printf("%d ", population[solut].people_remaining[j]);
}
printf("\nCapacity Remaining:\n");
for(j = 0; j < instance.shelters; j++){
	printf("%d ", population[solut].capacity_remaining[j]);
}
printf("\n");

printf("Bus Routes:\n");

for(bus = 0; bus < instance.buses;bus++){
	printf("Bus: %d \n", bus+1);
	printf("S%d -> ",population[solut].bus_list[bus].starting_tour.station);
	printf("P%d",population[solut].bus_list[bus].starting_tour.point);
	printf(":E%d",population[solut].bus_list[bus].starting_tour.evac);
	printf(" -> R%d",population[solut].bus_list[bus].starting_tour.shelter); 

	for(j = 0;j < population[solut].bus_list[bus].route_length; j++){
		printf("-> P%d",population[solut].bus_list[bus].route[j].point);
		printf(":E%d",population[solut].bus_list[bus].route[j].evac);
		printf("-> R%d ",population[solut].bus_list[bus].route[j].shelter);
	}
	printf("\n");
}


	return 0;
}
