/**
* @file
* @brief Definiciones para discos inicializados con esquema BR
*/

#ifndef MBR_H
#define MBR_H

/** @brief Boot sector signature */
#define MBR_SIGNATURE 0xAA55

/** @brief GPT Partition type - MBR */
#define MBR_TYPE_GPT 0xEE

/** @brief Unused partition table - MBR*/
#define MBR_TYPE_UNUSED 0x00

/** @brief Longitud maxima del texto para el tipo de particion */
#define TYPE_NAME_LEN 256

/*** @brief Cantidad de bytes de un sector en disco */
#define SECTOR_SIZE 512

/** @brief Partition descriptor - MBR */
typedef struct {
	/* TODO definir los atributos para la estructura de un descriptor de particion MBR */
	/* La suma completa de bytes de esta estructura debe ser 16 */
	unsigned char flag_boot; /**Boot indicator */
	unsigned char starting_CHS_values[3]; /**Starting CHS values */
	unsigned char partition_type; /**Partition type */
	unsigned char ending_CHS_values[3]; /**Ending CHS values */
	unsigned int starting_LBA; /**Starting LBA */
	unsigned int size_in_LBA; /**Size in LBA */
}__attribute__((packed)) mbr_partition_descriptor;

/** @brief Master Boot Record. */
typedef struct {
	/* TODO definir los atributos para la estructura de datos que representa un MBR (bootsector)*/
	/* La suma completa de bytes de esta estructura debe ser 512 */
	unsigned char contents[446];
	mbr_partition_descriptor partition_table[4];
	unsigned short signature;
	
}__attribute__((packed)) mbr;

/**
* @brief Checks if a bootsector is a MBR.
* @param boot_record Bootsector read in memory]
* @return 0 If the bootsector is a Protective MBR, 1 otherwise.
*/
int is_mbr(mbr * boot_record);

/**
* @brief Text description of a MBR partition type
* @param type Partition type reported in MBR
* @param buf String buffer to store the text description
*/
void mbr_partition_type(unsigned char type, char buf[TYPE_NAME_LEN]);

/**
 * @brief Print the MBR partition descriptor
 * @param partition_descriptor The partition descriptor to print
 */
void print_mbr_partition_descriptor(mbr_partition_descriptor * partition_descriptor);


#endif
