#include <stdio.h>
#include <stdlib.h>

#include "definitions.h"
#include "helpers.h"

//Read the file in filename path, and fill the BEPinstancce structure
int parse_file(const char* filename, BEPinstance* instance){
	FILE* fid;
	int i,j,tmp;


	fid = fopen(filename,"r");
	if (fid  == NULL ){
		printf("File not found\n");
		return -1;
	}
	
	fscanf(fid, "%d: %d", &((*instance).buses), &((*instance).bus_capacity));
	fscanf(fid, "%d:", &((*instance).stations));
	
	(*instance).bus_per_station =(int*) malloc(sizeof(int)* (*instance).buses); 
	
	for(i = 0 ; i <  (*instance).stations;i++){
		fscanf(fid, " %d", &((*instance).bus_per_station[i]));
	}


	fscanf(fid,"%d:", &((*instance).points));
	fscanf(fid,"%d:", &((*instance).max_people));
	
	(*instance).people_per_point = (int*) malloc(sizeof(int) * (*instance).points);
 
	for(i = 0; i< (*instance).points; i++){
		fscanf(fid,"%d ", &((*instance).people_per_point[i]));
	}

	fscanf(fid, "%d:", &((*instance).shelters));
        fscanf(fid,"%d:", &((*instance).max_capacity));

	
	(*instance).capacity_per_shelter = (int*) malloc(sizeof(int) * (*instance).shelters);
	
	for(i = 0; i < (*instance).shelters; i++){
		fscanf(fid, "%d ", &((*instance).capacity_per_shelter[i]));
	}


	(*instance).distance_station_matrix = (int*) malloc(sizeof(int) * (*instance).stations * (*instance).points);
	
	for(i = 0; i < (*instance).stations ; i++){
		fscanf(fid, "%d:", &tmp);
		for(j = 0; j <  (*instance).points ; j++){
			tmp = get_index(j,i,(*instance).points);
			fscanf(fid,"%d ", &((*instance).distance_station_matrix[tmp])); 
	
		}

	}

        (*instance).distance_shelter_matrix = (int*) malloc(sizeof(int) * (*instance).shelters * (*instance).points);

        for(i = 0; i < (*instance).points ; i++){
                fscanf(fid, "%d:", &tmp);
                for(j = 0; j <  (*instance).shelters ; j++){
                        tmp = get_index(j,i,(*instance).shelters);
                        fscanf(fid,"%d ", &((*instance).distance_shelter_matrix[tmp])); 
        
                }

        }	
	return 0;

}

