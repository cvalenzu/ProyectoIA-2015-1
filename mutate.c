#include "definitions.h"
#include "helpers.h"
#include "operators.h"

#include <stdio.h>

solution mutate_solution(solution sol,BEPinstance instance, double mutate_prob){
	int i,j;
	int evac;
	
	int point,station,shelter;
	int shelter1,shelter2;
	
	int d1,d2;

	int last_shelter_visited;

	//Iterating over buses
	for(i = 0; i < instance.buses; i++){
		//Mutate first solution
		if(mutate_prob > rand_double()){
			//Restoring evacuated people in initial tour
			sol.people_remaining[sol.bus_list[i].starting_tour.point] += sol.bus_list[i].starting_tour.evac;
            sol.capacity_remaining[sol.bus_list[i].starting_tour.shelter] += sol.bus_list[i].starting_tour.evac;

            //Calculating new point and shelter
			station = sol.bus_list[i].starting_tour.station;
			point = randint(instance.points);
			shelter = randint(instance.shelters);

			//Calculating new distances
			d1 = instance.distance_station_matrix[get_index(station,point,instance.shelters)];
			d2 = instance.distance_shelter_matrix[get_index(shelter,point,instance.shelters)];

			//Getting amount of people evacuated
			evac = get_evac(point,shelter,sol,instance);

			//Mutating solution
			sol.bus_list[i].starting_tour.point = point;
			sol.bus_list[i].starting_tour.shelter = shelter;
			sol.bus_list[i].starting_tour.evac = evac;
			sol.bus_list[i].starting_tour.distance = d1+d2;
			sol.people_remaining[point] -= evac;
			sol.capacity_remaining[shelter] -= evac;
		}

		last_shelter_visited = sol.bus_list[i].starting_tour.shelter; 
		for(j = 0; j < sol.bus_list[i].route_length ; j++){
			//Fixing bus flow errors
			if(last_shelter_visited != sol.bus_list[i].route[j].shelter1){
				d1 = instance.distance_shelter_matrix[get_index(sol.bus_list[i].route[j].shelter1,point,instance.shelters)];
				sol.bus_list[i].route[j].distance -= d1;

				d2 = instance.distance_shelter_matrix[get_index(last_shelter_visited,point,instance.shelters)];
				sol.bus_list[i].route[j].distance += d2;

				sol.bus_list[i].route[j].shelter1 = last_shelter_visited;
			}

			if(mutate_prob > rand_double()){
				//Restoring evacuated people in initial tour
				sol.people_remaining[sol.bus_list[i].route[j].point] += sol.bus_list[i].route[j].evac;
	            sol.capacity_remaining[sol.bus_list[i].route[j].shelter2] += sol.bus_list[i].route[j].evac;

	            //Calculating new point and shelter
				shelter1 = last_shelter_visited;
				point = randint(instance.points);
				shelter2 = randint(instance.shelters);

				//Calculating new distances
				d1 = instance.distance_shelter_matrix[get_index(shelter1,point,instance.shelters)];
				d2 = instance.distance_shelter_matrix[get_index(shelter2,point,instance.shelters)];

				//Getting amount of people evacuated
				evac = get_evac(point,shelter2,sol,instance);

				//Mutating solution

				sol.bus_list[i].route[j].point = point;
				sol.bus_list[i].route[j].shelter2 = shelter2;
				sol.bus_list[i].route[j].evac = evac;
				sol.bus_list[i].route[j].distance = d1+d2;
				sol.people_remaining[point] -= evac;
				sol.capacity_remaining[shelter2] -= evac;				
			}

			last_shelter_visited = sol.bus_list[i].route[j].shelter2;
		}

		if((mutate_prob  > rand_double()) && (sol.bus_list[i].route_length < MAX_TOURS)){
			shelter1 = last_shelter_visited;
			point = randint(instance.points);
			shelter2 = randint(instance.shelters);

			evac = get_evac(point,shelter2,sol,instance);

			d1 = instance.distance_shelter_matrix[get_index(shelter1,point,instance.shelters)];
			d2 = instance.distance_shelter_matrix[get_index(shelter2,point,instance.shelters)];

			sol.bus_list[i].route[sol.bus_list[i].route_length].shelter1 = shelter1;
			sol.bus_list[i].route[sol.bus_list[i].route_length].shelter2 = shelter2;
			sol.bus_list[i].route[sol.bus_list[i].route_length].point = point;

			sol.bus_list[i].route[sol.bus_list[i].route_length].evac = evac;
			sol.bus_list[i].route[sol.bus_list[i].route_length].distance = d1 + d2;

			sol.bus_list[i].route_length += 1;
			sol.people_remaining[point] -= evac;
            sol.capacity_remaining[shelter2] -= evac;
		}



	}

	sol.fitness = calculate_fitness(sol,instance);
	return sol;
}

