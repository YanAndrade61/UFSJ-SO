#include "main.h"

void init(void) {
	FILE* ptr_file;
	int i;
	ptr_file = fopen(fat_name,"wb");
	for (i = 0; i < CLUSTER_SIZE; ++i)
		boot_block[i] = 0xbb;

	fwrite(&boot_block, sizeof(boot_block), 1,ptr_file);

	fat[0] = 0xfffd;
	for (i = 1; i < 9; ++i)
		fat[i] = 0xfffe;

	fat[9] = 0xffff;
	for (i = 10; i < NUM_CLUSTER; ++i)
		fat[i] = 0x0000;

	fwrite(&fat, sizeof(fat), 1, ptr_file);

    memset(root_dir, 0, sizeof(root_dir));
    fwrite(&root_dir, sizeof(root_dir), 1,ptr_file);

    memset(clusters, 0, sizeof(clusters));
	fwrite(&clusters, sizeof(clusters), 1, ptr_file);

	fclose(ptr_file);

	is_load = 1;
}

void load() {
	FILE* ptr_file;
	int i;
	ptr_file = fopen(fat_name, "rb");
	fseek(ptr_file, sizeof(boot_block), SEEK_SET);
	fread(fat, sizeof(fat), 1, ptr_file);
	fread(root_dir, sizeof(root_dir), 1, ptr_file);
	fclose(ptr_file);

	is_load = 1;
}

/*Create arquive depend on attr (0 = file, 1 = directory)*/
int create(char** argv, int argc, int attr){

	if(!is_load){
		printf("erro: Load or init memory first\n");
		return 1;
	}
	if(argc != 2){
		erro(EINVAL);
		return 1;
	}

	int sz = 0;
	char** path = parserStr(argv[1],"/", &sz);
	int block = find_cluster(path,sz-1);
	data_cluster cluster;
	dir_entry_t entry;
	int sucess;

	if(block == -1){
		erro(ENOENT);
		return 1;
	}

	cluster = read_cluster(block);
	for (int i = 0; i < ENTRY_BY_CLUSTER; i++)
		if(strcmp(cluster.dir[i].filename,path[sz-1]) == 0){
			erro(EEXIST);
			return 1;
		}

	entry = create_entry(attr,path[sz-1]);
	sucess = put_entry(block,cluster,entry);
	fat[entry.first_block] = 0xffff;

	if(sucess == 0){
		printf("error: Directory without space");
	}

	write_fat();


	return sucess;
}

int ls(char** argv, int argc){

	if(!is_load){
		printf("erro: Load or init memory first\n");
		return 1;
	}
	if(argc > 2){
		erro(EINVAL);
		return 1;
	}

	int sz = 0;
	char** path = parserStr(argv[1],"/", &sz);
	int block = find_cluster(path,sz);
	data_cluster cluster;
	dir_entry_t entry;
	int free;

	if(block == -1){
		erro(ENOENT);
		return 1;
	}

	cluster = read_cluster(block);
	for (int i = 0; i < ENTRY_BY_CLUSTER; i++)
		if(cluster.dir[i].first_block != 0)
			printf("%s\n",cluster.dir[i].filename);

	return 0;
}

int ulink(char** argv, int argc){

	if(!is_load){
		printf("erro: Load or init memory first\n");
		return 1;
	}
	if(argc != 2){
		erro(EINVAL);
		return 1;
	}

	int sz = 0;
	char** path = parserStr(argv[1],"/", &sz);
	int block = find_cluster(path,sz-1);
	data_cluster cluster;
	dir_entry_t entry;
	uint16_t index;

	if(block == -1){
		erro(ENOENT);
		return 1;
	}

	cluster = read_cluster(block);
	
	for (int i = 0; i < ENTRY_BY_CLUSTER; i++)
		if(strcmp(cluster.dir[i].filename,path[sz-1]) == 0){
			if(cluster.dir[i].attributes == 1 && !dir_empty(cluster.dir[i].first_block)){
				erro(ENOTEMPTY);
				return 1;
			}
			index = cluster.dir[i].first_block;
			cluster.dir[i].first_block = 0;
		}

	do{
		int cp = fat[index]; 
		fat[index] = 0;
		index = cp;
	}while(index != 0xffff);

	write_cluster(block,cluster);

	return 0;
}

int cwrite(char** argv, int argc){

	if(!is_load){
		printf("erro: Load or init memory first\n");
		return 1;
	}
	if(argc != 3){
		erro(EINVAL);
		return 1;
	}

	int sz = 0;
	char** path = parserStr(argv[2],"/", &sz);
	char* string = argv[1];
	int block = find_cluster(path,sz-1);
	data_cluster cluster;
	dir_entry_t entry, ex_entry;
	uint16_t index;

	if(block == -1){
		erro(ENOENT);
		return 1;
	}

	cluster = read_cluster(block);
	
	for (int i = 0; i < ENTRY_BY_CLUSTER; i++)
		if(strcmp(cluster.dir[i].filename,path[sz-1]) == 0){
			if(cluster.dir[i].attributes == 1){
				erro(EISDIR);
				return 1;
			}			
			entry = cluster.dir[i];
			break;
		}

	block = entry.first_block;
	cluster = read_cluster(block);

	int nblocks = ceil((float) strlen(argv[1])/1024.0);
	for (int i = 0; i < nblocks; i++){
		
		strncpy((char*)cluster.data,&string[i*CLUSTER_SIZE],CLUSTER_SIZE);
		write_cluster(block,cluster);
		if(i < nblocks-1){
			ex_entry = extend_entry(entry);
			fat[entry.first_block] = ex_entry.first_block;
			fat[ex_entry.first_block] = 0xffff;
			block = ex_entry.first_block;
			cluster = read_cluster(block);
		}
	}

	
	return 0;
}

int append(char** argv, int argc){

	if(!is_load){
		printf("erro: Load or init memory first\n");
		return 1;
	}
	if(argc != 3){
		erro(EINVAL);
		return 1;
	}

	int sz = 0;
	char** path = parserStr(argv[2],"/", &sz);
	char* string = argv[1];
	int block = find_cluster(path,sz-1);
	data_cluster cluster;
	dir_entry_t entry, ex_entry;
	uint16_t index;

	if(block == -1){
		erro(ENOENT);
		return 1;
	}

	cluster = read_cluster(block);
	
	for (int i = 0; i < ENTRY_BY_CLUSTER; i++)
		if(strcmp(cluster.dir[i].filename,path[sz-1]) == 0){
			if(cluster.dir[i].attributes == 1){
				erro(EISDIR);
				return 1;
			}
			entry = cluster.dir[i];
			break;
		}
	
	block = entry.first_block;
	cluster = read_cluster(block);

	char buffer[8192] = { 0 };
	int len = strnlen((char*)cluster.data,CLUSTER_SIZE);
	memcpy(buffer,cluster.data,len);
	strncat(buffer,string,strlen(argv[1]));

	int nblocks = ceil((float) strlen(buffer)/1024.0);
	for (int i = 0; i < nblocks; i++){
		
		strncpy((char*)cluster.data,&buffer[i*CLUSTER_SIZE],CLUSTER_SIZE);
		write_cluster(block,cluster);
		if(i < nblocks-1){
			ex_entry = extend_entry(entry);
			fat[entry.first_block] = ex_entry.first_block;
			fat[ex_entry.first_block] = 0xffff;
			block = ex_entry.first_block;
			cluster = read_cluster(block);
		}
	}

	
	return 0;
}

int cread(char** argv, int argc){

	if(!is_load){
		printf("erro: Load or init memory first\n");
		return 1;
	}
	if(argc != 2){
		erro(EINVAL);
		return 1;
	}

	int sz = 0;
	char** path = parserStr(argv[1],"/", &sz);
	int block = find_cluster(path,sz-1);
	data_cluster cluster;
	dir_entry_t entry;
	uint16_t index;

	if(block == -1){
		erro(ENOENT);
		return 1;
	}

	cluster = read_cluster(block);
	
	for (int i = 0; i < ENTRY_BY_CLUSTER; i++)
		if(strcmp(cluster.dir[i].filename,path[sz-1]) == 0){
			if(cluster.dir[i].attributes == 1){
				erro(EISDIR);
				return 1;
			}
			index = cluster.dir[i].first_block;
		}

	do{
		cluster = read_cluster(index);
		printf("%s",cluster.data);
		index = fat[index]; 
	}while(index != 0xffff);

	printf("\n");

	return 0;
}

int find_cluster(char **path, int sz){

	data_cluster cluster;
	dir_entry_t entry;
	int block = 9;
	int exist = 1;

	for(int i = 0; path[i] != NULL && i < sz; i++){

		cluster = read_cluster(block);
		entry = get_entry(cluster, path[i]);
		if(entry.first_block == 0){
			exist = 0;
			break;
		}
		block = entry.first_block;
	}

	if(exist) return block;
	else return -1;
}