/**
* @file
* @author Erwin Meza Vega <emezav@unicauca.edu.co>
* @author Miguel Angel Calambas <mangelcvivas@unicauca.edu.co>abort
* @author Esteban Santiago Escadon Causaya <estebanescandon@unicauca.edu.co>
* @brief Definiciones para discos inicializados con esquema GPT
* @copyright MIT License
*/

#ifndef GPT_H
#define GPT_H

#include "mbr.h"


#define GPT_HEADER_SIGNATURE 0x5452415020494645
/**
* @brief GUID
* @see https://uefi.org/specs/UEFI/2.10/Apx_A_GUID_and_Time_Formats.html
*/
typedef struct {
	unsigned int time_lo; /*!< Low field of the timestamp */
	unsigned short time_mid; /*!< Middle field of the timestamp */
	unsigned short time_hi_and_version; /*!< High field of the timestamp and version */
	unsigned char clock_seq_hi_and_reserved; /*!< High field of the clock sequence */
	unsigned char clock_seq_lo; /*!< Low field of the clock sequence */
	unsigned char node[6]; /*!< Spatially unique node identifier */
}__attribute__((packed))guid;

/** @brief GPT Partition Table Header. */
typedef struct {
	unsigned long long signature; 			 /*!< GPT signature */
	unsigned int revision; 					 /*!< Revision */
	unsigned int header_size; 				 /*!< Header size */
	unsigned int header_crc32; 				 /*!< CRC32 of the header */
	unsigned int reserved; 					 /*!< Reserved */
	unsigned long long my_lba; 				 /*!< LBA of the header */
	unsigned long long alternate_lba; 		 /*!< LBA of the alternate header */
	unsigned long long first_usable_lba;	 /*!< First usable LBA for partitions */
	unsigned long long last_usable_lba; 	 /*!< Last usable LBA for partitions */
	guid disk_guid; 						 /*!< Disk GUID */
	unsigned long long partition_entry_lba;  /*!< LBA of the partition table */
	unsigned int num_partition_entries; 	 /*!< Number of partition entries */
	unsigned int size_partition_entry; 		 /*!< Size of a partition entry */
	unsigned int partition_entry_array_crc32;/*!< CRC32 of the partition entry array */
	unsigned char content[420]; 			 /*!< Reserved */
}__attribute__((packed)) gpt_header;

/**
* @brief GPT Partition Entry
*/
typedef struct {
	/* TODO definir los atributos de un descriptor de particion GPT */
	guid partition_type_guid; /*!< Partition type GUID */
	unsigned char unique_partition_guid[16]; /*!< Unique partition GUID */
	unsigned long long starting_lba; /*!< Starting LBA */
	unsigned long long ending_lba; /*!< Ending LBA */
	unsigned long long attributes; /*!< Attributes */
	unsigned char partition_name[72]; /*!< Partition name */
	/*!< Reserved 0 bytes */
}__attribute__((packed)) gpt_partition_descriptor;

/**
* @brief GPT Partition type
*/
typedef struct {
	const char * os; /*!< Operating system */
	const char * description; /*!< Description */
	const char * guid; /*!< GUID */
}gpt_partition_type;

/**
* @brief Text description of a GPT partition type
* @param type Partition type reported in MBR
* @param buf String buffer to store the text description
*/
const gpt_partition_type* get_gpt_partition_type(char * guid_str);

/**
* @brief Decodes a two-byte encoded partition name
* @param name two-byte encoded partition name
*/
char * gpt_decode_partition_name(char name[72]);

/**
* @brief Checks if a bootsector is Protective MBR.
* @param boot_record Bootsector read in memory]
* @return 1 If the bootsector is a Protective MBR, 0 otherwise.
*/
int is_protective_mbr(mbr * boot_record);

/**
* @brief Checks if a GPT header is valid.
* @param hdr Pointer to the GPT header
* @return 1 of hdr is a valid GPT header, 0 otherwise.
*/
int is_valid_gpt_header(gpt_header * hdr);


/**
* @brief Checks if the GPT partition descriptor is null (not used)
* @param desc Descriptor
* @return 1 if the descriptor is null (partition_type_guid = 0), 0 otherwise.
*/
int is_null_descriptor_gpt(gpt_partition_descriptor * desc);


/**
* @brief Creates a human-readable representation of a GUID
* @param buf Buffer containing the GUID
* @return New string with the text representation of the GUID
*/
char * guid_to_str(guid * buf);

/**
 * @brief Prints a GPT header
 */
void print_gpt_header(gpt_header * desc);

#endif
