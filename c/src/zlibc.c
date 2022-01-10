// https://github.com/madler/zlib/blob/master/test/example.c
// Rahul Chalamala

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "zlib.h"

#include "zlibc.h"
#include "utils.h"

void zlib(char** data, char** compressed_data, double* compression_ratio, double* compression_speed, size_t compression_level)
{
	size_t data_size = (size_t)(strlen(*data));

	size_t compressed_data_size = compressBound(data_size);

	*compressed_data = (char*)malloc((size_t)compressed_data_size);
	memset(*compressed_data, 0, compressed_data_size);

	if(*compressed_data == NULL)
	{
		fail("Failed to allocate memory for *compressed_data.", -1);
	}

	clock_t start, delta;

	start = clock();
	int error = compress2((Byte*)*compressed_data, &compressed_data_size, (const Bytef*)*data, data_size, compression_level);
	delta = clock() - start;

	*compression_speed = data_size / 1048576.0 * ((double)CLOCKS_PER_SEC / delta); // MB/s

	if(error != Z_OK)
	{
		fail("zlib failed to compress the data. ", 1);
	}

	*compression_ratio = (float)data_size / compressed_data_size;

	*compressed_data = (char*)realloc(*compressed_data, compressed_data_size); // reallocs memory to compressed_data_size
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

	error = uncompress((Byte*)uncompressed_data, &data_size, (const Byte*)*compressed_data, compressed_data_size);

	if(error != Z_OK)
	{
		fail("zlib failed to uncompress the data. ", 1);
	}

	if(memcmp(*data, uncompressed_data, data_size) != 0)
	{
		fail("Validation failed. original and uncompressed data are not identical.", -1);
	}

	free(uncompressed_data);
}

void zlib1(char** data, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	zlib(data, compressed_data, compression_ratio, compression_speed, 1);
}

void zlib2(char** data, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	zlib(data, compressed_data, compression_ratio, compression_speed, 2);
}

void zlib3(char** data, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	zlib(data, compressed_data, compression_ratio, compression_speed, 3);
}

void zlib4(char** data, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	zlib(data, compressed_data, compression_ratio, compression_speed, 4);
}

void zlib5(char** data, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	zlib(data, compressed_data, compression_ratio, compression_speed, 5);
}

void zlib6(char** data, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	zlib(data, compressed_data, compression_ratio, compression_speed, 6);
}

void zlib7(char** data, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	zlib(data, compressed_data, compression_ratio, compression_speed, 7);
}

void zlib8(char** data, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	zlib(data, compressed_data, compression_ratio, compression_speed, 8);
}

void zlib9(char** data, char** compressed_data, double* compression_ratio, double* compression_speed)
{
	zlib(data, compressed_data, compression_ratio, compression_speed, 9);
}
