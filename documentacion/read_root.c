#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    unsigned char number_of_fats;
    unsigned short root_dir_entries;
    unsigned short filesystem_number_sector;
    unsigned char media_type;
    unsigned short fat_size_sectors;
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
    unsigned char file_size[4];
} __attribute((packed)) Fat12Entry;
//
void print_file_info(Fat12Entry *entry) {
    switch(entry->first_character) {
    case 0x00:
        return; // unused entry
    case 0xE5:
        printf("Deleted file: [?%.7s.%.3s], Type of File [0x%02X]\n",entry->file_name, entry->file_name + 7, entry->file_attributes);
        return;
    case 0x05:
        printf("File starting with 0xE5: [%c%.7s.%.3s]\n", 0xE5,entry->file_name, entry->file_name + 7); // COMPLETAR
       break;
    case 0x2E:
        printf("Directory: [%c%.7s.%.3s]\n",entry->first_character,entry->file_name, entry->file_name+7); // Probado, funciona pero cambiamos el formato original
        break;
    default:
        printf("File: [%c%.10s], Type of File [0x%02X]]\n",entry->first_character,entry->file_name,entry->file_attributes);/// COMPLETAR
   }

}

int main() {
    FILE * in = fopen("test.img", "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;
    Fat12Entry entry;

    fseek(in, 0x1BE, SEEK_SET); // Voy al inicio...
    fread(&pt[0], sizeof(PartitionTable), 4, in);

    for(i=0; i<4; i++) {
        printf("%d\n",pt[i].partition_type);
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
    fread(&bs, sizeof(Fat12BootSector), 1, in);

    printf("En  0x%X, sector size %d, FAT size %d sectors, %d FATs\n\n",
           (unsigned int)ftell(in), bs.sector_size, bs.fat_size_sectors, bs.number_of_fats);

    fseek(in, (bs.reserved_sectors-1 + bs.fat_size_sectors * bs.number_of_fats) * //multiplico la cantidad de tablas de fat por la cantidad de sectores que integran la tabla fat sumado a los sectores reservador para ir al root directory entry
          bs.sector_size, SEEK_CUR);

    printf("Root dir_entries %d \n", bs.root_dir_entries);
    for(i=0; i<bs.root_dir_entries; i++) {
        fread(&entry, sizeof(entry), 1, in);
        print_file_info(&entry);

    }

    printf("\nLeido Root directory, ahora en 0x%X\n", (unsigned int)ftell(in));
    fclose(in);
    return 0;
}
