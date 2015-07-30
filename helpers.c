#include <stdlib.h>

#include "definitions.h"
#include "helpers.h"


//Takes x,y and return the index in the 
//linealized matrix
int get_index(int x,int y,int width){
	return x+y*width;
}

//Generate random value between 0 and max_value-1
int randint(int max_value){
	return (int)((max_value)*rand_double());
}

//Generate random value between 0 and 1
double rand_double(){
	return rand()/(double)RAND_MAX;
}

//Get max people that can be evacuated from point to shelter
int get_evac(int point, int shelter,  solution sol, BEPinstance instance){
	int evac = 0;
	//Always take full bus, unless the point has fewer people
	if(sol.people_remaining[point] > instance.bus_capacity){
		evac = instance.bus_capacity;
	}
    else{
    	evac = sol.people_remaining[point];
    }
    //Satisfying shelter capacity constraints
    if(evac > sol.capacity_remaining[shelter]){
        evac = sol.capacity_remaining[shelter];
    }
    return evac;
}

//Calculate evacuation time
int calculate_evac_time(solution sol, BEPinstance instance){
	int distances[instance.buses];
	int distance = 0;
	int people_left = 0;
	int shelter_capacity= 0;


	int last_shelter;
	int max_distance = 0;
	int i,j;

	//Calculating the distance to each bus
	for(i=0; i < instance.buses; i++){
			
		//Saving last shelter for tour flow
		last_shelter =  sol.bus_list[i].starting_tour.shelter;
		distance += instance.distance_station_matrix[get_index(sol.bus_list[i].starting_tour.point, sol.bus_list[i].starting_tour.station, instance.points)];
		distance += instance.distance_shelter_matrix[get_index(sol.bus_list[i].starting_tour.shelter, sol.bus_list[i].starting_tour.point, instance.shelters)];

		for(j = 0; j < sol.bus_list[i].route_length ; j++){
			//Iterating over tours
			distance += instance.distance_shelter_matrix[get_index(last_shelter, sol.bus_list[i].route[j].point, instance.shelters)];
			distance += instance.distance_shelter_matrix[get_index(sol.bus_list[i].route[j].shelter, sol.bus_list[i].route[j].point, instance.shelters)];
			last_shelter = sol.bus_list[i].route[j].shelter;
		}

		distances[i] = distance;
		distance = 0;
	}

	//Looking for the biggest_distance
	for(i = 0; i < instance.buses; i++){
		if(distances[i] > max_distance)
			max_distance = distances[i];
	}

	//Calculating the people that still in the points
	//after the evacuation
	for(i = 0; i < instance.points; i++){
		if(sol.people_remaining[i] >= 0) people_left += sol.people_remaining[i];
		else people_left -= sol.people_remaining[i];
	}
	
	//Calculating shelters with more people than its
	//capacity
	for(i = 0; i < instance.shelters;i++){
		if(sol.capacity_remaining[i] < 0){
			shelter_capacity += -1*sol.capacity_remaining[i]; 
		}
	}
	return (max_distance + PEOPLE_VIOLATION * people_left + CAPACITY_VIOLATION * shelter_capacity);

}

//Calculate the fitness value for a solution
double calculate_fitness(solution sol, BEPinstance instance){
	//Using big constants for constraint violation
	return (double) 1/calculate_evac_time(sol,instance);
}

//Create the first population where all solutions have buses with one initial tour from a
// station to a shelter. The buses are created satisfying the station bus capacity constraint.
void initialize_population(BEPinstance instance, solution** population, int pop_size){
	int station;
	int point;
	int shelter;

	int bus;
	int evac;

	int index;

	int i,j,k;

	double sum_fitness = 0;
	double fitness;
	
	//Iterating solutions
	for(i = 0; i < pop_size; i++){	
		bus = 0;
		(*population)[i].bus_list = (bus_tour*) malloc(sizeof(bus_tour)*instance.buses);
		(*population)[i].people_remaining = (int*) malloc(sizeof(int)*instance.points);
		(*population)[i].capacity_remaining = (int*) malloc(sizeof(int)*instance.shelters);
		(*population)[i].fitness = 0;

		for(j = 0; j < instance.points; j++){
			(*population)[i].people_remaining[j] = instance.people_per_point[j];
		}

		for(j = 0; j < instance.shelters; j++){
			(*population)[i].capacity_remaining[j] = instance.capacity_per_shelter[j];
		}

		for(j = 0; j < instance.buses; j++){
			for(k = 0; k <  MAX_TOURS; k++){
				(*population)[i].bus_list[j].route[k].point = 0;
				(*population)[i].bus_list[j].route[k].shelter = 0;
				(*population)[i].bus_list[j].route[k].evac = 0;
			}

			(*population)[i].bus_list[j].route_length = 0;

		}

		//Iterating stations
		for(j = 0; j < instance.stations; j++){
			
			//Bus to station assingation	
			for(k = 0; k < instance.bus_per_station[j]; k++){ 
				//Start in a station with buses
				station = j;
				evac = 0;
				while(evac == 0){
					//Random point and shelter
					point = randint(instance.points);
					shelter = randint(instance.shelters);
				
					//Creating starting_tour
					(*population)[i].bus_list[bus].starting_tour.station = station;
					(*population)[i].bus_list[bus].starting_tour.point = point;
					(*population)[i].bus_list[bus].starting_tour.shelter = shelter;
				
					//Calculating evacuation people
					evac = get_evac(point,shelter, (*population)[i], instance);
					(*population)[i].bus_list[bus].starting_tour.evac = evac;
				}

				//Updating lists
				(*population)[i].people_remaining[point] -= evac;
				(*population)[i].capacity_remaining[shelter] -= evac;
				
				bus += 1;	
			}
		}
		//Evacuating the remaining people
		for(j = 0; j < instance.points; j++){
			while((*population)[i].people_remaining[j]  > 0){
				//Get an available bus
				index = MAX_TOURS;
				while(index >= MAX_TOURS){
					bus = randint(instance.buses);
					index = (*population)[i].bus_list[bus].route_length;
				}
				for(k = 0; k < instance.shelters; k++){
					evac = 0;
					evac = get_evac(j,k,(*population)[i],instance);
					if(evac != 0) break;
				}
				(*population)[i].bus_list[bus].route[index].point = j;
				(*population)[i].bus_list[bus].route[index].shelter = k;
				(*population)[i].bus_list[bus].route[index].evac = evac;
				(*population)[i].bus_list[bus].route_length += 1;
				(*population)[i].people_remaining[j] -= evac;
				(*population)[i].capacity_remaining[k] -= evac;
			}
		}
		


		//Calculating solution initial fitness
		fitness = calculate_fitness((*population)[i],instance);
		sum_fitness += fitness;
		(*population)[i].fitness = fitness;	
	}

	for(i = 0; i< pop_size;i++){
		(*population)[i].relative_fitness = (double) (*population)[i].fitness / (double) sum_fitness;
	}
		

}

//Update relative fitness
void update_relative_fitness(solution** population, int pop_size){
	int i;
	double sum_fitness = 0;
	for(i = 0; i < pop_size; i++){
		sum_fitness += (*population)[i].fitness;
	}

	for(i = 0;i < pop_size; i++){
		(*population)[i].relative_fitness = (double) (*population)[i].fitness / (double) sum_fitness;
	}
}

//Update evac times
void update_evac_time(solution** population, BEPinstance instance, int pop_size){
	int i;
	int evac_time;
	for(i=0;i<pop_size;i++){
		evac_time = calculate_evac_time((*population)[i], instance);
		(*population)[i].evac_time = evac_time;
	}
}

//Copy a solution 
void deep_copy_solution(solution* original, solution* copy, BEPinstance instance){
	int bus;
	int i;

	(*copy).fitness = (*original).fitness;
	(*copy).relative_fitness = (*original).relative_fitness;
	(*copy).evac_time = (*original).evac_time;

	(*copy).bus_list = (bus_tour*) malloc(sizeof(bus_tour)*instance.buses);
	(*copy).people_remaining = (int*) malloc(sizeof(int)*instance.points);
	(*copy).capacity_remaining = (int*) malloc(sizeof(int)*instance.shelters);

	for(i = 0; i < instance.points; i++){
		(*copy).people_remaining[i] = (*original).people_remaining[i];
	}
	for(i = 0; i < instance.shelters; i++){
		(*copy).capacity_remaining[i] = (*original).capacity_remaining[i];
	}
	for(bus = 0; bus < instance.buses; bus++){
		(*copy).bus_list[bus].starting_tour = (*original).bus_list[bus].starting_tour;
		(*copy).bus_list[bus].route_length = (*original).bus_list[bus].route_length;
		for(i = 0; i < MAX_TOURS; i++){
			(*copy).bus_list[bus].route[i] = (*original).bus_list[bus].route[i];

		}
	}


}

//Remove tours without evacuees
void eliminate_void(solution* sol, BEPinstance instance){
	tour tmp[MAX_TOURS];
	int i,j;
	int tmp_index = 0;
	int tmp_length = 0;

	//Initializing temporary array
	for(j= 0; j< MAX_TOURS; j++){
			tmp[j].evac = 0;
			tmp[j].point = 0;
			tmp[j].shelter = 0;
	}


	for(i = 0; i < instance.buses; i++){
		//Looking starting tour evacuees
		if((*sol).bus_list[i].starting_tour.evac == 0){
			(*sol).bus_list[i].starting_tour.evac =  (*sol).bus_list[i].route[0].evac;
			(*sol).bus_list[i].starting_tour.point =  (*sol).bus_list[i].route[0].point;
			(*sol).bus_list[i].starting_tour.shelter =  (*sol).bus_list[i].route[0].shelter;

			(*sol).bus_list[i].route[0].evac    = 0;
			(*sol).bus_list[i].route[0].point   = 0;
			(*sol).bus_list[i].route[0].shelter = 0;
		}
		//Copying tours with evacuees to temporary array
		for(j = 0; j < MAX_TOURS; j++){
			if((*sol).bus_list[i].route[j].evac != 0){
				tmp[tmp_index].evac = (*sol).bus_list[i].route[j].evac;
				tmp[tmp_index].point = (*sol).bus_list[i].route[j].point;
				tmp[tmp_index].shelter = (*sol).bus_list[i].route[j].shelter;
				tmp_index  += 1;
				tmp_length += 1;
			}
		}
		//Copying back tours to solution
		for(j = 0; j < MAX_TOURS ;j++){
			if(j < tmp_length)
				(*sol).bus_list[i].route[j] = tmp[j];
			else{
				(*sol).bus_list[i].route[j].evac = 0;
				(*sol).bus_list[i].route[j].point = 0;
				(*sol).bus_list[i].route[j].shelter = 0;
			}
		}
		//Updating route length
		(*sol).bus_list[i].route_length = tmp_length;

		//Cleaning temporary variables for next bus
		for(j= 0; j< MAX_TOURS; j++){
				tmp[j].evac = 0;
				tmp[j].point = 0;
				tmp[j].shelter = 0;
		}
		tmp_index = 0;
		tmp_length = 0;
	}
}