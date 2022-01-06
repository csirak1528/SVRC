// Rahul Chalamala

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#include "lz4c.h"

const char* prefix = "/home/rahul/Documents/compression/SHVRC/testfiles/testfile.";

#define FILETYPE_N 10

const char* filetypes[FILETYPE_N] = {"html", "jpg", "js", "json", "mp3", "mp4", "pdf", "png", "txt", "wav"};

int main(void)
{
	const char* const file_path = "/home/rahul/Documents/compression/SHVRC/testfiles/testfile.json";

	char* data;
	char* compressed_data;
	double compression_ratio, compression_speed;

	for(int i = 0; i < FILETYPE_N; ++i)
	{
		char *result = malloc(strlen(prefix) + strlen(filetypes[i]) + 1);
		if(result == NULL)
		{
			fail("Failed to allocate memory for resultant string.", -1);
		}

		strcpy(result, prefix);
		strcat(result, filetypes[i]);

		file_read(result, &data);

		printf("Filetype: .%s\n", filetypes[i]);

		lz4(&data, &compressed_data, &compression_ratio, &compression_speed);

		printf("LZ4 Compression Ratio: %f\n", compression_ratio);
		printf("LZ4 Compression Speed (Mb/s): %f\n\n", compression_speed);

	}
	free(data);
}