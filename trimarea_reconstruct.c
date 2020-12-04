#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>

#define ENUM_BODY(name, value) name value,
#define AS_STRING_CASE(name, value) case name: return #name;
#define DEFINE_ENUM(name, list)	typedef enum {list(ENUM_BODY)} name;
#define AS_STRING_DEC(name, list) const char *name##AsString(name n);

#define AS_STRING_FUNC(name, list)	\
	const char *name##AsString(name n) {	\
		switch(n) {	\
			list(AS_STRING_CASE)	\
				default:	\
				{	\
					static char int_as_string[8];	\
					snprintf(int_as_string, sizeof(int_as_string), "%d", n);	\
					return int_as_string;	\
				}	\
		}	\
	}

/* partition types */
#define TRIMAREA_PARTITION_TRIM 1
#define TRIMAREA_PARTITION_MISC 2

#define MAX_UNIT_LINE_LEN 0x80000

#define TRIMAREA_UNITS(_)	\
	_(TA_UNIT_1_RF_BC_CFG, = 0x755)	\
	_(TA_UNIT_2_FLA_FLA, = 0x7D2)	\
	_(TA_UNIT_2_HW_CONF, = 0x7D3)	\
	_(TA_UNIT_2_SIMLOCK, = 0x7DA)	\
	_(TA_UNIT_2_DRM_KEY_STATUS, = 0x7E5)	\
	_(TA_UNIT_2_BLOB_0_CKB, = 0x7E6)	\
	_(TA_UNIT_2_BLOB_1_WIDEVINE, = 0x7E7)	\
	_(TA_UNIT_2_BLOB_2_MARLIN, = 0x7E8)	\
	_(TA_UNIT_2_BLOB_3_HDCP, = 0x7E9)	\
	_(TA_UNIT_2_BLOB_4, = 0x7EA)	\
	_(TA_UNIT_2_BLOB_5, = 0x7EB)	\
	_(TA_UNIT_2_BLOB_6, = 0x7EC)	\
	_(TA_UNIT_2_BLOB_7, = 0x7ED)	\
	_(TA_UNIT_2_BLOB_8, = 0x7EE)	\
	_(TA_UNIT_2_BLOB_9, = 0x7EF)	\
	_(TA_UNIT_2_BLOB_A, = 0x7F0)	\
	_(TA_UNIT_2_BLOB_B, = 0x7F1)	\
	_(TA_UNIT_2_BLOB_C, = 0x7F2)	\
	_(TA_UNIT_2_BLOB_D, = 0x7F3)	\
	_(TA_UNIT_2_BLOB_E, = 0x7F4)	\
	_(TA_UNIT_2_DRM_DYN_DATA, = 2040) /* 0x7F8 */	\
	_(TA_UNIT_2_LAST_BOOT_LOG, = 0x802)	\
	_(TA_UNIT_2_BOOT_COUNTER, = 0x805)	\
	_(TA_UNIT_2_DRM_CUST, = 2099) /* 0x833 */	\
	_(TA_UNIT_2_SIMLOCK_SIGNATURE, = 0x851)	\
	_(TA_UNIT_2_MACHINE_ID, = 0x85D)	\
	_(TA_UNIT_2_SW_VER, = 0x89A)	\
	_(TA_UNIT_2_CUST_VER, = 0x89D)	\
	_(TA_UNIT_2_FS_VER, = 0x89E)	\
	_(TA_UNIT_2_S1_BOOT_VER, = 0x89F)	\
	_(TA_UNIT_2_BUILD_TYPE, = 0x8A1)	\
	_(TA_UNIT_2_PHONE_NAME, = 0x8A2)	\
	_(TA_UNIT_2_AC_VER, = 0x8A4) /* cust-reset.ta zeroes this */	\
	_(TA_UNIT_2_BL_UNLOCKCODE, = 0x8B2)	\
	_(TA_UNIT_2_STARTUP_SHUTDOWNRESULT, = 0x8B3)	\
	_(TA_UNIT_2_CHECKPOINTS_ENABLED, = 2228) /* 0x8B4 */	\
	_(TA_UNIT_2_CHECKPOINTS_REACHED, = 2229) /* 0x8B5 */	\
	_(TA_UNIT_2_STARTUP_REASON, = 0x8FD) /* boot config override unit */	\
	_(TA_UNIT_2_DISABLE_CHARGE_ONLY, = 0x907)	\
	_(TA_UNIT_2_DISABLE_USB_CHARGING, = 2312) /* 0x908 */	\
	_(TA_UNIT_2_OSV_RESTRICTION, = 0x91A)	\
	_(TA_UNIT_2_FOTA_STATUS, = 2401) /* 0x961 */	\
	_(TA_UNIT_2_FOTA_REPART, = 2402) /* 0x962 */	\
	_(TA_UNIT_2_MODEM_CUST_CFG, = 0x964) /* FOTA_INTERNAL?? cfg located in system/etc/customization/modem/ -> fota-reset.ta zeroes this */	\
	_(TA_UNIT_2_VIRTUAL_UIM, = 2460) /* 0x99C */	\
	_(TA_UNIT_2_ENABLE_USB_ENG_PID, = 2470) /* 0x9A6 */	\
	_(TA_UNIT_2_ENABLE_USB_DEBUGGING, = 2471) /* 0x9A7 */	\
	_(TA_UNIT_2_ENABLE_SERIAL_CONSOLE, = 0x9A9) /* value 1 for enable serial console or value 0 (default) to disable (https://forum.xda-developers.com/showpost.php?p=80212371&postcount=1125) */	\
	_(TA_UNIT_2_VERSION_HISTORY, = 0x9AB)	\
	_(TA_UNIT_2_ENABLE_NONSECURE_USB_DEBUG, = 0x9B6)	\
	_(TA_UNIT_2_BATTERY_CAPACITY, = 2490) /* 0x9BA */	\
	_(TA_UNIT_2_DCMLAC_TRANSMISSION_CONFIG, = 2495) /* 0x9BF */	\
	_(TA_UNIT_2_DCMLAC_SEND_TIME, = 2496) /* 0x9C0 */	\
	_(TA_UNIT_2_DCMLAC_UPLOAD_LOG_LATEST, = 2497) /* 0x9C1 */	\
	_(TA_UNIT_2_DCMLAC_IDDCONFIG_SW_VERSION, = 2498) /* 0x9C2 */	\
	_(TA_UNIT_2_CREDMGR_KEYTABLE_PRESET, = 0x9C4)	\
	_(TA_UNIT_2_SECURITY_GA_DATA, = 2501) /* 0x9C5 */	\
	_(TA_UNIT_2_MASTER_RESET, = 2550) /* 0x9F6 */	\
	_(TA_UNIT_2_BASEBAND_CFG, = 0x9F7) /* cfg located in the modem */	\
	_(TA_UNIT_2_WIPE_DATA, = 2552) /* 0x9F8 */	\
	_(TA_UNIT_2_WLAN_ADDR_0, = 2560) /* 0xA00 */	\
	_(TA_UNIT_2_WLAN_ADDR_1, = 2561) /* 0xA01 */	\
	_(TA_UNIT_2_WLAN_ADDR_2, = 2562) /* 0xA02 */	\
	_(TA_UNIT_2_WLAN_ADDR_3, = 2563) /* 0xA03 */	\
	_(TA_UNIT_2_WLAN_TXPOWER_2_4G, = 2564) /* 0xA04 */	\
	_(TA_UNIT_2_WLAN_TXPOWER_5G_LOW, = 2565) /* 0xA05 */	\
	_(TA_UNIT_2_WLAN_TXPOWER_5G_MID, = 2566) /* 0xA06 */	\
	_(TA_UNIT_2_WLAN_TXPOWER_5G_HIGH, = 2567) /* 0xA07 */	\
	_(TA_UNIT_2_BD_ADDR, = 2568) /* 0xA08 */	\
	_(TA_UNIT_2_MMS_USER_AGENT, = 2570) /* 0xA0A */	\
	_(TA_UNIT_2_MMSC_URL, = 2571) /* 0xA0B */	\
	_(TA_UNIT_2_LCD_NVM_1, = 2585) /* 0xA19 */	\
	_(TA_UNIT_2_LCD_NVM_2, = 2586) /* 0xA1A */	\
	_(TA_UNIT_2_LCD_NVM_WRITE_COUNT, = 2587) /* 0xA1B */	\
	_(TA_UNIT_2_LCD_NVM_HWID, = 2588) /* 0xA1C */	\
	_(TA_UNIT_2_RETAIL_DEMO_ACTIVE_STATUS, = 2590) /* 0xA1E */	\
	_(TA_UNIT_2_PHONE_USAGE_FLAG, = 2595) /* 0xA23 */	\
	_(TA_UNIT_2_NV_PREF_MODE_I, = 2601) /* 0xA29 */	\
	_(TA_UNIT_2_NV_LIFE_TIMER_G_I, = 2602) /* 0xA2A */	\
	_(TA_UNIT_2_NV_SERVICE_DOMAIN_PREF_I, = 2603) /* 0xA2B */	\
	_(TA_UNIT_2_SONY_SERVICE_ID, = 4899) /* 0x1323 */	\
	_(TA_UNIT_2_SERIAL_NO, = 0x1324)	\
	_(TA_UNIT_2_PBA_ID, = 0x1325)	\
	_(TA_UNIT_2_PBA_ID_REV, = 0x1326)	\
	_(TA_UNIT_2_PP_SEMC_ITP_PRODUCT_NO, = 0x132C)	\
	_(TA_UNIT_2_PP_SEMC_ITP_REV, = 0x132D)	\
	_(TA_UNIT_2_NFC_CHIP_FW, = 4952) /* 0x1358 */	\
	_(TA_UNIT_2_NFC_CHIP_VERSION, = 4953) /* 0x1359 */	\
	_(TA_UNIT_2_GYRO_CALIBRATED, = 4960) /* 0x1360 */	\
	_(TA_UNIT_2_MPU3050_CALIBRATION_DATA, = 4961) /* 0x1361 */	\
	_(TA_UNIT_2_MPU6050_CALIBRATION_DATA, = 4962) /* 0x1362 */	\
	_(TA_UNIT_2_ACCEL_CALIBRATION_DATA, = 4963) /* 0x1363 */	\
	_(TA_UNIT_2_GYRO_CALIBRATION_DATA, = 4964) /* 0x1364 */	\
	_(TA_UNIT_2_PROXIMITY_CALIBRATION_DATA, = 4970) /* 0x136A */	\
	_(TA_UNIT_1_LTE_BC_CFG, = 0x1AAC)	\
	_(TA_UNIT_2_THERMAL_SHUTDOWN_COUNT, = 10022) /* 0x2726 */	\
	_(TA_UNIT_2_THERMAL_LAST_SHUTDOWN_1, = 10023) /* 0x2727 */	\
	_(TA_UNIT_2_THERMAL_LAST_SHUTDOWN_2, = 10024) /* 0x2728 */	\
	_(TA_UNIT_2_THERMAL_LAST_SHUTDOWN_3, = 10025) /* 0x2729 */	\
	_(TA_UNIT_2_FLASH_PROGRESS, = 10100) /* 0x2774 */	\
	_(TA_UNIT_2_FLIP_SLIDE_COUNTER, = 10200) /* 0x27D8 */	\
	_(TA_UNIT_2_DEVICE_KEY, = 0x1046B)	\
	_(TA_UNIT_2_REMOTE_LOCK, = 0x1046C) /* a sin file */	\
	_(TA_UNIT_2_GOOGLE_LOCK, = 0x1046F) /* google lock ( allow bootloader unlock in dev settings ) */	\
	_(TA_UNIT_2_DEVICE_ID_HMAC_KEY, = 0x10471) /* dev_id HMAC key. Depend on existance of unit, = 0x874 (https://forum.xda-developers.com/showpost.php?p=82983507&postcount=1628) */

DEFINE_ENUM(Tu, TRIMAREA_UNITS)
AS_STRING_DEC(Tu, TRIMAREA_UNITS)
AS_STRING_FUNC(Tu, TRIMAREA_UNITS)

struct trim_area_unit_s {
	uint32_t unit;
	uint32_t unit_sz;
	uint32_t magic;
	uint32_t four_bytes;
	void *data;
};

struct trim_area_partition_s {
	uint32_t magic;
	uint32_t adler32;
	uint8_t total_parts;
	uint8_t part;
	uint8_t partition;
	uint8_t total_blocks;
	char *blocks;
};

typedef struct trim_area_unit_s trim_area_unit_t;
typedef struct trim_area_partition_s trim_area_partition_t;

static void print_unit(FILE *fp, uint32_t unit)
{
	if (TuAsString(unit)[0] < 0x30 || TuAsString(unit)[0] > 0x39)
	{
		fprintf(fp, "// %s\n", TuAsString(unit));
	}	
}

static uint32_t unit_sz_aligned(uint32_t n)
{
	return ((n % 4) > 0) ? (n - (n % 4) + 4) : n;
}

#define MOD_ADLER 65521

static uint32_t calculate_adler32(uint8_t total_parts, uint8_t part, uint8_t partition, uint8_t total_blocks, char *data, uint32_t len)
{
	uint32_t a = 1;
	uint32_t b = 0;
	uint32_t i;

	a = (a + total_parts) % MOD_ADLER;
	b = (b + a) % MOD_ADLER;

	a = (a + part) % MOD_ADLER;
	b = (b + a) % MOD_ADLER;

	a = (a + partition) % MOD_ADLER;
	b = (b + a) % MOD_ADLER;

	a = (a + total_blocks) % MOD_ADLER;
	b = (b + a) % MOD_ADLER;

	for (i=0; i < len-4; ++i)
	{
		a = (a + data[i]) % MOD_ADLER;
		b = (b + a) % MOD_ADLER;
	}
	
	return (b << 16) | a;
}

static uint32_t file_size(char *filename)
{
	struct stat st;
	uint32_t size;
	stat(filename, &st);
	size = st.st_size;
	return size;
}

static uint8_t check_valid_unit(char *in)
{
	uint8_t i, ret=0;

	if (strlen(in) < 8)
		return ret;

	for (i=0; i<8; ++i) {
		if ((in[i] >= '0' && in[i] <= '9') || (in[i] >= 'A' && in[i] <= 'Z') || (in[i] >= 'a' && in[i] <= 'z'))
			ret += 1;
	}

	if (ret == 8)
		return 1;
	else
		return 0;
}

static ssize_t g_getline(char **lineptr, size_t *n, FILE *stream)
{
	char *cur_pos, *new_lineptr;
	int c;
	size_t new_lineptr_len;

	if (lineptr == NULL || n == NULL || stream == NULL) {
		errno = EINVAL;
		printf("Error: EINVAL!\n");
		return -1;
	}

	if (*lineptr == NULL) {
		*n = MAX_UNIT_LINE_LEN;
		if ((*lineptr = (char *)malloc(*n)) == NULL) {
			errno = ENOMEM;
			printf("Error: MAX_UNIT_LINE_LEN reached!\n");
			return -1;
		}
	}

	cur_pos = *lineptr;
	for (;;) {
		c = getc(stream);

		if (ferror(stream) || (c == EOF && cur_pos == *lineptr))
			return -1;

		if (c == EOF)
			break;

		if ((*lineptr + *n - cur_pos) < 2) {
			if (SSIZE_MAX / 2 < *n) {
#ifdef EOVERFLOW
				errno = EOVERFLOW;
#else
				errno = ERANGE; /* no EOVERFLOW defined */
#endif
			printf("Error: EOVERFLOW!\n");
			return -1;
		}
		new_lineptr_len = *n * 2;

		if ((new_lineptr = (char *)realloc(*lineptr, new_lineptr_len)) == NULL) {
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

static void trimm(char *ptr)
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

static void to_ascii(char *dest, const char *text)
{
	unsigned long int ch;
	for(; sscanf((const char *)text, "%02lx", &ch)==1; text+=2)
		*dest++ = ch;
	*dest = 0;
}

static void to_uppercase(char *ptr)
{
	for ( ; *ptr; ++ptr) *ptr = toupper(*ptr);
}

int main(int argc, char *argv[])
{
	int ret = 0;

	if (argc < 2)
	{
		printf("Usage: %s TA.img\n", argv[0]);
		return ret;
	}

	trim_area_partition_t trim[16];
	trim_area_partition_t trim_new[16];

	FILE *ta_fp = NULL;
	FILE *trim_fp = NULL;
	FILE *misc_fp = NULL;

	bool trim_printed = false;
	bool misc_printed = false;

	uint32_t total_units = 0;
	uint32_t total_size = 0;

	uint8_t i = 0;
	uint32_t j = 0;
	uint32_t k = 0;

	uint32_t block_sz;
	uint32_t supported_ta = file_size(argv[1]);

	switch(supported_ta)
	{
		case 0x200000:
			block_sz = 0x20000;
			break;

		case 0x800000:
			block_sz = 0x80000;
			break;

		default:
			block_sz = 0;
			break;
	}

	if (!block_sz)
	{
		printf("Unsupported %s with size: 0x%X!\n", argv[1], supported_ta);
		return -1;
	}

	if ((ta_fp = fopen(argv[1], "rb")) == NULL)
	{
		printf("Unable to open %s!\n", argv[1]);
		return -1;
	}

	printf("File: %s\n", argv[1]);
	printf("File_sz: 0x%X\n", supported_ta);
	printf("Block_sz: 0x%X\n\n", block_sz);

	for (i=0; i < 16; ++i)
	{
		if ((trim[i].blocks = (char *)malloc(block_sz - 12)) == NULL)
		{
			printf("Unable to alocate 0x%X bytes for trim block %d!\n", block_sz - 12, i);
			fclose(ta_fp);
			return -1;
		}

		if ((trim_new[i].blocks = (char *)malloc(block_sz - 12)) == NULL)
		{
			printf("Unable to alocate 0x%X bytes for trim_new block %d!\n", block_sz - 12, i);
			fclose(ta_fp);
			return -1;
		}

		if ((fread(&trim[i], 12, 1, ta_fp)) != 1)
		{
			printf("Unable to read 12 bytes for trim block %d!\n", i);
			fclose(ta_fp);
			return -1;
		}

		if ((fread(trim[i].blocks, block_sz - 12, 1, ta_fp)) != 1)
		{
			printf("Unable to read 0x%X bytes for trim block %d!\n", block_sz - 12, i);
			fclose(ta_fp);
			return -1;
		}
	}

	fclose(ta_fp);

	if ((trim_fp = fopen("ta_01.ta", "wb")) == NULL)
	{
		printf("Unable to open ta_01.ta for write!\n");
		return -1;
	}

	if ((misc_fp = fopen("ta_02.ta", "wb")) == NULL)
	{
		printf("Unable to open ta_02.ta for write!\n");
		fclose(trim_fp);
		return -1;
	}

	for (i=0; i < 16; ++i)
	{
		printf("Block: %d (%08X)\n", i, trim[i].magic);

		switch(trim[i].partition)
		{
			case TRIMAREA_PARTITION_TRIM:
				total_units = 0;
				total_size = 0;

				printf("  total_parts: 0x%02x, part: 0x%02x, partition: 0x%02x, total_blocks: 0x%02x\n", trim[i].total_parts, trim[i].part, trim[i].partition, trim[i].total_blocks);

				if (!trim_printed)
				{
					fprintf(trim_fp, "// partition trim\n01\n\n");
					trim_printed = true;
				}

				for (j=0; j < (block_sz - 12); j+=4)
				{
					if (memcmp(trim[i].blocks + j, "\xc1\xe9\xf8\x3b", 4) == 0)
					{
						trim_area_unit_t unit;

						memcpy(&unit.unit, trim[i].blocks + j - 8, 4);
						memcpy(&unit.unit_sz, trim[i].blocks + j - 4, 4);
						unit.data = trim[i].blocks + j + 8;

						print_unit(trim_fp, unit.unit);
						if (unit.unit_sz > 0xffff)
							fprintf(trim_fp, "%08X %08X", unit.unit, unit.unit_sz);
						else
							fprintf(trim_fp, "%08X %04X", unit.unit, unit.unit_sz);

						for (k=0; k < unit.unit_sz; ++k)
							fprintf(trim_fp, " %02X", *((char *)unit.data + k));

						fprintf(trim_fp, "\n\n");
	
						total_units += 1;
						total_size += unit_sz_aligned(unit.unit_sz);
						total_size += 16;

						memset(&unit, 0, sizeof(unit));
					}
				}

				printf("    total units: %d\n", total_units);
				printf("    total size: 0x%X\n", total_size);
				printf("    total blocks: 0x%X\n", (total_size / 0x800));
				printf("    total blocks size: 0x%X\n", ((((total_size / 0x800)) + 1) * 0x800) - 0x8);
				printf("    total blocks size aligned: 0x%X\n", ((((total_size / 0x800)) + 1) * 0x800));
				printf("    adler32 hash: 0x%08X\n", calculate_adler32(trim[i].total_parts, trim[i].part, trim[i].partition, trim[i].total_blocks, trim[i].blocks, ((((total_size / 0x800)) + 1) * 0x800) - 0x8));
				break;

			case TRIMAREA_PARTITION_MISC:
				total_units = 0;
				total_size = 0;

				printf("  total_parts: 0x%02x, part: 0x%02x, partition: 0x%02x, total_blocks: 0x%02x\n", trim[i].total_parts, trim[i].part, trim[i].partition, trim[i].total_blocks);

				if (!misc_printed)
				{
					fprintf(misc_fp, "// partition misc\n02\n\n");
					misc_printed = true;
				}

				for (j=0; j < (block_sz - 12); j+=4)
				{
					if (memcmp(trim[i].blocks + j, "\xc1\xe9\xf8\x3b", 4) == 0)
					{
						trim_area_unit_t unit;

						memcpy(&unit.unit, trim[i].blocks + j - 8, 4);
						memcpy(&unit.unit_sz, trim[i].blocks + j - 4, 4);
						unit.data = trim[i].blocks + j + 8;

						print_unit(misc_fp, unit.unit);
						if (unit.unit_sz > 0xffff)
							fprintf(misc_fp, "%08X %08X", unit.unit, unit.unit_sz);
						else
							fprintf(misc_fp, "%08X %04X", unit.unit, unit.unit_sz);

						for (k=0; k < unit.unit_sz; ++k)
							fprintf(misc_fp, " %02X", *((char *)unit.data + k));

						fprintf(misc_fp, "\n\n");
	
						total_units += 1;
						total_size += unit_sz_aligned(unit.unit_sz);
						total_size += 16;

						memset(&unit, 0, sizeof(unit));
					}
				}

				printf("    total units: %d\n", total_units);
				printf("    total size: 0x%X\n", total_size);
				printf("    total blocks: 0x%X\n", (total_size / 0x800));
				printf("    total blocks size: 0x%X\n", ((((total_size / 0x800)) + 1) * 0x800) - 0x8);
				printf("    total blocks size aligned: 0x%X\n", ((((total_size / 0x800)) + 1) * 0x800));
				printf("    adler32 hash: 0x%08X\n", calculate_adler32(trim[i].total_parts, trim[i].part, trim[i].partition, trim[i].total_blocks, trim[i].blocks, ((((total_size / 0x800)) + 1) * 0x800) - 0x8));
				break;

			default:
				break;
		}
	}

	fclose(trim_fp);
	fclose(misc_fp);

	for (i=0; i < 16; ++i)
	{
		free(trim[i].blocks);
	}

	memset(&trim, 0, sizeof(trim));

	/* create reconstructed trim area */
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	i = 0;
	uint32_t curr_sz = 12;   // skip trim_area_partition_t structure
	uint32_t curr_offset = 0;

	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	FILE *fp = NULL;
	FILE *ofp = NULL;

	char unit[9];
	char unit_sz_tmp[9];
	char *unit_data = NULL;
	uint8_t part = 0;
	char partition[3];
	uint8_t pt = 0;
	char ta_f[10];
	uint32_t ii, jj, unit_dec=0, the_rest=0, finished=0, unit_sz, nob=0, adler32, ofp_now=0;

	/* some devices have some units which exceeds sizeof uint16_t */
	bool is_32bit = false;

	if ((unit_data = (char *)malloc(MAX_UNIT_LINE_LEN)) == NULL)
	{
		printf(" - Error allocating unit_data!\n");
		ret = 1;
		goto free_me;
	}

	if ((ofp = fopen("TA_new.img", "wb")) == NULL)
	{
		printf(" - Unable to open TA_new.img for write!\n");
		ret = 1;
		goto finish_proced_ta;
	}

	for (pt=1; pt<=2; ++pt)
	{
		snprintf(ta_f, sizeof(ta_f), "ta_0%d.ta", pt);

		if ((fp = fopen(ta_f, "rb")) == NULL)
		{
			printf(" - Unable to open ta_01.ta!\n");
			ret = 1;
			goto finish_proced_ta;
		}

		while((read = g_getline(&line, &len, fp)) != -1)
		{
			is_32bit = false;

			switch(read)
			{
				case 1:
					//printf("Skipped empty line.\n\n");
					break;

				case 3:
					printf("Partition line.\n");
					memset(partition, 0, sizeof(partition));
					memcpy(partition, line, 2);
					to_ascii(partition, partition);
					part = 0;
					printf("Partition: 0x%02X Part: 0x%02X\n\n", partition[0], part);
					break;

				default:
					if (line[0] == '/')
					{
						//printf("Skipped comment line.\n\n");
					}
					else
					{
						//printf("Retrieved line of lenght: %lu\n", read);

						if (check_valid_unit(line))
						{
							finished = 0;
							memcpy(unit, line, 8);
							unit[8] = '\0';
							to_uppercase(unit);
							sscanf(unit, "%x", &unit_dec);
							printf(" - Unit: %X (%u)\n", unit_dec, unit_dec);

							/*
							 * in case of 32 bit unit size!
							 * unit(8) + space(1) + unit size(8) + space(1)
							 */
							if (strlen(line) >= 18)
							{
								if (line[8] == ' ' && line[17] == ' ')
								{
									printf(" - 32 bit unit.\n");
									is_32bit = true;
								}
							}

							trimm(line);
							//printf("Line lenght after trim: %lu\n", strlen(line));

							if (is_32bit)
							{
								/* unit(8) + unit size(8) + at least one hex(2) */
								if (strlen(line) < 18)
								{
									if (strlen(line) == 16)
									{
										printf(" - Found specific unit which don't contain data.\n");
										the_rest = 0;
										finished = 1;
										unit_sz = 0;
									}
									else
									{
										printf(" - Error: corrupted unit! Skipping this unit!\n\n");
										the_rest = 0;
										break;
									}
								}
								else
								{
									memcpy(unit_sz_tmp, line+8, 8);
									unit_sz_tmp[8] = '\0';
									sscanf(unit_sz_tmp, "%x", &unit_sz);
									printf(" - Unit size: 0x%x\n", unit_sz);
									memset(unit_data, '\0', MAX_UNIT_LINE_LEN);
									ii = strlen(line);
									if (ii)
										memcpy(unit_data, line+16, ii);
									unit_data[ii] = '\0';

									if ((unsigned int)strlen(line)-16 < unit_sz*2)
									{
										printf("Data probably continues in a new line (%u not match %u)!\n",
											(unsigned int)strlen(line)-16, unit_sz*2);
										the_rest = 1;
									}
									else
									{
										the_rest = 0;
									}

									if ((unsigned int)strlen(unit_data) == unit_sz*2)
										finished = 1;
								}
							}
							else
							{
								/* unit(8) + unit size(4) + at least one hex(2) */
								if (strlen(line) < 14)
								{
									if (strlen(line) == 12)
									{
										printf(" - Found specific unit which don't contain data.\n");
										the_rest = 0;
										finished = 1;
										unit_sz = 0;
									}
									else
									{
										printf(" - Error: corrupted unit! Skipping this unit!\n\n");
										the_rest = 0;
										break;
									}
								}
								else
								{
									memcpy(unit_sz_tmp, line+8, 4);
									unit_sz_tmp[4] = '\0';
									sscanf(unit_sz_tmp, "%x", &unit_sz);
									printf(" - Unit size: 0x%x\n", unit_sz);
									memset(unit_data, '\0', MAX_UNIT_LINE_LEN);
									ii = strlen(line);
									if (ii)
										memcpy(unit_data, line+12, ii);
									unit_data[ii] = '\0';

									if ((unsigned int)strlen(line)-12 < unit_sz*2)
									{
										//printf("Data probably continues in a new line (%u not match %u)!\n",
										//	(unsigned int)strlen(line)-12, unit_sz*2);
										the_rest = 1;
									}
									else
									{
										the_rest = 0;
									}

									if ((unsigned int)strlen(unit_data) == unit_sz*2)
										finished = 1;
								}

							}
						}
						else
						{
							if (the_rest)
							{
								finished = 0;
								trimm(line);
								//printf("Line lenght after trim: %lu\n", strlen(line));
								//printf("Found the rest ot the data!\n");
								jj = strlen(unit_data);
								ii = strlen(line);
								if (ii)
									memcpy(unit_data+jj, line, ii);
								unit_data[jj + ii] = '\0';

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

							//printf("\n<<-------------------- Retrieval finished! Found unit: %s,"
							//	" Unit size: %04X, Unit data:%s\n",
							//	 unit, unit_sz, unit_sz ? "" : " NULL");

							to_ascii(unit_data, unit_data);

							if (unit_sz > 0)
							{
								if (curr_sz == 12)
								{
									fwrite("\xC1\xE9\xF8\x3B\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 12, 1, ofp); // dummy
								}

								if ((curr_sz + unit_sz) > (block_sz - 8))
								{
									i += 1;
									part += 1;
									curr_sz = 12;
									curr_offset += block_sz;
									fseek(ofp, curr_offset, SEEK_SET);
									fwrite("\xC1\xE9\xF8\x3B\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 12, 1, ofp); // dummy

								}

								*((char *)trim_new[i].blocks + 0 + curr_sz - 12) = ((unit_dec >> 0) & 0xff);
								*((char *)trim_new[i].blocks + 1 + curr_sz - 12) = ((unit_dec >> 8) & 0xff);
								*((char *)trim_new[i].blocks + 2 + curr_sz - 12) = ((unit_dec >> 16) & 0xff);
								*((char *)trim_new[i].blocks + 3 + curr_sz - 12) = ((unit_dec >> 24) & 0xff);
								*((char *)trim_new[i].blocks + 4 + curr_sz - 12) = ((unit_sz >> 0) & 0xff);
								*((char *)trim_new[i].blocks + 5 + curr_sz - 12) = ((unit_sz >> 8) & 0xff);
								*((char *)trim_new[i].blocks + 6 + curr_sz - 12) = ((unit_sz >> 16) & 0xff);
								*((char *)trim_new[i].blocks + 7 + curr_sz - 12) = ((unit_sz >> 24) & 0xff);
								curr_sz += 8;
								memcpy(trim_new[i].blocks + curr_sz - 12, "\xC1\xE9\xF8\x3B\xFF\xFF\xFF\xFF", 8);
								curr_sz += 8;
								memcpy(trim_new[i].blocks + curr_sz - 12, unit_data, unit_sz);
								curr_sz += unit_sz;

								fwrite(trim_new[i].blocks + curr_sz - 12 - unit_sz - 8 - 8, unit_sz + 8 + 8, 1, ofp);

								if (unit_sz_aligned(unit_sz) > unit_sz)
								{
									memset(trim_new[i].blocks + curr_sz - 12, 0xFF, unit_sz_aligned(unit_sz) - unit_sz);
									curr_sz += unit_sz_aligned(unit_sz) - unit_sz;

									fwrite(trim_new[i].blocks + curr_sz - 12 - (unit_sz_aligned(unit_sz) - unit_sz), unit_sz_aligned(unit_sz) - unit_sz, 1, ofp);
								}
							}
							else
							{
								if (curr_sz == 12)
								{
									fwrite("\xC1\xE9\xF8\x3B\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 12, 1, ofp); // dummy
								}

								if ((curr_sz + unit_sz) > (block_sz - 8))
								{
									i += 1;
									part += 1;
									curr_sz = 12;
									curr_offset += block_sz;
									fseek(ofp, curr_offset, SEEK_SET);
									fwrite("\xC1\xE9\xF8\x3B\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 12, 1, ofp); // dummy

								}

								*((char *)trim_new[i].blocks + 0 + curr_sz - 12) = ((unit_dec >> 0) & 0xff);
								*((char *)trim_new[i].blocks + 1 + curr_sz - 12) = ((unit_dec >> 8) & 0xff);
								*((char *)trim_new[i].blocks + 2 + curr_sz - 12) = ((unit_dec >> 16) & 0xff);
								*((char *)trim_new[i].blocks + 3 + curr_sz - 12) = ((unit_dec >> 24) & 0xff);
								*((char *)trim_new[i].blocks + 4 + curr_sz - 12) = ((unit_sz >> 0) & 0xff);
								*((char *)trim_new[i].blocks + 5 + curr_sz - 12) = ((unit_sz >> 8) & 0xff);
								*((char *)trim_new[i].blocks + 6 + curr_sz - 12) = ((unit_sz >> 16) & 0xff);
								*((char *)trim_new[i].blocks + 7 + curr_sz - 12) = ((unit_sz >> 24) & 0xff);
								curr_sz += 8;
								memcpy(trim_new[i].blocks + curr_sz - 12, "\xC1\xE9\xF8\x3B\xFF\xFF\xFF\xFF", 8);
								curr_sz += 8;

								fwrite(trim_new[i].blocks + curr_sz - 12 - 8 - 8, 8 + 8, 1, ofp);
							}

							/* update block partition structure */
							nob = (curr_sz / 0x800);
							ofp_now = ftell(ofp);
							fseek(ofp, ofp_now - (ofp_now % block_sz), SEEK_SET);
							fwrite("\xC1\xE9\xF8\x3B", 4, 1, ofp);
							adler32 = calculate_adler32(2, part, partition[0], nob & 0xff, trim_new[i].blocks, (((curr_sz / 0x800) + 1) * 0x800) - 0x8);
							fwrite(&adler32, 4, 1, ofp);
							fputc(2, ofp);
							fputc(part, ofp);
							fputc(partition[0], ofp);
							fputc(nob & 0xff, ofp);
							fseek(ofp, ofp_now, SEEK_SET);
						}
						printf("\n");
					}
					break;
			}
		}

		curr_offset += block_sz;
		fseek(ofp, curr_offset, SEEK_SET);
		curr_sz = 12;

		if (fp)
			fclose(fp);
		fp = NULL;
	}

finish_proced_ta:

	if (unit_data)
		free(unit_data);

	if (fp)
		fclose(fp);

	if (line)
		free(line);

	ofp_now = ftell(ofp);

	if (!ofp_now || pt != 3)
	{
		printf("TA_new.img is invalid, deleted!\n");
		remove("TA_new.img");
	}
	else
	{
		while(ofp_now % (block_sz * 16))
		{
			if ((ofp_now % block_sz) == 0)
				fwrite("\xff\xff\xff\xff", 4, 1, ofp);
			fputc(0, ofp);
			ofp_now = ftell(ofp); 
		}
	}

	if (ofp)
		fclose(ofp);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////

free_me:

	for (i=0; i < 16; ++i)
	{
		free(trim_new[i].blocks);
	}

	memset(&trim_new, 0, sizeof(trim_new));

	return ret;
}
