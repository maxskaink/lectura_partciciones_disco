/**
 * @file
 * @author Erwin Meza Vega <emezav@unicauca.edu.co>
 * @author Miguel Angel Calambas <mangelcvivas@unicauca.edu.co>abort
 * @author Esteban Santiago Escadon Causaya <estebanescandon@unicauca.edu.co>
 * @brief Listar particiones de discos duros MBR/GPT
 * @copyright MIT License
*/

#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mbr.h"
#include "gpt.h"


/**
* @brief Hex dumps a buffer
* @param buf Pointer to buffer
* @param size Buffer size
*/
void hex_dump(char * buf, size_t size);

/**
* @brief ASCII dumps a buffer
* @param buf Pointer to buffer
* @param size Buffer size
*/
void ascii_dump(char * buf, size_t size);

/**
 * @brief Reads a sector from a disk
 * 
 * @param disk Disk filename
 * @param iba Sector to reado (9 - amount of LBA sector on disk)
 * @param buf Buffer to read the sector i nto
 * @return 0 if success, -0 on failure
 */
int read_lba_sector(char * disk, unsigned long long lba, char buf[SECTOR_SIZE]);

/**
 * @brief Prints usage information
 */
void usage();

int main(int argc, char *argv[]) {

	// 1. Validar los argumentos de l inea de comandos
	if ( argc != 2 ){
		fprintf(stderr, "Invalid arguments\n", argv[0]);
		usage();
		exit(EXIT_FAILURE);
	}

	// 2. Leer el primer sector sector del disco especificado
	char * disk;
	mbr boot_record;
	disk = argv[1];
	// 2.1 Si la lectura falla, imprimir error y terminar
	if(read_lba_sector(disk, 0, (char*)&boot_record) == 0){
		fprintf(stderr, "Unable to open device %s\n", disk);
		exit(EXIT_FAILURE); // o continue si se usa estructura repetitiva
	}
	// PRE: Se pudo leer el primer secto del disco
	// 3. Imprimmir la tabla de particiones del MBR leido!
	print_mbr_partition_descriptor(boot_record.partition_table);
	//hex_dump((char*)&boot_record, sizeof(mbr));
	// 4. Si el esquema de paricionado es MBR: terminado
	unsigned int flag_is_protective_mbr = is_protective_mbr(&boot_record);

	if( flag_is_protective_mbr )
		printf("Disk initialized as GPT\n");
	else 
		printf("Disk initialized as MBR\n");

	if( !flag_is_protective_mbr )
		exit(EXIT_SUCCESS);
	// PRE: El esquema de particionado es GPT
	// 5 . Imprimir la tabla de particiones GPT
	// 5.1 Leer el segundo sector del disco (PTHDR - Encabezado de la tabla de particiones GPT)
	gpt_header second_sector_gpt_header;;
	if( read_lba_sector(disk, 1, (char*)&second_sector_gpt_header) == 0){
		fprintf(stderr, "Unable to open device %s\n", disk);
		exit(EXIT_FAILURE); 
	}
	//Validar que sesa valido el encabezado GPT
	if( is_valid_gpt_header(&second_sector_gpt_header) ){
		fprintf(stderr, "Invalid GPT header\n");
		exit(EXIT_FAILURE);
	}
	// En el PTHDR se encuentra la cantidad de descriptores de la tabla
	print_gpt_header(&second_sector_gpt_header);
	int cantidad_descriptores = second_sector_gpt_header.num_partition_entries;
	int cantidad_sectores_descriptores = cantidad_descriptores/4;
	// 5.2 Reptetir:
	printf("Start LBA       End LBA         Size              Type                                Partition Name\n");
	printf("--------------- --------------- ---------------   ----------------------------------- -----------------------------------\n");
	for(size_t i=0; i< cantidad_sectores_descriptores;i++){
		// 5.2.1 Leer un sector que contiene descriptores de particion GPT
		gpt_partition_descriptor partition_descriptor[4];
		if( read_lba_sector(disk, 2 + i, (char*)&partition_descriptor) == 0){
			fprintf(stderr, "Unable to open device %s\n", disk);
			exit(EXIT_FAILURE); 
		}
		// 5.2.2 Para cada descriptor leido, imprimir su informacion
		for(size_t j=0; j<4; j++){
			if(is_null_descriptor_gpt(&partition_descriptor[j]))
				continue;
			printf("%15u %15u %15llu %35s %35s\n", 
				partition_descriptor[j].starting_lba, 
				partition_descriptor[j].ending_lba, 
				((partition_descriptor[j].ending_lba - partition_descriptor[j].starting_lba)*(unsigned long long )(SECTOR_SIZE)), 
				get_gpt_partition_type(guid_to_str(&partition_descriptor[j].partition_type_guid))->description, 
				gpt_decode_partition_name(partition_descriptor[j].partition_name)
			);
		}
	}
	printf("--------------- --------------- ---------------   ----------------------------------- -----------------------------------\n");

	return 0;
}


int read_lba_sector(char * disk, unsigned long long lba, char buf[SECTOR_SIZE]) {
	/* TODO leer un sector de 512 bytes del disco especificado en el buffer */

	FILE * fp;
	
	// Abrir el dispositivo en modo lectura
	fp = fopen(disk, "r");
	
	if( fp == NULL)
		return 0;
	
	// Leer el sector del disco
	if( fseek(fp, lba * SECTOR_SIZE, SEEK_SET) != 0 )
		return 0;

	if( fread(buf, 1, SECTOR_SIZE, fp) != SECTOR_SIZE )
		return 0;


	fclose(fp);
	return 1;
}

void ascii_dump(char * buf, size_t size) {
	for (size_t i = 0; i < size; i++) {
		if (buf[i] >= 0x20 && buf[i] < 0x7F) {
			printf("%c", buf[i]);
		}else {
			printf(".");
		}
	}
}

void hex_dump(char * buf, size_t size) {
	int cols;
	cols = 0;
	for (size_t i=0; i < size; i++) {
		printf("%02x ", buf[i] & 0xff);
		if (++cols % 16 == 0) {
			ascii_dump(&buf[cols - 16], 16);
			printf("\n");
		}
	}
}

void usage(){
	printf("Usage: \n diskinfo <disk>\n");
}

