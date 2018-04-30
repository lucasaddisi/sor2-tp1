#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char first_byte;
    unsigned char start_chs[3];
    unsigned char partition_type;
    unsigned char end_chs[3];
    unsigned short starting_cluster;
    unsigned int file_size;
} __attribute((packed)) PartitionTable;

typedef struct {
    unsigned char jmp[3];
    char oem[8];
    unsigned short sector_size;
    unsigned char cluster_size;
    unsigned short reserved_sectors;
    unsigned char number_fat;
    unsigned short max_files_root;
    unsigned short filesystem_number_sector;
    unsigned char media_type;
    unsigned short size_fat;
    unsigned short sectors_per_track;
    unsigned short numbers_heads;
    unsigned char sectors_before_partition[4];//Probar con long
    unsigned char sectors_in_filesystem[4]; // Probar con long
    unsigned char bios_int;
    unsigned char not_used;
    unsigned char boot_signature;
    char volume_id[4];
    char volume_label[11];
    char fs_type[8];
    char boot_code[448];
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

typedef struct {
    unsigned char first_character;
    unsigned char file_name[10];
    unsigned char file_attributes;
    unsigned char reserved;
    unsigned char creation_time_tenths_seconds;
    unsigned short creation_time;
    unsigned short creation_date;
    unsigned short access_date;
    unsigned short ho_address_first_cluster;
    unsigned short modified_time;
    unsigned short modified_date;
    unsigned short lo_address_first_cluster;
    unsigned long file_size;
	// {...} COMPLETAR
} __attribute((packed)) Fat12Entry;
//
//void print_file_info(Fat12Entry *entry) {
//    switch(entry->filename[0]) {
//    case 0x00:
//        return; // unused entry
//    case 0xE5:
//        printf("Deleted file: [?%.7s.%.3s]\n", // COMPLETAR
//        return;
//    case 0x05:
//        printf("File starting with 0xE5: [%c%.7s.%.3s]\n", 0xE5, // COMPLETAR
//        break;
//    case 0x2E:
//        printf("Directory: [%.8s.%.3s]\n", // COMPLETAR
//        break;
//    default:
//        printf("File: [%.8s.%.3s]\n", // COMPLETAR
//    }
//
//}

int main() {
    FILE * in = fopen("test.img", "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    Fat12Entry entry;

	//{...} Completar

    for(i=0; i<4; i++) {
        if(pt[i].partition_type == 1) {
            printf("Encontrada particion FAT12 %d\n", i);
            break;
        }
    }

    if(i == 4) {
        printf("No encontrado filesystem FAT12, saliendo...\n");
        return -1;
    }

    fseek(in, 0, SEEK_SET);
	//{...} Leo boot sector

    printf("En  0x%X, sector size %d, FAT size %d sectors, %d FATs\n\n",
           ftell(in), bs.sector_size, bs.fat_size_sectors, bs.number_of_fats);

    fseek(in, (bs.reserved_sectors-1 + bs.fat_size_sectors * bs.number_fats) *
          bs.sector_size, SEEK_CUR);

    printf("Root dir_entries %d \n", bs.root_dir_entries);
    for(i=0; i<bs.root_dir_entries; i++) {
        fread(&entry, sizeof(entry), 1, in);
        print_file_info(&entry);
    }

    printf("\nLeido Root directory, ahora en 0x%X\n", ftell(in));
    fclose(in);
    return 0;
}
