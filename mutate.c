#include "definitions.h"
#include "helpers.h"
#include "operators.h"

#include <stdio.h>

solution mutate_solution(solution sol,BEPinstance instance, double mutate_prob){
	int i,j;
	
	tour tmp_tour;


	int bus1, bus2;
	int route1, route2;
	int tmp_shelter;

	//Swapping bus routes
	if(mutate_prob > rand_double()){
		bus1 = randint(instance.buses);
		bus2 = randint(instance.buses);

		route1 = randint(sol.bus_list[bus1].route_length+1);
		route2 = randint(sol.bus_list[bus2].route_length+1);

		if(sol.bus_list[bus1].route_length > 1 && sol.bus_list[bus2].route_length > 1){
				tmp_tour = sol.bus_list[bus1].route[route1];
				sol.bus_list[bus1].route[route1] = sol.bus_list[bus2].route[route2];
				sol.bus_list[bus2].route[route2] = tmp_tour;
		}
	}

	eliminate_void(&sol, instance);
	//Changing tours to random shelter
	for(i = 0; i < instance.buses; i++){
			if(mutate_prob > rand_double()){
				tmp_shelter = randint(instance.shelters);
				sol.bus_list[i].starting_tour.shelter = tmp_shelter;
			}
			for(j = 0; j < sol.bus_list[i].route_length; j++){
				if(mutate_prob > rand_double()){
					tmp_shelter = randint(instance.shelters);
					sol.bus_list[i].route[j].shelter = tmp_shelter;
				}
			}
	}


	sol.fitness = calculate_fitness(sol,instance);
	return sol;
}

