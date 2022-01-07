// Rahul Chalamala

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"

#include "zstd.h"

void zstd(char** data, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	const size_t data_size = strlen(*data) + 1;

	const size_t max_compressed_size = ZSTD_compressBound(data_size);

	*compressed_data = (char*)malloc(max_compressed_size);
	memset(*compressed_data, 0, max_compressed_size);
	if(*compressed_data == NULL)
	{
		fail("Failed to allocate memory for *compressed_data.", -1);
	}

	clock_t start, delta;

	start = clock();
	const size_t compressed_data_size = ZSTD_compress(*compressed_data, max_compressed_size, *data, data_size, 1);
	delta = clock() - start;

	*compression_speed = data_size / 1048576.0 * ((double)CLOCKS_PER_SEC / delta); // MB/s

	if(ZSTD_isError(compressed_data_size))
	{
		fail(ZSTD_getErrorName(compressed_data_size), -1);
	}

	*compression_ratio = (float)data_size / compressed_data_size;

	*compressed_data = (char*)realloc(*compressed_data, compressed_data_size); // reallocs memory to compressed_data_size
	if(*compressed_data == NULL)
	{
		fail("Failed to re-alloc memory for compressed_data.", -1);
	}

	const size_t uncompressed_data_size = ZSTD_getFrameContentSize(*compressed_data, compressed_data_size);

	if(uncompressed_data_size == ZSTD_CONTENTSIZE_ERROR)
	{
		fail("original not compressed by zstd.", -1);
	}

	if(uncompressed_data_size == ZSTD_CONTENTSIZE_UNKNOWN)
	{
		fail("original size unknown.", -1);
	}

	char* uncompressed_data = (char*)malloc(uncompressed_data_size);
	memset(uncompressed_data, 0, uncompressed_data_size);

	if(uncompressed_data == NULL)
	{
		fail("Failed to allocate memory for *uncompressed_data.", -1);
	}

	const size_t uncompressed_size = ZSTD_decompress(uncompressed_data, uncompressed_data_size, *compressed_data, compressed_data_size);

	if(ZSTD_isError(uncompressed_size))
	{
		fail(ZSTD_getErrorName(uncompressed_size), -1);
	}

	if(memcmp(*data, uncompressed_data, data_size) != 0)
	{
		fail("Validation failed. original and uncompressed data are not identical.", -1);
	}

	free(uncompressed_data);
}

