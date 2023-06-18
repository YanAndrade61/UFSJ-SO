#ifndef __FAT_H__
#define __FAT_H__

/*DEFINE*/
#define SECTOR_SIZE	     512
#define CLUSTER_SIZE     2*SECTOR_SIZE
#define ENTRY_BY_CLUSTER CLUSTER_SIZE/sizeof(dir_entry_t)
#define NUM_CLUSTER	     4096
#define fat_name	     "fat.part"

struct _dir_entry_t {
	char 	 filename[18];
	uint8_t  attributes;
	uint8_t  reserved[7];
	uint16_t first_block;
	uint32_t size;
};

typedef struct _dir_entry_t  dir_entry_t;

union _data_cluster {
	dir_entry_t dir[ENTRY_BY_CLUSTER];
	uint8_t 	data[CLUSTER_SIZE];
};

typedef union _data_cluster data_cluster;

/*DATA DECLARATION*/
static uint16_t 	fat[NUM_CLUSTER];
static uint8_t 		boot_block[CLUSTER_SIZE];
static dir_entry_t 	root_dir[ENTRY_BY_CLUSTER];
static data_cluster clusters[4086];


int 		  find_cluster(char **path, int sz);
int 		  put_entry(int block, data_cluster cluster, dir_entry_t entry);
int 		  dir_empty(int block);
void 		  write_fat();
void 		  write_cluster(int index, data_cluster data);
data_cluster  read_cluster(int index);
dir_entry_t   get_entry(data_cluster cluster, char* file);
dir_entry_t   create_entry(int attr, char* file);
dir_entry_t   extend_entry(dir_entry_t entry);



#endif