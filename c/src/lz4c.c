// https://github.com/lz4/lz4/blob/dev/examples/simple_buffer.c
// Rahul Chalamala

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "lz4.h"

#include "lz4c.h"
#include "utils.h"

void lz4(const char* file_path, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	char* data;

	file_read(file_path, &data);

	const int data_size = (int)(strlen(data) + 1);
	const int max_compressed_size = LZ4_compressBound(data_size);

	*compressed_data = (char*)malloc((size_t)max_compressed_size);
	if(*compressed_data == NULL)
	{
		fail("Failed to allocate memory for *compressed_data.", -1);
	}

	clock_t start, delta;

	start = clock();
	const int compressed_data_size = LZ4_compress_default(data, *compressed_data, data_size, max_compressed_size);
	delta = clock() - start;

	*compression_speed = data_size / 1000000.0 / ((double)delta / CLOCKS_PER_SEC); // Mb/s

	if(compressed_data_size <= 0)
	{
		fail("A 0 or negative result from LZ4_compress_default() indicates a failure trying to compress the data. ", 1);
	}
	*compression_ratio = (float)compressed_data_size / data_size;

	*compressed_data = (char*)realloc(*compressed_data, (size_t)compressed_data_size); // reallocs memory to compressed_data_size
	if(*compressed_data == NULL)
	{
		fail("Failed to re-alloc memory for compressed_data.", -1);
	}

	char* uncompressed_data = (char*)malloc(data_size);
	if(uncompressed_data == NULL)
	{
		fail("Failed to allocate memory for *uncompressed_data.", -1);
	}

	const int decompressed_size = LZ4_decompress_safe(*compressed_data, uncompressed_data, compressed_data_size, data_size);

	if(decompressed_size < 0)
	{
		fail("A negative result from LZ4_decompress_safe indicates a failure trying to decompress the data.  See exit code (echo $?) for value returned.", decompressed_size);
	}

	if(decompressed_size != data_size)
	{
		fail("Decompressed data is different from original!\n", -1);
	}

	if(memcmp(data, uncompressed_data, data_size) != 0)
	{
		fail("Validation failed. original and uncompressed data are not identical.", -1);
	}

	free(data);
	free(uncompressed_data);
}

int main(void)
{
	const char* const file_path = "/home/rahul/Documents/compression/SHVRC/c/test.json";
	//const char* const file_path = "/home/rahul/Documents/compression/SHVRC/c/citylots.json";

	char* compressed_data;
	double compression_ratio, compression_speed;

	lz4(file_path, &compressed_data, &compression_ratio, &compression_speed);

	printf("Compression Ratio: %f\n", compression_ratio);
	printf("Compression Speed (Mb/s): %f\n", compression_speed);
}