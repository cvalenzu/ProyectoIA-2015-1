#include <stdlib.h>

#include "definitions.h"
#include "helpers.h"
#include "operators.h"

#include <stdio.h>
solution* crossover_solutions(solution sol1, solution sol2, BEPinstance instance){
	//Add random and one_point_cross when finished
	//return one_point_crossover(sol1, sol2, instance);
	return bus_tour_swapping(sol1, sol2, instance);

}


/*
solution* one_point_crossover(solution sol1, solution sol2, BEPinstance instance){
	solution *sols;
	int max_cutpoint;
	int max_route;
	int cut;
	int bus;
	int i;

	int d1,d2;

	sols = (solution*) malloc(sizeof(solution)*2);

	deep_copy_solution(&(sol1), &(sols[0]),instance);
	deep_copy_solution(&(sol2), &(sols[1]),instance);

	for(bus = 0; bus < instance.buses; bus++){
		if(sol1.bus_list[bus].route_length > sol2.bus_list[bus].route_length){
			max_route = sol1.bus_list[bus].route_length;
			max_cutpoint = sol2.bus_list[bus].route_length;
		}
		else{
			max_route = sol2.bus_list[bus].route_length;
			max_cutpoint = sol1.bus_list[bus].route_length;
		}
		cut = randint(max_cutpoint);

		sols[0].bus_list[bus].route_length = sol2.bus_list[bus].route_length;
		sols[1].bus_list[bus].route_length = sol1.bus_list[bus].route_length;	
	
		//Copy routes and changing route_length 
		for(i = cut; i < max_route; i++){
			//Restoring people remaining and shelter capacity
			sols[0].people_remaining[sols[0].bus_list[bus].route[i].point] += sols[0].bus_list[bus].route[i].evac;
			sols[1].people_remaining[sols[1].bus_list[bus].route[i].point] += sols[1].bus_list[bus].route[i].evac;


			sols[0].capacity_remaining[sols[0].bus_list[bus].route[i].shelter2] += sols[0].bus_list[bus].route[i].evac;
			sols[1].capacity_remaining[sols[1].bus_list[bus].route[i].shelter2] += sols[1].bus_list[bus].route[i].evac;
		}
		printf("Se restaura la custion \n");
		printf("max_route %d \n", max_route);
		for(i = cut; i < max_route; i++){
			//Mixing routes
			sols[0].bus_list[bus].route[i] = sol2.bus_list[bus].route[i];
			sols[1].bus_list[bus].route[i] = sol1.bus_list[bus].route[i];

			//
			sols[0].people_remaining[sols[0].bus_list[bus].route[i].point] -= sols[0].bus_list[bus].route[i].evac;
			sols[1].people_remaining[sols[1].bus_list[bus].route[i].point] -= sols[1].bus_list[bus].route[i].evac;


			sols[0].capacity_remaining[sols[0].bus_list[bus].route[i].shelter2] -= sols[0].bus_list[bus].route[i].evac;
			sols[1].capacity_remaining[sols[1].bus_list[bus].route[i].shelter2] -= sols[1].bus_list[bus].route[i].evac;
		}
		printf("Se custionean las rutas \n");
		if(cut != 0){
			for(i = 0; i < 2;i++){
				if(sols[i].bus_list[bus].route[cut-1].shelter2 != sols[i].bus_list[bus].route[cut].shelter1){
					d1 = instance.distance_shelter_matrix[get_index(sols[i].bus_list[bus].route[cut].shelter1,sols[i].bus_list[bus].route[cut].point,instance.shelters)];
					sols[i].bus_list[bus].route[cut].distance -= d1;

					d2 = instance.distance_shelter_matrix[get_index(sols[i].bus_list[bus].route[cut-1].shelter2,sols[i].bus_list[bus].route[cut].point,instance.shelters)];
					sols[i].bus_list[bus].route[cut].distance += d2;

					sols[i].bus_list[bus].route[cut].shelter1 = sols[i].bus_list[bus].route[cut-1].shelter2;

				}
			}
			printf("Se maneja el flujo de la custion \n");
		}

	}

	sols[0].fitness = calculate_fitness(sols[0], instance);
	sols[1].fitness = calculate_fitness(sols[1], instance);
	sols[0].evac_time = calculate_evac_time(sols[0],instance);
	sols[1].evac_time = calculate_evac_time(sols[1],instance);


	return sols;
}*/

solution* bus_tour_swapping(solution sol1, solution sol2, BEPinstance instance){
	solution *sols;
	
	int bus;
	int i;
	int index;

	sols = (solution*) malloc(sizeof(solution)*2);

	deep_copy_solution(&(sol1), &(sols[0]),instance);
	deep_copy_solution(&(sol2), &(sols[1]),instance);

	bus = randint(instance.buses);

	for(index = 0; index < 2; index++){
		sols[index].people_remaining[sols[index].bus_list[bus].starting_tour.point] += sols[index].bus_list[bus].starting_tour.evac; 
		sols[index].capacity_remaining[sols[index].bus_list[bus].starting_tour.shelter] += sols[index].bus_list[bus].starting_tour.evac; 

		for(i= 0; i < sols[index].bus_list[bus].route_length;i++){
			sols[index].people_remaining[sols[index].bus_list[bus].route[i].point] += sols[index].bus_list[bus].route[i].evac; 
			sols[index].capacity_remaining[sols[index].bus_list[bus].route[i].shelter] += sols[index].bus_list[bus].route[i].evac; 
		}
	}

	sols[0].bus_list[bus] = sol2.bus_list[bus];
	sols[1].bus_list[bus] = sol1.bus_list[bus];

	for(index = 0; index < 2; index++){
		sols[index].people_remaining[sols[index].bus_list[bus].starting_tour.point] -= sols[index].bus_list[bus].starting_tour.evac; 
		sols[index].capacity_remaining[sols[index].bus_list[bus].starting_tour.shelter] -= sols[index].bus_list[bus].starting_tour.evac; 

		for(i= 0; i < sol1.bus_list[bus].route_length;i++){
			sols[index].people_remaining[sols[index].bus_list[bus].route[i].point] -= sols[index].bus_list[bus].route[i].evac; 
			sols[index].capacity_remaining[sols[index].bus_list[bus].route[i].shelter] -= sols[index].bus_list[bus].route[i].evac; 
		}
	}


	return sols;


}