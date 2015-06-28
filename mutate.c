#include "definitions.h"
#include "helpers.h"
#include "operators.h"

solution mutate_solution(solution sol,BEPinstance instance, double mutate_prob){
	int i,j, route_index;
	int evac;
	int station,point,shelter;
	int shelter1,shelter2;
	int d1,d2;
	initial_tour tmp_initial;
	tour tmp_tour;
	tour *tmp_route;

	int sum_fitness;
	//Iterating over the buses
	for(i = 0; i < instance.buses;i++){

		//Iterating over the bus tours
		for(j = 0; j < sol.bus_list[i].tour_length; j++){
			if(mutate_prob > rand_double()){
				//Mutate the initial tour
				if(j==0){
					station = sol.bus_list[i].starting_tour.station;
                	point = randint(instance.points);
                	if(sol.bus_list[i].tour_length == 1){
                		shelter = randint(instance.shelters);
                	}
                	else{
                		shelter = sol.bus_list[i].route[0].shelter1;
                	}
					
					sol.people_remaining[sol.bus_list[i].starting_tour.point] += sol.bus_list[i].starting_tour.evac;
                    sol.capacity_remaining[sol.bus_list[i].starting_tour.shelter] += sol.bus_list[i].starting_tour.evac;

					evac = get_evac(point,shelter,sol,instance);

					tmp_initial.station = station;
                	tmp_initial.point = point;
                	tmp_initial.shelter = shelter;

                	d1 = instance.distance_station_matrix[get_index(point,station,instance.points)];
                	d2 = instance.distance_shelter_matrix[get_index(shelter,point,instance.shelters)];

                	tmp_initial.distance = d1 + d2;
					tmp_initial.evac = evac;

					sol.bus_list[i].starting_tour = tmp_initial;
					sol.people_remaining[point] -= evac;
					sol.capacity_remaining[shelter] -= evac;

								
				}
				//Mutate a tour after the first one
				else{

					route_index = j-1;
					//Looking for the last shelter
					if(route_index == 0){
						shelter1 = sol.bus_list[i].starting_tour.shelter;
					}
					else{
						shelter1 = sol.bus_list[i].route[route_index-1].shelter2;
					}
					if(route_index == sol.bus_list[i].tour_length - 1){
						shelter2 = randint(instance.shelters);
					}
					else{
						shelter2 = sol.bus_list[i].route[route_index+1].shelter1;
					}

					point = randint(instance.points);
					
					
					sol.people_remaining[sol.bus_list[i].route[route_index].point] += sol.bus_list[i].route[route_index].evac;
                    sol.capacity_remaining[sol.bus_list[i].route[route_index].shelter2] += sol.bus_list[i].route[route_index].evac;

                	evac = get_evac(point,shelter2,sol,instance);

					tmp_tour.shelter1 = shelter1;
					tmp_tour.shelter2 = shelter2;
					tmp_tour.point = point;

					d1 = instance.distance_shelter_matrix[get_index(shelter1,point,instance.shelters)];
                    d2 = instance.distance_shelter_matrix[get_index(shelter2,point,instance.shelters)];

					tmp_tour.distance = d1 + d2;
					tmp_tour.evac = evac;
					
					sol.bus_list[i].route[route_index] = tmp_tour;
					sol.people_remaining[point] -= evac;
					sol.capacity_remaining[shelter2] -= evac;
				}
			}
		}
		if((mutate_prob > rand_double()) && (sol.bus_list[i].tour_length < MAX_TOURS)){
			route_index = j-1;
			//Add new tour
            if(route_index == 0){
                 shelter1 = sol.bus_list[i].starting_tour.shelter;
             }
            else{
                 shelter1 = sol.bus_list[i].route[route_index-1].shelter2;
             }

			point = randint(instance.points);
            shelter2 = randint(instance.shelters);
			evac = get_evac(point,shelter2,sol,instance);
	
			tmp_tour.shelter1 = shelter1;
			tmp_tour.shelter2 = shelter2;
			tmp_tour.point = point;
			
            d1 = instance.distance_shelter_matrix[get_index(shelter1,point,instance.shelters)];
            d2 = instance.distance_shelter_matrix[get_index(shelter2,point,instance.shelters)];

			tmp_tour.distance = d1+d2;
			tmp_tour.evac = evac;
			
			sol.bus_list[i].route[sol.bus_list[i].tour_length-1] = tmp_tour;
			sol.bus_list[i].tour_length += 1;
			sol.people_remaining[point] -= evac;
            sol.capacity_remaining[shelter2] -= evac;
            
		}
		

	}
	sol.fitness = calculate_fitness(sol,instance);
	return sol;
}

