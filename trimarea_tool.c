/*
 *  By munjeni @ 2016
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>

#define ENABLE_DEBUG 1

/*
/////////////////////////////////////////////////////////////////////////////////

			FOLOWING USEFULL INFORMATION IS FROM
         https://github.com/dosomder/ta-info/blob/master/TAInfo/trimarea.h

/////////////////////////////////////////////////////////////////////////////////
*/

//partition types
#define TRIMAREA_PARTITION_TRIM 1
#define TRIMAREA_PARTITION_MISC 2

//TA units
//When a modem is flashed, nv units are read from trim partition
//most of nv units are overwritten by the baseband config located in amss_fsg or in system/etc/customization/modem/
#define TA_UNIT_1_RF_BC_CFG 0x755
#define TA_UNIT_1_LTE_BC_CFG 0x1AAC

#define TA_UNIT_2_FLA_FLA 0x7D2 //what is flafla?
#define TA_UNIT_2_S1_LDR 0x7D3 //hw conf
#define TA_UNIT_2_SENS_DATA 0x7DA //simlock / bootloader unlock allowed, etc.
#define TA_UNIT_2_DRM_KEY_STATUS 0x7E5
#define TA_UNIT_2_BLOB_0 0x7E6 //marlin
#define TA_UNIT_2_BLOB_1 0x7E7 //ckb
#define TA_UNIT_2_BLOB_2 0x7E8 //widevine
#define TA_UNIT_2_BLOB_3 0x7E9
#define TA_UNIT_2_BLOB_4 0x7EA
#define TA_UNIT_2_BLOB_5 0x7EB
#define TA_UNIT_2_BLOB_6 0x7EC
#define TA_UNIT_2_BLOB_7 0x7ED
#define TA_UNIT_2_BLOB_8 0x7EE
#define TA_UNIT_2_BLOB_9 0x7EF
#define TA_UNIT_2_BLOB_A 0x7F0
#define TA_UNIT_2_BLOB_B 0x7F1
#define TA_UNIT_2_BLOB_C 0x7F2
#define TA_UNIT_2_BLOB_D 0x7F3
#define TA_UNIT_2_BLOB_E 0x7F4
#define TA_UNIT_2_SRM 0x7F8
#define TA_UNIT_2_LAST_BOOT_LOG 0x802
#define TA_UNIT_2_SENS_DATA_SIGNATURE 0x851	//signature of the unit 0x7DA
#define TA_UNIT_2_MACHINE_ID 0x85D
#define TA_UNIT_2_SW_VER 0x89A
#define TA_UNIT_2_CUST_VER 0x89D
#define TA_UNIT_2_FS_VER 0x89E
#define TA_UNIT_2_S1_BOOT_VER 0x89F
#define TA_UNIT_2_BUILD_TYPE 0x8A1
#define TA_UNIT_2_PHONE_NAME 0x8A2
#define TA_UNIT_2_AC_VER 0x8A4 //cust-reset.ta zeroes this
#define TA_UNIT_2_BL_UNLOCKCODE 0x8B2
#define TA_UNIT_2_STARTUP_SHUTDOWNRESULT 0x8B3
#define TA_UNIT_2_STARTUP_REASON 0x8FD //"override unit"
#define TA_UNIT_2_DISABLE_CHARGE_ONLY 0x907
#define TA_UNIT_2_OSV_RESTRICTION 0x91A
#define TA_UNIT_2_MODEM_CUST_CFG 0x964 //FOTA_INTERNAL ?? cfg located in system/etc/customization/modem/ -> fota-reset.ta zeroes this
#define TA_UNIT_2_ENABLE_SERIAL_CONSOLE 0x9A9	//value 1 for enable serial console or value 0 (default) to disable (https://forum.xda-developers.com/showpost.php?p=80212371&postcount=1125)
#define TA_UNIT_2_FLASH_LOG 0x9AB
#define TA_UNIT_2_ENABLE_NONSECURE_USB_DEBUG 0x9B6
#define TA_UNIT_2_CREDMGR_KEYTABLE_PRESET 0x9C4
#define TA_UNIT_2_BASEBAND_CFG 0x9F7 //cfg located in the modem
#define TA_UNIT_2_WIFI_MAC 0xA00
#define TA_UNIT_2_BLUETOOTH_MAC 0xA08
#define TA_UNIT_2_SERIAL_NO 0x1324
#define TA_UNIT_2_PBA_ID 0x1325
#define TA_UNIT_2_PBA_ID_REV 0x1326
#define TA_UNIT_2_PP_SEMC_ITP_PRODUCT_NO 0x132C
#define TA_UNIT_2_PP_SEMC_ITP_REV 0x132D
#define TA_UNIT_2_DEVICE_KEY 0x1046B
#define TA_UNIT_2_REMOTE_LOCK 0x1046C //a sin file
#define TA_UNIT_2_GOOGLE_LOCK 0x1046F	//google lock ( allow bootloader unlock in dev settings )
#define TA_UNIT_2_PROTOCOL_SWITCH 0x10471	//dev_id HMAC key. Depend on existance of unit 0x874 (https://forum.xda-developers.com/showpost.php?p=82983507&postcount=1628)
#define TA_UNIT_2_BOOT_COUNTER 0x805
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

const char *name(int value)
{
#define NAME(UNITNAME) case UNITNAME: return #UNITNAME;
	switch (value)
	{
		NAME(TA_UNIT_1_RF_BC_CFG)
		NAME(TA_UNIT_1_LTE_BC_CFG)
		//
		NAME(TA_UNIT_2_FLA_FLA)
		NAME(TA_UNIT_2_S1_LDR)
		NAME(TA_UNIT_2_SENS_DATA)
		NAME(TA_UNIT_2_DRM_KEY_STATUS)
		NAME(TA_UNIT_2_BLOB_0)
		NAME(TA_UNIT_2_BLOB_1)
		NAME(TA_UNIT_2_BLOB_2)
		NAME(TA_UNIT_2_BLOB_3)
		NAME(TA_UNIT_2_BLOB_4)
		NAME(TA_UNIT_2_BLOB_5)
		NAME(TA_UNIT_2_BLOB_6)
		NAME(TA_UNIT_2_BLOB_7)
		NAME(TA_UNIT_2_BLOB_8)
		NAME(TA_UNIT_2_BLOB_9)
		NAME(TA_UNIT_2_BLOB_A)
		NAME(TA_UNIT_2_BLOB_B)
		NAME(TA_UNIT_2_BLOB_C)
		NAME(TA_UNIT_2_BLOB_D)
		NAME(TA_UNIT_2_BLOB_E)
		NAME(TA_UNIT_2_SRM)
		NAME(TA_UNIT_2_LAST_BOOT_LOG)
		NAME(TA_UNIT_2_SENS_DATA_SIGNATURE)
		NAME(TA_UNIT_2_MACHINE_ID)
		NAME(TA_UNIT_2_SW_VER)
		NAME(TA_UNIT_2_CUST_VER)
		NAME(TA_UNIT_2_FS_VER)
		NAME(TA_UNIT_2_S1_BOOT_VER)
		NAME(TA_UNIT_2_BUILD_TYPE)
		NAME(TA_UNIT_2_PHONE_NAME)
		NAME(TA_UNIT_2_AC_VER)
		NAME(TA_UNIT_2_BL_UNLOCKCODE)
		NAME(TA_UNIT_2_STARTUP_SHUTDOWNRESULT)
		NAME(TA_UNIT_2_STARTUP_REASON)
		NAME(TA_UNIT_2_DISABLE_CHARGE_ONLY)
		NAME(TA_UNIT_2_OSV_RESTRICTION)
		NAME(TA_UNIT_2_MODEM_CUST_CFG)
		NAME(TA_UNIT_2_ENABLE_SERIAL_CONSOLE)
		NAME(TA_UNIT_2_FLASH_LOG)
		NAME(TA_UNIT_2_ENABLE_NONSECURE_USB_DEBUG)
		NAME(TA_UNIT_2_CREDMGR_KEYTABLE_PRESET)
		NAME(TA_UNIT_2_BASEBAND_CFG)
		NAME(TA_UNIT_2_WIFI_MAC)
		NAME(TA_UNIT_2_BLUETOOTH_MAC)
		NAME(TA_UNIT_2_SERIAL_NO)
		NAME(TA_UNIT_2_PBA_ID)
		NAME(TA_UNIT_2_PBA_ID_REV)
		NAME(TA_UNIT_2_PP_SEMC_ITP_PRODUCT_NO)
		NAME(TA_UNIT_2_PP_SEMC_ITP_REV)
		NAME(TA_UNIT_2_DEVICE_KEY)
		NAME(TA_UNIT_2_REMOTE_LOCK)
		NAME(TA_UNIT_2_GOOGLE_LOCK)
		NAME(TA_UNIT_2_PROTOCOL_SWITCH)
		NAME(TA_UNIT_2_BOOT_COUNTER)
	}
	return "unknown";
#undef NAME
}

#ifndef SSIZE_MAX
typedef unsigned long long ssize_t;
#define SSIZE_MAX ((ssize_t) (SIZE_MAX / 2))
#endif

#if ENABLE_DEBUG
#define LOG printf
#else
#define LOG(...)
#endif

#define HEX "0123456789ABCDEF"

#define MOD_ADLER 65521

#define TOTAL_BLOCKS 16
#define BLOCK_SIZE 0x20000
#define MAX_UNIT_SIZE 0x10000
#define MAX_SIZE 0x200000

#define MAGIC_BYTES 0x3BF8E9C1
#define UNIT_DEVICE_NAME 0x000008A2
#define UNIT_UNLOCK_KEY 0x000008b2
#define UNIT_DRM_KEY 0x0001046b

static ssize_t g_getline(char **lineptr, size_t *n, FILE *stream)
{
	char *cur_pos, *new_lineptr;
	int c;
	size_t new_lineptr_len;
	
	if (lineptr == NULL || n == NULL || stream == NULL)
	{
		errno = EINVAL;
		printf("Error: EINVAL!\n");
		return -1;
	}
	
	if (*lineptr == NULL)
	{
		*n = MAX_UNIT_SIZE;
		if ((*lineptr = (char *)malloc(*n)) == NULL)
		{
			errno = ENOMEM;
			printf("Error: MAX_UNIT_SIZE reached!\n");
			return -1;
		}
	}
	
	cur_pos = *lineptr;
	for (;;)
	{
		c = getc(stream);
		
		if (ferror(stream) || (c == EOF && cur_pos == *lineptr))
		return -1;
		
		if (c == EOF)
			break;
		
		if ((*lineptr + *n - cur_pos) < 2)
		{
			if (SSIZE_MAX / 2 < *n)
			{
#ifdef EOVERFLOW
				errno = EOVERFLOW;
#else
				errno = ERANGE; /* no EOVERFLOW defined */
#endif
				printf("Error: EOVERFLOW!\n");
				return -1;
			}
			new_lineptr_len = *n * 2;
			
			if ((new_lineptr = (char *)realloc(*lineptr, new_lineptr_len)) == NULL)
			{
				errno = ENOMEM;
				printf("Error: ENOMEM for realloc!\n");
				return -1;
			}
			*lineptr = new_lineptr;
			*n = new_lineptr_len;
		}
		
		*cur_pos++ = c;
		
		if (c == '\r' || c == '\n')
			break;
	}
	
	*cur_pos = '\0';
	return (ssize_t)(cur_pos - *lineptr);
}

static void trim(char *ptr)
{
	int i = 0;
	int j = 0;
	
	while(ptr[j] != '\0')
	{
		if (ptr[j] == 0x20 || ptr[j] == 0x09 || ptr[j] == '\n' || ptr[j] == '\r')
		{
			++j;
			ptr[i] = ptr[j];
		}
		else
		{
			ptr[i] = ptr[j];
			++i;
			++j;
		}
	}
	ptr[i] = '\0';
}

static void to_uppercase(char *ptr)
{
	for ( ; *ptr; ++ptr) *ptr = toupper(*ptr);
}

static void to_ascii(char *dest, const char *text) {
	unsigned long int ch;
	for(; sscanf((const char *)text, "%02lx", &ch)==1; text+=2)
		*dest++ = ch;
	*dest = '\0';
}

static void printf_hex(unsigned char *buffer, unsigned int size)
{
	unsigned int i;
	
	for (i=0; i<size; ++i)
	{
		LOG("%02X", buffer[i] & 0xff);
	}
	LOG("\n\n" );
}

static void display_buffer_hex(char *message, unsigned char *buffer, unsigned int size)
{
	unsigned int i, j, k;
	
	LOG("%s[%d]:\n", message, (int)size);
	
	for (i=0; i<size; i+=16)
	{
		LOG("\n  %08X  ", i);
		for(j=0,k=0; k<16; j++,k++)
		{
			if (i+j < size)
			{
				LOG("%02X", buffer[i+j] & 0xff);
			}
			else
			{
				LOG("  ");
			}
			LOG(" ");
		}
		LOG(" ");
		for(j=0,k=0; k<16; j++,k++)
		{
			if (i+j < size)
			{
				if ((buffer[i+j] < 32) || (buffer[i+j] > 126))
				{
					LOG(".");
				}
				else
				{
					LOG("%c", buffer[i+j]);
				}
			}
		}
	}
	LOG("\n\n" );
}

static int file_exist(char *filename)
{
	struct stat st;
	return (stat(filename, &st) == 0) ? 0 : 1;
}

static unsigned int file_size(char *filename)
{
	struct stat st;
	unsigned int size;
	stat(filename, &st);
	size = st.st_size;
	return size;
}

static int check_valid_unit(char *in)
{
	int i, ret=0;
	
	if (strlen(in) < 8)
		return ret;
	
	for (i=0; i<8; ++i)
	{
		if ((in[i] >= '0' && in[i] <= '9') || (in[i] >= 'A' && in[i] <= 'Z') || (in[i] >= 'a' && in[i] <= 'z'))
			ret += 1;
	}
	
	if (ret == 8)
		return 1;
	else
		return 0;
}

static int check_valid_drm(char *in)
{
	int i, ret=0;
	
	if (strlen(in) < 32)
		return ret;
	
	for (i=0; i<32; ++i)
	{
		if ((in[i] >= '0' && in[i] <= '9') || (in[i] >= 'A' && in[i] <= 'Z') || (in[i] >= 'a' && in[i] <= 'z'))
			ret += 1;
	}
	
	if (ret == 32)
		return 1;
	else
		return 0;
}

static unsigned int calculate_adler32(unsigned char *data, unsigned int len)
{
	unsigned int a = 1;
	unsigned int b = 0;
	unsigned int i;
	
	for (i=0; i < len; ++i)
	{
		a = (a + data[i]) % MOD_ADLER;
		b = (b + a) % MOD_ADLER;
	}
	
	return (b << 16) | a;
}

unsigned char **allocate2D(int nrows, int ncols)
{
	int i;
	unsigned char **dat2;
	
	dat2 = (unsigned char **)malloc(nrows * sizeof(unsigned char *));
	
	for (i=0; i<nrows; ++i)
	{
		dat2[i] = (unsigned char *)malloc(ncols * sizeof(unsigned char));
	}
	
	if (dat2 == NULL || dat2[i-1] == NULL)
	{
		printf("Error allocating 2D memory!\n");
		exit(1);
	}
	
	return dat2;
}

#if 0
int maine(void)
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	
	char unit[9];
	char unit_sz_tmp[5];
	unsigned int unit_sz;
	char unit_data[MAX_UNIT_SIZE];
	int ret=1, i, j, the_rest=0, partition_oppened=0, finished=0;
	
	fp = fopen("test.ta", "r");
	if (fp == NULL)
		return 0;
	
	while((read = g_getline(&line, &len, fp)) != -1)
	{
		switch(read)
		{
			case 1:
				LOG("Skipped empty line.\n\n");
				break;
			
			case 3:
				if (line[0] >= 0x30 && line[0] <= 0x39 && line[1] >= 0x30 && line[1] <= 0x39)
				{
					LOG("Found partition: %02x\n", atoi(line));
					if (!partition_oppened)
					{
						printf("Sending command to bootloader to open partition: %02x ...\n", atoi(line));
						partition_oppened = atoi(line);
						printf("Partition %02X opened!\n", partition_oppened);
						LOG("\n");
					}
					else
					{
						printf("Sending command to bootloader to close partition %02X and open new partition: %02x ...\n",
						partition_oppened, atoi(line));
						partition_oppened = atoi(line);
						printf("Partition %02X opened!\n", partition_oppened);
						LOG("\n");
					}
				}
				break;
			
			default:
				if (line[0] == '/')
				{
					LOG("Skipped comment line.\n\n");
				}
				else
				{
					LOG("Retrieved line of lenght: %lu\n", read);
					
					if (check_valid_unit(line))
					{
						finished = 0;
						memcpy(unit, line, 8);
						unit[8] = '\0';
						to_uppercase(unit);
						trim(line);
						LOG("Line lenght after trim: %lu\n", strlen(line));
						LOG("Found unit: %s\n", unit);
						
						/* unit(8) + unit size(4) + at least one hex(2) */
						if (strlen(line) < 14)
						{
							printf("Error: corrupted unit! Skipping this unit!\n\n");
							the_rest = 0;
							break;
						}
						else
						{
							memcpy(unit_sz_tmp, line+8, 4);
							unit_sz_tmp[4] = '\0';
							sscanf(unit_sz_tmp, "%X", &unit_sz);
							LOG("Unit size: %02X\n", unit_sz);
							memset(unit_data, '\0', sizeof(unit_data));
							i = 0;
							do {
								memcpy(unit_data+i, line+12+i, 1);
							} while(++i < strlen(line));
							unit_data[i] = '\0';
							
							if ((unsigned int)strlen(line)-12 < unit_sz*2)
							{
								LOG("Data probably continues in a new line (%u not match %u)!\n",
								(unsigned int)strlen(line)-12, unit_sz*2);
								the_rest = 1;
							}
							else
								the_rest = 0;
							
							if ((unsigned int)strlen(unit_data) == unit_sz*2)
								finished = 1;
						}				
					}
					else
					{
						if (the_rest)
						{
							finished = 0;
							trim(line);
							LOG("Line lenght after trim: %lu\n", strlen(line));
							LOG("Found the rest ot the data!\n");
							j = strlen(unit_data);
							i = 0;
							do {
								memcpy(unit_data+j+i, line+i, 1);
							} while(++i < strlen(line));
							unit_data[j+i] = '\0';
							
							if ((unsigned int)strlen(unit_data) == unit_sz*2)
							{
								the_rest = 0;
								finished = 1;
							}
						}
					}
					
					if (finished)
					{
						finished = 0;
						printf("\n<<-------------------- Retrieval finished! Found unit: %s, Unit size: %04X, Unit data:\n", unit, unit_sz);
						to_ascii(unit_data, unit_data);
						display_buffer_hex(unit, (unsigned char *)unit_data, unit_sz);
						printf("---------------------->> Writing data to phone...\n");
						
						/*************************************
						
						HERE BOOTLOADER COMMANDS :)
						
						**************************************/
					}
					LOG("\n");
				}
				break;
		}
	}
	
	if (fp)
		fclose(fp);
	
	if (line)
		free(line);
	
	if (partition_oppened)
	{
		printf("Sending command to bootloader to close ta.\n");
		LOG("\n");
		partition_oppened = 0;
	}
	
	return ret;
}
#endif

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("usage: %s TA.img 245B53286EA780E3A8BB014627AF7B25\n", argv[0]);
		printf("eg: %s your_ta_dump.img drm_key_in_hex\n", argv[0]);
#ifdef _WIN32
	system("pause");
#endif
		exit(1);
	}
	
	if (file_exist(argv[1]))
	{
		printf("File: %s not exist!\n", argv[1]);
		exit(1);
	}
	
	unsigned int filesize = file_size(argv[1]);
	if (filesize != MAX_SIZE)
	{
		printf("File: %s with size: 0x%x is a trim area dump?\n", argv[1], filesize);
		exit(1);
	}
	
	if (!check_valid_drm(argv[2]))
	{
		printf("Drm key is invalid!\nDo in mind if you enter wrong drm key you can say bye bye to your device!!!\n");
		exit(1);
	}
	
	size_t blocks_read = 0;
	size_t blocks_write = 0;
	
	unsigned char **blocks;
	unsigned char **reconstruct;
	unsigned char *data;
	char supported_device[128];
	int supported = 0;
	int errornotice = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int total_blocks = 0;
	
	unsigned int partition_magic;
	unsigned int adler32;
	unsigned int calc_adler32;
	unsigned char total_parts;
	unsigned char part;
	unsigned char partition;
	unsigned char nob;
	
	int data_size;
	int numofunits;
	int curpos;
	int block_space_remain = 0;
	
	unsigned int unit_magic;
	unsigned int unit;
	unsigned int unit_size;
	unsigned int unit_size_aligned;
	unsigned char *unit_data;
	
	FILE *fp = NULL;
	FILE *fprec = NULL;
	FILE *fpta = NULL;
	
	blocks = allocate2D(TOTAL_BLOCKS, BLOCK_SIZE);
	reconstruct = allocate2D(TOTAL_BLOCKS, BLOCK_SIZE);
	
	fp = fopen(argv[1], "rb");
	
	if (NULL == fp)
	{
		printf("Failed to open TA.img\n");
		exit(1);
	}
	else
	{
		while (!feof(fp))
		{
			blocks_read = fread(blocks[i], 1, BLOCK_SIZE, fp);
			if (blocks_read == BLOCK_SIZE)
				i += 1;
			else
				break;
		}
		fclose(fp);
		total_blocks = i;
	}
	
	if (total_blocks != 16)
	{
		printf("Error: found %d blocks in TA.img instead of 16!\n", total_blocks);
		free(blocks);
		free(reconstruct);
		exit(1);
	}
	
	for (i=0; i<total_blocks; ++i)
	{
		block_space_remain = BLOCK_SIZE;
		memset(reconstruct[i], 0, BLOCK_SIZE);
		
		//partition_magic = *(unsigned int *)&blocks[i];
		memcpy(&partition_magic, blocks[i], 4);
		switch (partition_magic)
		{
			case MAGIC_BYTES:
				printf("block %d\n", i);
				//adler32 = *(unsigned int *)&blocks[i][4];
				memcpy(&adler32, blocks[i]+4, 4);
				total_parts = *(unsigned char *)&blocks[i][8];
				part = *(unsigned char *)&blocks[i][9];
				partition = *(unsigned char *)&blocks[i][10];
				nob = *(unsigned char *)&blocks[i][11];
				
				block_space_remain -= 12; //magic, adler32, total_parts+part+partition+numblocks
				
				memcpy(reconstruct[i], (unsigned int *)&partition_magic, 4);
				memcpy(reconstruct[i]+4, (unsigned char *)&adler32, 4);
				memcpy(reconstruct[i]+8, (unsigned char *)&total_parts, 1);
				memcpy(reconstruct[i]+9, (unsigned char *)&part, 1);
				memcpy(reconstruct[i]+10, (unsigned char *)&partition, 1);
				memcpy(reconstruct[i]+11, (unsigned char *)&nob, 1);
				//display_buffer_hex("reconstructed block header", reconstruct[i], 12);
			
				if (nob == 0xff)
					data_size = BLOCK_SIZE - 0x8;
				else
					data_size = ((nob + 1) * 0x800) - 0x8;
				
				printf("partition hash: %08X\n", adler32);
				printf("partition: %d\n", partition);
				printf("part: %d\n", part);
				printf("total_parts: %d\n", total_parts);
				printf("numofblocks: 0x%X\n", nob);
				printf("data size: 0x%X\n", data_size);
				
				char tmp[10];
				tmp[0] = HEX[total_parts >> 4];
				tmp[1] = HEX[total_parts & 15];
				tmp[2] = HEX[part >> 4];
				tmp[3] = HEX[part & 15];
				tmp[4] = HEX[partition >> 4];
				tmp[5] = HEX[partition & 15];
				tmp[6] = '.';
				tmp[7] = 't';
				tmp[8] = 'a';
				tmp[9] = 0;
				fpta = fopen(tmp, "w+");
				if (fpta)
				{
					switch((uint8_t)partition)
					{
						case TRIMAREA_PARTITION_TRIM:
							fwrite("//trim partition\n", 1, 17, fpta);
							break;

						case TRIMAREA_PARTITION_MISC:
							fwrite("//misc partition\n", 1, 17, fpta);
							break;

						default:
							break;
					}
					fwrite(tmp+4, 1, 2, fpta);
					fwrite("\n\n", 1, 2, fpta);
				}
			
				if ((data = (unsigned char *)malloc(data_size * sizeof(unsigned char))) == NULL) {
					printf("Fatal error: can't malloc %d bytes for data!\n", data_size);
					free(blocks);
					free(reconstruct);
					if (fpta)
						fclose(fpta);
					exit(1);
				}
				memset(data, 0, data_size);
				memcpy(data, blocks[i]+8, data_size);
				//display_buffer_hex("data", data, data_size);
			
				numofunits = 0;
				curpos = 12;
			
				for (j=0; j<data_size; ++j)
				{
					unit_size = 0;        
					unit_magic = *(unsigned int *)&data[j];
					switch (unit_magic)
					{
						case MAGIC_BYTES:
							unit = *(unsigned int *)&data[j-8];
							unit_size = *(unsigned int *)&data[j-4];
							if ((unit_data = (unsigned char *)malloc(unit_size * sizeof(unsigned char))) == NULL)
							{
								printf("Fatal error: can't malloc %d bytes for unit data!\n", unit_size);
								free(data);
								free(blocks);
								free(reconstruct);
								if (fpta)
								fclose(fpta);
								exit(1);
							}
							memset(unit_data, 0, unit_size);
							memcpy(unit_data, data+j+8, unit_size);
							//display_buffer_hex("unit_data", unit_data, unit_size);
							
							unit_size_aligned = ((unit_size % 4) > 0) ? (unit_size - (unit_size % 4) + 4) : unit_size;
							
							memcpy(reconstruct[i]+curpos, (unsigned int *)&unit, 4);
							memcpy(reconstruct[i]+curpos+4, (unsigned int *)&unit_size, 4);
							memcpy(reconstruct[i]+curpos+8, (unsigned int *)&unit_magic, 4);
							memset(reconstruct[i]+curpos+12, 0xff, 4);
							memcpy(reconstruct[i]+curpos+16, unit_data, unit_size);
							if ((unit_size_aligned - unit_size) > 0)
								memset(reconstruct[i]+curpos+unit_size+16, 0xff, unit_size_aligned - unit_size);
							
							block_space_remain -= unit_size_aligned; //unit aligned data
							block_space_remain -= 16; //unit, unit_size, magic, ffffffff
							
							switch (unit)
							{
								case UNIT_DEVICE_NAME:
									memcpy(supported_device, unit_data, unit_size);
									supported_device[unit_size] = 0;
									
									if (memcmp(unit_data, "D5503", unit_size) == 0 || memcmp(unit_data, "D5803", unit_size) == 0 || memcmp(unit_data, "F5321", unit_size) == 0)
									{
										supported = 1;
									}
									else
									{
										supported = 0;
									}
									break;
								
								case UNIT_UNLOCK_KEY:
									unit = UNIT_DRM_KEY;
									memcpy(reconstruct[i]+curpos, (unsigned int *)&unit, 4);
									char drm[0x10];
									to_ascii(drm, argv[2]);
									memcpy(reconstruct[i]+curpos+16, drm, 0x10);
									break;
								
								default:
									break;
							}
							
							//printf("%08X ", unit);
							//printf("%04X ", unit_size);
							//printf_hex(unit_data, unit_size);
							if (fpta)
							{
								if (memcmp(name(unit), "unknown", 7) != 0)
								{
									switch(partition)
									{
										case TRIMAREA_PARTITION_TRIM:
											if (strstr(name(unit), "TA_UNIT_1") != NULL)
												fprintf(fpta, "\n//%s\n", name(unit));
											break;

										case TRIMAREA_PARTITION_MISC:
											if (strstr(name(unit), "TA_UNIT_2") != NULL)
												fprintf(fpta, "\n//%s\n", name(unit));
											break;

										default:
											break;
									}
								}
								fprintf(fpta, "%08X ", unit);
								fprintf(fpta, "%04X ", unit_size);
								for (k=0; k<unit_size; ++k)
									fprintf(fpta, "%02X", reconstruct[i][curpos+16+k] & 0xff);
								fprintf(fpta, "\n");
								if (memcmp(name(unit), "unknown", 7) != 0)
								{
									switch(partition)
									{
										case TRIMAREA_PARTITION_TRIM:
											if (strstr(name(unit), "TA_UNIT_1") != NULL)
												fprintf(fpta, "\n");
											break;

										case TRIMAREA_PARTITION_MISC:
											if (strstr(name(unit), "TA_UNIT_2") != NULL)
												fprintf(fpta, "\n");
											break;

										default:
											break;
									}
								}

							}
							
							curpos += (4 + 4 + 4 + 4 + unit_size + (unit_size_aligned - unit_size));
							numofunits += 1;
							free(unit_data);
							break;
						
						default:
							break;
					}
				}
			
				if (block_space_remain > 0)
					memset(reconstruct[i]+curpos, 0xff, block_space_remain);
				
				printf("numofunits: %d\n", numofunits);
				//printf_hex(data, data_size);
				calc_adler32 = calculate_adler32(data, data_size);
				printf("calculated old hash: %08X ", calc_adler32);
				if (calc_adler32 == adler32)
					printf("\n");
				else
				{
					printf("...nok!\n");
					errornotice = 1; //error happened, do not generate corupted trim area.
				}
				calc_adler32 = calculate_adler32(reconstruct[i]+8, data_size);
				printf("calculated hash from reconstruct: %08X ", calc_adler32);
				if (calc_adler32 != adler32)
				{
					printf("...changes in block.");
					memcpy(reconstruct[i]+4, (unsigned char *)&calc_adler32, 4);
				}
				printf("\n");
				printf("remaining space in block: 0x%x\n", block_space_remain);
				printf("----------------------------------\n");
				
				free(data);
				
				if (fpta)
				{
					fprintf(fpta, "\n");
					fclose(fpta);
				}
				
				break;
			
			default:
				printf("block %d is empty or backup block.\n", i);
				memcpy(reconstruct[i], blocks[i], BLOCK_SIZE);
				//printf_hex(blocks[i], BLOCK_SIZE);
				printf("----------------------------------\n");
				break;
		}
		
		//display_buffer_hex("unit_data reconstruct", reconstruct[i], BLOCK_SIZE);
	}
	
	switch (supported)
	{
		case 1:
			printf("\nFound device (%s).\n", supported_device);
			printf("generating reconstructed trim area...\n");
			
			if (errornotice)
			{
				printf("Error happened! Did you modified your trim area?\n");
			}
			else
			{
				fprec = fopen("reconstructed_TA.img", "wb+");
				if (NULL == fprec)
				{
					printf("error: can't open reconstructed_TA.img for writing!\n");
				}
				else
				{
					for (i=0; i<total_blocks; ++i)
						blocks_write += fwrite(reconstruct[i], 1, BLOCK_SIZE, fprec);
					fclose(fprec);
				}
				
				if (blocks_write != MAX_SIZE)
				{
					printf("Error! Writen 0x%X bytes instead of 0x%X!\n", (unsigned int)blocks_write, MAX_SIZE);
					remove("reconstructed_TA.img");
				}
				else
				{
					printf("reconstructed_TA.img is created. Done.\n");
				}
			}
			break;
		
		default:
			printf("Your device (%s) is not supported!\n", supported_device);
			break;
	}
	
	free(blocks);
	free(reconstruct);

#ifdef _WIN32
	system("pause");
#endif
	
	return 0;
}

