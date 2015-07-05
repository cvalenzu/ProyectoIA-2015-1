#include "definitions.h"
#include "helpers.h"
#include "operators.h"

#include <stdio.h>

solution mutate_solution(solution sol,BEPinstance instance, double mutate_prob){
	int i,j;
	int evac;
	
	int point;
	int shelter;
	
	tour tmp[MAX_TOURS];
	int tmp_index = 0;
	int tmp_length = 0;

	//Iterating over buses
	for(i = 0; i < instance.buses; i++){
		//Mutate first solution
		if(mutate_prob > rand_double()){
			//Restoring evacuated people in initial tour
			sol.people_remaining[sol.bus_list[i].starting_tour.point] += sol.bus_list[i].starting_tour.evac;
            sol.capacity_remaining[sol.bus_list[i].starting_tour.shelter] += sol.bus_list[i].starting_tour.evac;

            //Calculating new point and shelter
			point = randint(instance.points);
			shelter = randint(instance.shelters);

			//Getting amount of people evacuated
			evac = get_evac(point,shelter,sol,instance);

			//Mutating solution
			sol.bus_list[i].starting_tour.point = point;
			sol.bus_list[i].starting_tour.shelter = shelter;
			sol.bus_list[i].starting_tour.evac = evac;
			sol.people_remaining[point] -= evac;
			sol.capacity_remaining[shelter] -= evac;
		}

		for(j = 0; j < sol.bus_list[i].route_length ; j++){
			if(mutate_prob > rand_double()){
				//Restoring evacuated people in initial tour
				sol.people_remaining[sol.bus_list[i].route[j].point] += sol.bus_list[i].route[j].evac;
	            sol.capacity_remaining[sol.bus_list[i].route[j].shelter] += sol.bus_list[i].route[j].evac;

	            //Calculating new point and shelter
				point = randint(instance.points);
				shelter = randint(instance.shelters);

				//Getting amount of people evacuated
				evac = get_evac(point,shelter,sol,instance);

				//Mutating solution

				sol.bus_list[i].route[j].point = point;
				sol.bus_list[i].route[j].shelter = shelter;
				sol.bus_list[i].route[j].evac = evac;
				sol.people_remaining[point] -= evac;
				sol.capacity_remaining[shelter] -= evac;				
			}

		}

		if((mutate_prob  > rand_double()) && (sol.bus_list[i].route_length < MAX_TOURS)){
			point = randint(instance.points);
			shelter = randint(instance.shelters);

			evac = get_evac(point,shelter,sol,instance);

			sol.bus_list[i].route[sol.bus_list[i].route_length].shelter = shelter;
			sol.bus_list[i].route[sol.bus_list[i].route_length].point = point;

			sol.bus_list[i].route[sol.bus_list[i].route_length].evac = evac;

			sol.bus_list[i].route_length += 1;
			sol.people_remaining[point] -= evac;
            sol.capacity_remaining[shelter] -= evac;
		}



	}


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


	sol.fitness = calculate_fitness(sol,instance);
	return sol;
}

