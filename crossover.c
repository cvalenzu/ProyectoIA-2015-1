#include <stdlib.h>

#include "definitions.h"
#include "helpers.h"
#include "operators.h"

solution* crossover_solutions(solution sol1, solution sol2, BEPinstance instance){
	solution *sols;
	int max_cutpoint;
	int max_tour;
	int cut;
	int bus;
	int i;

	int evac;
	int shelter1;
	int shelter2;
	int point;
	int d1,d2;

	sols = (solution*) malloc(sizeof(solution)*2);

	deep_copy_solution(&(sol1), &(sols[0]),instance);
	deep_copy_solution(&(sol2), &(sols[1]),instance);

	for(bus = 0; bus < instance.buses; bus++){
		if(sol1.bus_list[bus].tour_length > sol2.bus_list[bus].tour_length){
			max_tour = sol1.bus_list[bus].tour_length;
			max_cutpoint = sol2.bus_list[bus].tour_length;
		}
		else{
			max_tour = sol2.bus_list[bus].tour_length;
			max_cutpoint = sol1.bus_list[bus].tour_length;
		}
		cut = randint(max_cutpoint);

		sols[0].bus_list[bus].tour_length = sol2.bus_list[bus].tour_length;
		sols[1].bus_list[bus].tour_length = sol1.bus_list[bus].tour_length;

		for(i = cut; i < max_tour; i++){
			if(i==0){
				sols[0].bus_list[bus].starting_tour = sol2.bus_list[bus].starting_tour;
				sols[1].bus_list[bus].starting_tour = sol1.bus_list[bus].starting_tour;

				//Restoring people as never been evacuated
				sols[0].people_remaining[sols[0].bus_list[bus].starting_tour.point] +=  sol1.bus_list[bus].starting_tour.evac;
				sols[1].people_remaining[sols[1].bus_list[bus].starting_tour.point] +=  sol2.bus_list[bus].starting_tour.evac;
				
				sols[0].capacity_remaining[sols[0].bus_list[bus].starting_tour.shelter] +=  sol1.bus_list[bus].starting_tour.evac;
				sols[1].capacity_remaining[sols[1].bus_list[bus].starting_tour.shelter] +=  sol2.bus_list[bus].starting_tour.evac;

				//Evacuate the people
				sols[0].capacity_remaining[sols[0].bus_list[bus].starting_tour.shelter] -=  sol2.bus_list[bus].starting_tour.evac;
				sols[1].capacity_remaining[sols[1].bus_list[bus].starting_tour.shelter] -=  sol1.bus_list[bus].starting_tour.evac;
				
				sols[0].people_remaining[sols[0].bus_list[bus].starting_tour.point] -=  sol2.bus_list[bus].starting_tour.evac;
				sols[1].people_remaining[sols[1].bus_list[bus].starting_tour.point] -=  sol1.bus_list[bus].starting_tour.evac;

			}
			else{
				sols[0].bus_list[bus].route[i-1] = sol2.bus_list[bus].route[i-1];
				sols[1].bus_list[bus].route[i-1] = sol1.bus_list[bus].route[i-1];

				//Restoring people as never been evacuated
				sols[0].people_remaining[sols[0].bus_list[bus].route[i-1].point] +=  sol1.bus_list[bus].route[i-1].evac;
				sols[1].people_remaining[sols[1].bus_list[bus].route[i-1].point] +=  sol2.bus_list[bus].route[i-1].evac;

				sols[0].capacity_remaining[sols[0].bus_list[bus].route[i-1].shelter2] +=  sol1.bus_list[bus].starting_tour.evac;
				sols[1].capacity_remaining[sols[1].bus_list[bus].route[i-1].shelter2] +=  sol2.bus_list[bus].starting_tour.evac;


				//Evacuate the people
				if(i != cut){
					sols[0].people_remaining[sols[0].bus_list[bus].route[i-1].point] -=  sol2.bus_list[bus].route[i-1].evac;
					sols[1].people_remaining[sols[1].bus_list[bus].route[i-1].point] -=  sol1.bus_list[bus].route[i-1].evac;

					sols[0].capacity_remaining[sols[0].bus_list[bus].starting_tour.shelter] -=  sol2.bus_list[bus].starting_tour.evac;
					sols[1].capacity_remaining[sols[1].bus_list[bus].starting_tour.shelter] -=  sol1.bus_list[bus].starting_tour.evac;
				}
				else{
					sols[0].bus_list[bus].route[i-1].shelter1 = sol2.bus_list[bus].route[i-2].shelter2;
					sols[1].bus_list[bus].route[i-1].shelter1 = sol1.bus_list[bus].route[i-2].shelter2;


					//Changing sol1 cut point
					shelter1 = sols[0].bus_list[bus].route[i-1].shelter1; 
					shelter2 = sols[0].bus_list[bus].route[i-1].shelter2;
					point = sols[0].bus_list[bus].route[i-1].point;

					evac = get_evac(point,shelter2,sols[0],instance);
					d1 = instance.distance_shelter_matrix[get_index(shelter1,point,instance.shelters)];
                    d2 = instance.distance_shelter_matrix[get_index(shelter2,point,instance.shelters)];

					sols[0].bus_list[bus].route[i-1].distance = d1 + d2;
					sols[0].bus_list[bus].route[i-1].evac = evac;

					//Chaning sol2 cut point
					shelter1 = sols[1].bus_list[bus].route[i-1].shelter1; 
					shelter2 = sols[1].bus_list[bus].route[i-1].shelter2;
					point = sols[1].bus_list[bus].route[i-1].point;

					evac = get_evac(point,shelter2,sols[1],instance);
					d1 = instance.distance_shelter_matrix[get_index(shelter1,point,instance.shelters)];
                    d2 = instance.distance_shelter_matrix[get_index(shelter2,point,instance.shelters)];

					sols[1].bus_list[bus].route[i-1].distance = d1 + d2;
					sols[1].bus_list[bus].route[i-1].evac = evac;


				}
			}
		}

	}

	sols[0].fitness = calculate_fitness(sols[0], instance);
	sols[1].fitness = calculate_fitness(sols[1], instance);

	return sols;
}