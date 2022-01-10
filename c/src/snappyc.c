// Rahul Chalamala

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "snappy-c.h"

#include "snappyc.h"
#include "utils.h"

void snappy(char** data, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	size_t data_size = strlen(*data) + 1;

	size_t compressed_data_size = snappy_max_compressed_length(data_size);

	*compressed_data = (char*)calloc(compressed_data_size, sizeof(unsigned char));
	if(*compressed_data == NULL)
	{
		fail("Failed to allocate memory for *compressed_data.", -1);
	}

	clock_t start, delta;

	start = clock();
	size_t flag = snappy_compress(*data, data_size, *compressed_data, &compressed_data_size);
	delta = clock() - start;

	*compression_speed = data_size * CLOCKS_PER_SEC / (1048576.0 * delta); // MB/s

	if (flag != 0)
	{
		fail("snappy failed to compress.", -1);
	}

	*compression_ratio = (double)data_size / compressed_data_size;

	*compressed_data = (char*)realloc(*compressed_data, (size_t)compressed_data_size); // reallocs memory to compressed_data_size
	if(*compressed_data == NULL)
	{
		fail("Failed to re-alloc memory for compressed_data.", -1);
	}

	char* uncompressed_data = (char*)calloc(data_size, sizeof(unsigned char));
	if(uncompressed_data == NULL)
	{
		fail("Failed to allocate memory for *uncompressed_data.", -1);
	}

	flag = snappy_uncompress(*compressed_data, compressed_data_size, uncompressed_data, &data_size);

	if(flag != 0)
	{
		fail("snappy failed to uncompress.", -1);
	}

	if(memcmp(*data, uncompressed_data, data_size) != 0)
	{
		fail("Validation failed. original and uncompressed data are not identical.", -1);
	}

	free(uncompressed_data);
}
