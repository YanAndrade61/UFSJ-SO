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
}

void load() {
	FILE* ptr_file;
	int i;
	ptr_file = fopen(fat_name, "rb");
	fseek(ptr_file, sizeof(boot_block), SEEK_SET);
	fread(fat, sizeof(fat), 1, ptr_file);
	fread(root_dir, sizeof(root_dir), 1, ptr_file);
	fclose(ptr_file);
}

/*Create arquive depend on attr (0 = file, 1 = directory)*/
int create(char** argv, int attr){

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
	entry = create_entry(attr,path[sz-1]);
	sucess = put_entry(block,cluster,entry);
	if(sucess == 0){
		printf("error: Directory without space");
	}
    write_fat();


	return sucess;
}

int ls(char** argv){

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