#ifndef __FAT_H__
#define __FAT_H__

/*INCLUDE*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <inttypes.h>

/*DEFINE*/
#define SECTOR_SIZE	     512
#define CLUSTER_SIZE     2*SECTOR_SIZE
#define ENTRY_BY_CLUSTER CLUSTER_SIZE/sizeof(dir_entry_t)
#define NUM_CLUSTER	     4096
#define fat_name	     "fat.part"

struct _dir_entry_t {
	char filename[18];
	uint8_t attributes;
	uint8_t reserved[7];
	uint16_t first_block;
	uint32_t size;
};

typedef struct _dir_entry_t  dir_entry_t;

union _data_cluster {
	dir_entry_t dir[ENTRY_BY_CLUSTER];
	uint8_t data[CLUSTER_SIZE];
};

typedef union _data_cluster data_cluster;

/*DATA DECLARATION*/
static uint16_t fat[NUM_CLUSTER];
static uint8_t boot_block[CLUSTER_SIZE];
static dir_entry_t root_dir[ENTRY_BY_CLUSTER];
static data_cluster clusters[4086];


void init(void);
void load();

#endif