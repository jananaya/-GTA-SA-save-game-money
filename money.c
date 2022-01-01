// Game save checksum and modify money Gta San Andreas
// Jan Carlos Anaya Jimenez
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#define BYTES_BEFORE_CHECKSUM 0x317fb

UINT32 GetChecksumValue(const char* file_path)
{
	FILE* file;

	fopen_s(&file, file_path, "rb");

	if (file == NULL)
		return 0;

	UINT8 k;
	UINT checksum = 0;
	int bytes;

	for (bytes = 0; bytes <= BYTES_BEFORE_CHECKSUM; bytes++)
	{
		fseek(file, bytes, SEEK_SET);
		fread(&k, sizeof(UINT8), 1, file);
		checksum += k;
	}

	fclose(file);

	return checksum;
}

int main(int argc, char* argv[])
{
	char* file_path = malloc(strlen(argv[1]));

	strcpy(file_path, argv[1]);

	FILE* savegame_file;

	fopen_s(&savegame_file, file_path, "r+");

	if (savegame_file == NULL)
	{
		printf_s("Error, save game file not found \"%s\"", file_path);
		return EXIT_FAILURE;
	}

	const UINT32 money_offset = 0x1e758;
	DWORD32 money = atoi(argv[2]);

	fseek(savegame_file, money_offset, SEEK_SET);
	fwrite(&money, sizeof(DWORD32), 1, savegame_file);

	fclose(savegame_file);

	UINT32 checksum_value = GetChecksumValue(file_path);

	savegame_file = fopen(file_path, "r+");

	fseek(savegame_file, BYTES_BEFORE_CHECKSUM + 0x01, SEEK_SET);
	fwrite(&checksum_value, sizeof(UINT32), 1, savegame_file);

	fclose(savegame_file);
	free(file_path);

	printf_s("Game save update is success!\n");
	printf_s("checksum value is %X", checksum_value);

	return EXIT_SUCCESS;
}