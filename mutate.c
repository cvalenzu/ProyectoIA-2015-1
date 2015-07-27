#include "definitions.h"
#include "helpers.h"
#include "operators.h"

#include <stdio.h>

solution mutate_solution(solution sol,BEPinstance instance, double mutate_prob){
	int i,j;
	
	tour tmp[MAX_TOURS];
	tour tmp_tour;

	int tmp_index = 0;
	int tmp_length = 0;


	int bus1, bus2;
	int route1, route2;
	int tmp_shelter;

	// Deleting tours without evacuees
	for(j= 0; j< MAX_TOURS; j++){
			tmp[j].evac = 0;
			tmp[j].point = 0;
			tmp[j].shelter = 0;
	}

	for(i = 0; i < instance.buses; i++){
		if(sol.bus_list[i].starting_tour.evac == 0){
			sol.bus_list[i].starting_tour.evac =  sol.bus_list[i].route[0].evac;
			sol.bus_list[i].starting_tour.point =  sol.bus_list[i].route[0].point;
			sol.bus_list[i].starting_tour.shelter =  sol.bus_list[i].route[0].shelter;

			sol.bus_list[i].route[0].evac    = 0;
			sol.bus_list[i].route[0].point   = 0;
			sol.bus_list[i].route[0].shelter = 0;
		}

		for(j = 0; j < sol.bus_list[i].route_length; j++){
			if(sol.bus_list[i].route[j].evac != 0){
				tmp[tmp_index].evac = sol.bus_list[i].route[j].evac;
				tmp[tmp_index].point = sol.bus_list[i].route[j].point;
				tmp[tmp_index].shelter = sol.bus_list[i].route[j].shelter;
				tmp_index  += 1;
				tmp_length += 1;
			}
		}
		for(j = 0; j < sol.bus_list[i].route_length ;j++){
			if(j < tmp_length)
				sol.bus_list[i].route[j] = tmp[j];
			else{
				sol.bus_list[i].route[j].evac = 0;
				sol.bus_list[i].route[j].point = 0;
				sol.bus_list[i].route[j].shelter = 0;
			}
		}
		sol.bus_list[i].route_length = tmp_length;

		for(j= 0; j< MAX_TOURS; j++){
			tmp[j].evac = 0;
			tmp[j].point = 0;
			tmp[j].shelter = 0;
		}
		tmp_index = 0;
		tmp_length = 0; 

	}

	//Swapping bus routes
	if(mutate_prob > rand_double()){
		bus1 = randint(instance.buses);
		bus2 = randint(instance.buses);

		route1 = randint(sol.bus_list[bus1].route_length);
		route2 = randint(sol.bus_list[bus2].route_length);

		if(sol.bus_list[bus1].route_length > 1 && sol.bus_list[bus2].route_length > 1){
				tmp_tour = sol.bus_list[bus1].route[route1];
				sol.bus_list[bus1].route[route1] = sol.bus_list[bus2].route[route2];
				sol.bus_list[bus2].route[route2] = tmp_tour;
		}
	}
/*	if(mutate_prob > rand_double()){
		bus1 = randint(instance.buses);

		route1 = randint(sol.bus_list[bus1].route_length);
		route2 = randint(sol.bus_list[bus1].route_length);
		
		if(route1 != route2 && sol.bus_list[bus1].route[route1].evac == sol.bus_list[bus1].route[route2].evac){
			tmp_shelter = sol.bus_list[bus1].route[route1].shelter;
			sol.bus_list[bus1].route[route2].shelter = sol.bus_list[bus1].route[route1].shelter;
			sol.bus_list[bus1].route[route1].shelter = tmp_shelter;
		}

	}
*/
	sol.fitness = calculate_fitness(sol,instance);
	return sol;
}

