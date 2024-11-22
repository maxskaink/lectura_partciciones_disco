/**
 * @file
 * @author Erwin Meza Vega <emezav@unicauca.edu.co>
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

/*** @brief Cantidad de bytes de un sector en disco */
#define SECTOR_SIZE 512

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


int main(int argc, char *argv[]) {

	char * disk;
	// 1. Validar los argumentos de l inea de comandos
	// 2. Leer el primer sector sector del disco especificado
	mbr boot_record;
	disk = argv[1];
	// 2.1 Si la lectura falla, imprimir error y terminar
	if(read_lba_sector(disk, 0, (char*)&boot_record) == 0){
		fprintf(stderr, "Unable to open device %s\n", disk);
		exit(EXIT_FAILURE); // o continue si se usa estructura repetitiva
	}
	// PRE: Se pudo leer el priemr secto del disco
	// 3. Imprimmir la tabla de particiones del MBR leido!
	printf("size of mbr %d\n", sizeof(mbr));
	hex_dump((char*)&boot_record, sizeof(mbr));

	// 4. Si el esquema de paricionado es MBR: terminado
	// PRE: El esquema de particionado es GPT
	// 5 . Imprimir la tabla de particiones GPT
	// 5.1 Leer el segundo sector del disco (PTHDR - Encabezado de la tabla de particiones GPT)
	// Em el PTHDR se encuentra la cantidad de descriptores de la tabla
	// 5.2 Reptetir:
	// 5.2.1 Leer un sector que contiene descriptores de particion GPT
	// 5.2.2 Para cada descriptor leido, imprimir su informacion

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



