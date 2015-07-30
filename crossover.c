#include <stdlib.h>

#include "definitions.h"
#include "helpers.h"
#include "operators.h"

#include <stdio.h>
solution* crossover_solutions(solution sol1, solution sol2, BEPinstance instance){
	//Add random and one_point_cross when finished
	return one_point_crossover(sol1, sol2, instance);
}


solution* one_point_crossover(solution sol1, solution sol2, BEPinstance instance){
	solution *sols;
	int max_cutpoint;
	int cut;
	int bus;
	int i;
	int evac;

	sols = (solution*) malloc(sizeof(solution)*2);

	deep_copy_solution(&(sol1), &(sols[0]),instance);
	deep_copy_solution(&(sol2), &(sols[1]),instance);

	for(bus = 0; bus < instance.buses; bus++){
		if(sol1.bus_list[bus].route_length < sol2.bus_list[bus].route_length){
			max_cutpoint = sol1.bus_list[bus].route_length;
		}
		else{
			max_cutpoint = sol2.bus_list[bus].route_length;
		}

		cut = randint(max_cutpoint);
		if(bus == 1) printf("Sol1 len = %d Sol2 len = %d CUT = %d\n",sol1.bus_list[bus].route_length,sol2.bus_list[bus].route_length,cut);


		for(i = cut; i < sol1.bus_list[bus].route_length; i++){
			//Restoring Evacuees for new solution 1
			sols[0].people_remaining[sol1.bus_list[bus].route[i].point] += sol1.bus_list[bus].route[i].evac;
			sols[0].capacity_remaining[sol1.bus_list[bus].route[i].shelter] += sol1.bus_list[bus].route[i].evac;
		
			//Swapping tours for new solution 2
			sols[1].bus_list[bus].route[i].point = sol1.bus_list[bus].route[i].point;
			sols[1].bus_list[bus].route[i].shelter = sol1.bus_list[bus].route[i].shelter;
			sols[1].bus_list[bus].route[i].evac = 0;

		}

		for(i = cut; i < sol2.bus_list[bus].route_length; i++){
			//Restoring Evacuees for new solution 2
			sols[1].people_remaining[sol2.bus_list[bus].route[i].point] += sol2.bus_list[bus].route[i].evac;
			sols[1].capacity_remaining[sol2.bus_list[bus].route[i].shelter] += sol2.bus_list[bus].route[i].evac;
			
			//Swapping tours for new solution 1
			sols[0].bus_list[bus].route[i].point = sol2.bus_list[bus].route[i].point;
			sols[0].bus_list[bus].route[i].shelter = sol2.bus_list[bus].route[i].shelter;
			sols[0].bus_list[bus].route[i].evac = 0;
		}

		//Chaging route length
		sols[0].bus_list[bus].route_length = sol2.bus_list[bus].route_length;
		sols[1].bus_list[bus].route_length = sol1.bus_list[bus].route_length;

	}

	//Evacuating People to offspring solutions
	for(bus = 0; bus < instance.buses; bus++){
		for(i = cut; i < sols[0].bus_list[bus].route_length ; i++){
			evac = get_evac(sols[0].bus_list[bus].route[i].point,sols[0].bus_list[bus].route[i].shelter, sols[0],instance);
			sols[0].bus_list[bus].route[i].evac = evac;
			sols[0].people_remaining[sols[0].bus_list[bus].route[i].point] -= evac;
			sols[0].capacity_remaining[sols[0].bus_list[bus].route[i].shelter] -= evac;
		}

		for(i = cut; i < sols[1].bus_list[bus].route_length ; i++){
			evac = get_evac(sols[1].bus_list[bus].route[i].point,sols[1].bus_list[bus].route[i].shelter, sols[1],instance);
			sols[1].bus_list[bus].route[i].evac = evac;
			sols[1].people_remaining[sols[1].bus_list[bus].route[i].point] -= evac;
			sols[1].capacity_remaining[sols[1].bus_list[bus].route[i].shelter] -= evac;

		}

	}

	//Eliminating tours without evacuees
//	eliminate_void(&sols[0],instance);
//	eliminate_void(&sols[1],instance);

	sols[0].fitness = calculate_fitness(sols[0],instance);
	sols[1].fitness = calculate_fitness(sols[1],instance);
	return sols;
}