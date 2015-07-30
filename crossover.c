#include <stdlib.h>

#include "definitions.h"
#include "helpers.h"
#include "operators.h"

#include <stdio.h>

//Call one_point_crossover function
solution* crossover_solutions(solution sol1, solution sol2, BEPinstance instance){
	//Add random and one_point_cross when finished
	return one_point_crossover(sol1, sol2, instance);
}

//Select a cutting point for every bus, and mix the solution's routes, setting evacuees to 0
//after mixing all the buses, set the right amount of evacuees, satisfying shelter and bus capacity
//constraints.
solution* one_point_crossover(solution sol1, solution sol2, BEPinstance instance){
	solution *sols;
	int max_cutpoint;
	int cut;
	int *cuts;
	int bus;
	int i;
	int evac;

	//Creating new solutions and cut point array
	sols = (solution*) malloc(sizeof(solution)*2);
	cuts = (int*) malloc(sizeof(int)*instance.buses);

	//Making a copy of parent solutions
	deep_copy_solution(&(sol1), &(sols[0]),instance);
	deep_copy_solution(&(sol2), &(sols[1]),instance);

	for(bus = 0; bus < instance.buses; bus++){

		//Getting the smallest route_length
		if(sol1.bus_list[bus].route_length < sol2.bus_list[bus].route_length){
			max_cutpoint = sol1.bus_list[bus].route_length;
		}
		else{
			max_cutpoint = sol2.bus_list[bus].route_length;
		}

		//Generating cutting point and saving to the array
		cut = randint(max_cutpoint);
		cuts[bus] = cut;
	

		for(i = cut; i < MAX_TOURS; i++){
			//Restoring Evacuees for new solution 1
			sols[0].people_remaining[sol1.bus_list[bus].route[i].point] += sol1.bus_list[bus].route[i].evac;
			sols[0].capacity_remaining[sol1.bus_list[bus].route[i].shelter] += sol1.bus_list[bus].route[i].evac;
		
			//Swapping tours for new solution 2
			sols[1].bus_list[bus].route[i].point = sol1.bus_list[bus].route[i].point;
			sols[1].bus_list[bus].route[i].shelter = sol1.bus_list[bus].route[i].shelter;
			sols[1].bus_list[bus].route[i].evac = 0;

			//Restoring Evacuees for new solution 2
			sols[1].people_remaining[sol2.bus_list[bus].route[i].point] += sol2.bus_list[bus].route[i].evac;
			sols[1].capacity_remaining[sol2.bus_list[bus].route[i].shelter] += sol2.bus_list[bus].route[i].evac;
			
			//Swapping tours for new solution 1
			sols[0].bus_list[bus].route[i].point = sol2.bus_list[bus].route[i].point;
			sols[0].bus_list[bus].route[i].shelter = sol2.bus_list[bus].route[i].shelter;
			sols[0].bus_list[bus].route[i].evac = 0;
		}

		//Changing route length
		sols[0].bus_list[bus].route_length = sol2.bus_list[bus].route_length;
		sols[1].bus_list[bus].route_length = sol1.bus_list[bus].route_length;

	}

	//Evacuating People to offspring solutions
	for(bus = 0; bus < instance.buses; bus++){
		cut = cuts[bus];
		for(i = cut; i < MAX_TOURS ; i++){
			evac = get_evac(sols[0].bus_list[bus].route[i].point,sols[0].bus_list[bus].route[i].shelter, sols[0],instance);
			sols[0].bus_list[bus].route[i].evac = evac;
			sols[0].people_remaining[sols[0].bus_list[bus].route[i].point] -= evac;
			sols[0].capacity_remaining[sols[0].bus_list[bus].route[i].shelter] -= evac;


			evac = get_evac(sols[1].bus_list[bus].route[i].point,sols[1].bus_list[bus].route[i].shelter, sols[1],instance);
			sols[1].bus_list[bus].route[i].evac = evac;
			sols[1].people_remaining[sols[1].bus_list[bus].route[i].point] -= evac;
			sols[1].capacity_remaining[sols[1].bus_list[bus].route[i].shelter] -= evac;

		}

	}

	//Eliminating tours without evacuees
	eliminate_void(&sols[0],instance);
	eliminate_void(&sols[1],instance);

	//Updating fitness
	sols[0].fitness = calculate_fitness(sols[0],instance);
	sols[1].fitness = calculate_fitness(sols[1],instance);
	return sols;
}