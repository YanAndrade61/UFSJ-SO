#include "main.h"

int firstAddr(){

	int i = 10;
	while(fat[i] != 0 && i < NUM_CLUSTER) i++;
	if(i == NUM_CLUSTER) return -1;
	return i;
}

data_cluster read_cluster(int block){

    FILE* fp  = fopen(fat_name,"rb");
    data_cluster data;

    if(fp == NULL){
        erro(ENOENT);
        exit(EXIT_FAILURE);
    }

    fseek(fp,CLUSTER_SIZE*block, SEEK_SET);
    fread(&data, sizeof(data), 1, fp);
    fclose(fp);

    return data;
}

void write_cluster(int block, data_cluster data){

    FILE* fp  = fopen(fat_name,"rb+");

    if(fp == NULL){
        erro(ENOENT);
        exit(EXIT_FAILURE);
    }

    fseek(fp,CLUSTER_SIZE*block, SEEK_SET);
    fwrite(&data, sizeof(data), 1, fp);
    fclose(fp);
}

void write_fat(){

    FILE* fp  = fopen(fat_name,"rb+");

    if(fp == NULL){
        erro(ENOENT);
        exit(EXIT_FAILURE);
    }

    fseek(fp,CLUSTER_SIZE, SEEK_SET);
    fwrite(&fat, sizeof(fat), 1, fp);
    fclose(fp);
}

dir_entry_t get_entry(data_cluster cluster, char* file){

	dir_entry_t entry;
	for (int i = 0; i < ENTRY_BY_CLUSTER; i++){
		if(strcmp((char*)cluster.dir[i].filename,file) == 0)
			entry = cluster.dir[i];
	}

	return entry;

}

int put_entry(int block, data_cluster cluster, dir_entry_t entry){

    int free = 0;

	for (int i = 0; i < ENTRY_BY_CLUSTER; i++){
        if(cluster.dir[i].first_block == 0){
            free = 1;
            cluster.dir[i] = entry;
            break;
        }
    }
    write_cluster(block,cluster);
    

	return free;

}

dir_entry_t create_entry(int attr, char* file){

	dir_entry_t entry;
	
	strncpy((char*)entry.filename,file,17*sizeof(char));
	entry.attributes = attr;
	entry.first_block = firstAddr();
	fat[entry.first_block] = 0xffff;

	return entry;
}

dir_entry_t extend_entry(dir_entry_t entry){

	dir_entry_t ex_entry;
	
	strncpy((char*)ex_entry.filename,entry.filename,17*sizeof(char));
	ex_entry.attributes = entry.attributes;
	ex_entry.first_block = firstAddr();
	fat[ex_entry.first_block] = 0xffff;
    fat[entry.first_block] = ex_entry.first_block;

	return ex_entry;
}

int dir_empty(int block){

    int empty = 1;
    data_cluster cluster = read_cluster(block);
    for (int i = 0; i < ENTRY_BY_CLUSTER; i++)
        if(cluster.dir[i].first_block != 0){
            empty = 0;
            break;
        }

    return empty;
}
