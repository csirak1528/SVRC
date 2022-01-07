// Rahul Chalamala

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#include "lz4c.h"
#include "zstdc.h"

const char* prefix = "/home/rahul/Documents/compression/SHVRC/testfiles/testfile.";

#define FILETYPE_N 2

const char* filetypes[FILETYPE_N] = {"html", "json"};

int main(void)
{
	char* data;
	char* compressed_data;
	double compression_ratio, compression_speed;

	for(int i = 0; i < FILETYPE_N; ++i)
	{
		char* result = malloc(strlen(prefix) + strlen(filetypes[i]) + 1);
		if(result == NULL)
		{
			fail("Failed to allocate memory for resultant string.", -1);
		}

		strcpy(result, prefix);
		strcat(result, filetypes[i]);

		file_read(result, &data);

		free(result);

		printf("Filetype: .%s\n", filetypes[i]);

		lz4(&data, &compressed_data, &compression_ratio, &compression_speed);

		printf("lz4 Compression Ratio: %f\n", compression_ratio);
		printf("lz4 Compression Speed (MB/s): %f\n\n", compression_speed);

		free(compressed_data);

		zstd(&data, &compressed_data, &compression_ratio, &compression_speed);

		printf("Zstd Compression Ratio: %f\n", compression_ratio);
		printf("Zstd Compression Speed (MB/s): %f\n\n", compression_speed);

		free(data);
		free(compressed_data);
	}
}