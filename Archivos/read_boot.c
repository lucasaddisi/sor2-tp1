#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char start_chs[3];
    unsigned char first_byte;
    unsigned char partition_type;
    unsigned char end_chs[3];
    char start_sector[4];
    char length_sectors[4];
} __attribute((packed)) PartitionTable;

typedef struct {
    unsigned char jmp[3];
    char oem[8];
    unsigned short sector_size; // 2 bytes
    unsigned char cluster_size;
    unsigned short reserved_area;
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
    unsigned char fs_type[8]; // Type in ascii
    char boot_code[448];
    unsigned short boot_sector_signature;
} __attribute((packed)) Fat12BootSector;

int main() {
    FILE * in = fopen("test.img", "rb");
    int i;
    PartitionTable pt[4];
    Fat12BootSector bs;

    fseek(in, 0x1BE, SEEK_SET); // Ir al inicio de la tabla de particiones
    fread(pt, sizeof(PartitionTable), 4, in); // leo entradas

    for(i=0; i<4; i++) {
        printf("Partiion type: %d\n", pt[i].partition_type);
        if(pt[i].partition_type == 1) {
            printf("Encontrado FAT12 %d\n", i);
            break;
        }
    }

    if(i == 4) {
        printf("No FAT12 filesystem found, exiting...\n");
        return -1;
    }

    fseek(in, 0, SEEK_SET);
    fread(&bs, sizeof(Fat12BootSector), 1, in);

    printf("  Jump code: %02X:%02X:%02X\n", bs.jmp[0], bs.jmp[1], bs.jmp[2]);
    printf("  OEM code: [%.8s]\n", bs.oem);
    printf("  sector_size: %d\n", bs.sector_size);
	// {...} COMPLETAR
	printf("  Sector per cluster: %d\n", bs.cluster_size);
	printf("  Size of reserved data: %d\n", bs.reserved_area);
	printf("  Numbers of FAT: %d\n", bs.number_fat);
	printf("  Maximum number of files in the root directory: %d\n", bs.max_files_root);
	printf("  Numbers of sector in the files system: %d\n", bs.filesystem_number_sector);
	printf("  Media type: %02X\n", bs.media_type);
	printf("  Size of each FAT in sectors: %d\n", bs.size_fat);
	printf("  Sectors per track in storage device: %d\n", bs.sectors_per_track);
	printf("  Number of head in storage device: %d\n", bs.numbers_heads);
	printf("  Number of sectors before partition: %d\n", bs.sectors_before_partition[0]);
	printf("  Number of sectors in the filesystem: %d\n", bs.sectors_in_filesystem[0]);
	printf("  BIOS INT: %d\n", bs.bios_int);
	printf("  Not used byte: %02X \n", bs.not_used);
	printf("  Extended boot signature: %02X\n", bs.boot_signature);
	//printf("  Volume_id: 0x%08X\n", (unsigned int)bs.volume_id);
    printf("  Volume label: [%.11s]\n", bs.volume_label);
    printf("  Filesystem type: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", bs.fs_type[0],bs.fs_type[1],bs.fs_type[2],bs.fs_type[3],bs.fs_type[4],bs.fs_type[5],bs.fs_type[6],bs.fs_type[7]);
    printf("  Filesystem type: [%.8s]\n", bs.fs_type);
    printf("  Boot sector signature: 0x%04X\n", bs.boot_sector_signature);

    fclose(in);
    return 0;
}
