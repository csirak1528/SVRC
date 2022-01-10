// https://github.com/lz4/lz4/blob/dev/examples/simple_buffer.c
// Rahul Chalamala

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "lz4.h"

#include "lz4c.h"
#include "utils.h"

void lz4(char** data, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	const int data_size = (int)(strlen(*data));

	const int max_compressed_size = LZ4_compressBound(data_size);

	*compressed_data = (char*)malloc((size_t)max_compressed_size);
	memset(*compressed_data, 0, max_compressed_size);

	if(*compressed_data == NULL)
	{
		fail("Failed to allocate memory for *compressed_data.", -1);
	}

	clock_t start, delta;

	start = clock();
	const int compressed_data_size = LZ4_compress_default(*data, *compressed_data, data_size, max_compressed_size);
	delta = clock() - start;

	*compression_speed = data_size / 1048576.0 * ((double)CLOCKS_PER_SEC / delta); // MB/s

	if(compressed_data_size <= 0)
	{
		fail("A 0 or negative result from LZ4_compress_default() indicates a failure trying to compress the data. ", 1);
	}
	*compression_ratio = (float)data_size / compressed_data_size;

	*compressed_data = (char*)realloc(*compressed_data, (size_t)compressed_data_size); // reallocs memory to compressed_data_size
	if(*compressed_data == NULL)
	{
		fail("Failed to re-alloc memory for compressed_data.", -1);
	}

	char* uncompressed_data = (char*)malloc(data_size);
	memset(uncompressed_data, 0, data_size);

	if(uncompressed_data == NULL)
	{
		fail("Failed to allocate memory for *uncompressed_data.", -1);
	}

	const int uncompressed_size = LZ4_decompress_safe(*compressed_data, uncompressed_data, compressed_data_size, data_size);

	if(uncompressed_size < 0)
	{
		fail("A negative result from LZ4_uncompress_safe indicates a failure trying to uncompress the data.  See exit code (echo $?) for value returned.", uncompressed_size);
	}

	if(uncompressed_size != data_size)
	{
		fail("uncompressed size is different from original!\n", -1);
	}

	if(memcmp(*data, uncompressed_data, data_size) != 0)
	{
		fail("Validation failed. original and uncompressed data are not identical.", -1);
	}

	free(uncompressed_data);
}
