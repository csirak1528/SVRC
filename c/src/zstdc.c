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

	*compressed_data = (char*)malloc((size_t)max_compressed_size);
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
}


