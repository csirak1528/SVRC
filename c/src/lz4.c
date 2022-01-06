// https://github.com/lz4/lz4/blob/dev/examples/simple_buffer.c
// Rahul Chalamala

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "lz4.h"
#include "utils.h"

struct LZ4
{
	con
};
typedef struct LZ4 LZ4;

int main(void)
{
	//const char* const filepath = "/home/rahul/Documents/compression/SHVRC/c/test.json";
	const char* const filepath = "/home/rahul/Documents/compression/SHVRC/c/citylots.json";

	char* src;

	file_read(filepath, &src);

	const int src_size = (int)(strlen(src) + 1);
	// LZ4 provides a function that will tell you the maximum size of compressed output based on input data via LZ4_compressBound().
	const int max_dst_size = LZ4_compressBound(src_size);
	printf("original size: %d\n", src_size);
	printf("max compression size: %d\n", max_dst_size);
	// We will use that size for our destination boundary when allocating space.
	char* compressed_data = (char*)malloc((size_t)max_dst_size);
	if(compressed_data == NULL)
	{
		fail("Failed to allocate memory for *compressed_data.", 1);
	}
	// That's all the information and preparation LZ4 needs to compress *src into *compressed_data.
	// Invoke LZ4_compress_default now with our size values and pointers to our memory locations.
	// Save the return value for error checking.

	clock_t start, diff;
	start = clock();
	const int compressed_data_size = LZ4_compress_default(src, compressed_data, src_size, max_dst_size);
	diff = clock() - start;
	float sec = (float)diff / CLOCKS_PER_SEC;

	printf("Compression speed: %f Mb/s\n", src_size / 1000000.0 / sec);
	// Check return_value to determine what happened.
	if(compressed_data_size <= 0)
	{
		fail("A 0 or negative result from LZ4_compress_default() indicates a failure trying to compress the data. ", 1);
	}
	if(compressed_data_size > 0)
	{
		printf("We successfully compressed some data! Ratio: %.2f\n", (float)compressed_data_size / src_size);
	}
	// Not only does a positive return_value mean success, the value returned == the number of bytes required.
	// You can use this to realloc() *compress_data to free up memory, if desired.  We'll do so just to demonstrate the concept.
	compressed_data = (char*)realloc(compressed_data, (size_t)compressed_data_size);
	if(compressed_data == NULL)
	{
		fail("Failed to re-alloc memory for compressed_data.  Sad :(", 1);
	}


	/* Decompression */
	// Now that we've successfully compressed the information from *src to *compressed_data, let's do the opposite!
	// The decompression will need to know the compressed size, and an upper bound of the decompressed size.
	// In this example, we just re-use this information from previous section,
	// but in a real-world scenario, metadata must be transmitted to the decompression side.
	// Each implementation is in charge of this part. Oftentimes, it adds some header of its own.
	// Sometimes, the metadata can be extracted from the local context.

	// First, let's create a *new_src location of size src_size since we know that value.
	char* const regen_buffer = (char*)malloc(src_size);
	if(regen_buffer == NULL)
	{
		fail("Failed to allocate memory for *regen_buffer.", 1);
	}
	// The LZ4_decompress_safe function needs to know where the compressed data is, how many bytes long it is,
	// where the regen_buffer memory location is, and how large regen_buffer (uncompressed) output will be.
	// Again, save the return_value.
	const int decompressed_size = LZ4_decompress_safe(compressed_data, regen_buffer, compressed_data_size, src_size);
	free(compressed_data);   /* no longer useful */
	if(decompressed_size < 0)
	{
		fail("A negative result from LZ4_decompress_safe indicates a failure trying to decompress the data.  See exit code (echo $?) for value returned.", decompressed_size);
	}
	if(decompressed_size >= 0)
	{
		printf("We successfully decompressed some data!\n");
	}
	// Not only does a positive return value mean success,
	// value returned == number of bytes regenerated from compressed_data stream.
	if(decompressed_size != src_size)
	{
		fail("Decompressed data is different from original! \n", 1);
	}

	/* Validation */
	// We should be able to compare our original *src with our *new_src and be byte-for-byte identical.
	if(memcmp(src, regen_buffer, src_size) != 0)
	{
		fail("Validation failed.  *src and *new_src are not identical.", 1);
	}
	//printf("Validation done. The string we ended up with is:\n%s\n", regen_buffer);

	free(src);
	free(regen_buffer);
	return 0;
}