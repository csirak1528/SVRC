// Rahul Chalamala

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#include "lz4c.h"
#include "zstdc.h"
#include "zlibc.h"
#include "snappyc.h"

const char* prefix = "/home/rahul/Documents/compression/SHVRC/testfiles/testfile.";

#define TESTFILE_COUNT 2
#define ALGORITHM_COUNT 20
#define TRIALS 100

const char* filetypes[] = {"html", "json"};
char* algorithm_names[] = {"lz4",
						   "zstd1", "zstd2","zstd3","zstd4","zstd5","zstd6","zstd7","zstd8","zstd9",
						   "zlib1", "zlib2","zlib3","zlib4","zlib5","zlib6","zlib7","zlib8","zlib9",
						   "snappy"};

double average_compression_ratio[TESTFILE_COUNT][ALGORITHM_COUNT], average_compression_speed[TESTFILE_COUNT][ALGORITHM_COUNT];

void (* algorithms[])(char**, char**, double*, double*) = {lz4,
														   zstd1, zstd2, zstd3, zstd4, zstd5, zstd6, zstd7, zstd8, zstd9,
														   zlib1, zlib2, zlib3, zlib4, zlib5, zlib6, zlib7, zlib8, zlib9,
														   snappy};

char* data;
char* compressed_data;
double compression_ratio, compression_speed;

int main(void)
{
	for(int trial = 0; trial < TRIALS; ++trial)
	{
		for(int testfile = 0; testfile < TESTFILE_COUNT; ++testfile)
		{
			char* result = malloc(strlen(prefix) + strlen(filetypes[testfile]) + 1);
			if(result == NULL)
			{
				fail("Failed to allocate memory for resultant string.", -1);
			}

			strcpy(result, prefix);
			strcat(result, filetypes[testfile]);

			file_read(result, &data);

			free(result);

			for(int algorithm = 0; algorithm < ALGORITHM_COUNT; ++algorithm)
			{
				(*algorithms[algorithm])(&data, &compressed_data, &compression_ratio, &compression_speed);

				average_compression_ratio[testfile][algorithm] += compression_ratio;
				average_compression_speed[testfile][algorithm] += compression_speed;

				free(compressed_data);
			}

			free(data);

			/*
			printf("Filetype: .%s\n", filetypes[testfile]);

			lz4(&data, &compressed_data, &compression_ratio, &compression_speed);

			printf("lz4 Compression Ratio: %f\n", compression_ratio);
			printf("lz4 Compression Speed (MB/s): %f\n\n", compression_speed);

			free(compressed_data);

			zstd(&data, &compressed_data, &compression_ratio, &compression_speed);

			printf("zstd Compression Ratio: %f\n", compression_ratio);
			printf("zstd Compression Speed (MB/s): %f\n\n", compression_speed);

			free(compressed_data);
			free(data);
			*/
		}
	}

	for(int testfile = 0; testfile < TESTFILE_COUNT; ++testfile)
	{
		for(int algorithm = 0; algorithm < ALGORITHM_COUNT; ++algorithm)
		{
			average_compression_ratio[testfile][algorithm] /= TRIALS;
			average_compression_speed[testfile][algorithm] /= TRIALS;
		}
	}

	/*
	for(int testfile = 0; testfile < TESTFILE_COUNT; ++testfile)
	{
		printf("Filetype: .%s\n", filetypes[testfile]);
		for(int algorithm = 0; algorithm < ALGORITHM_COUNT; ++algorithm)
		{
			printf("%s Compression Ratio: %f\n", algorithm_names[algorithm], average_compression_ratio[testfile][algorithm]);
			printf("%s Compression Speed (MB/s): %f\n\n", algorithm_names[algorithm], average_compression_speed[testfile][algorithm]);
		}
	}
	*/

	const char* json_file_path = "/home/rahul/Documents/compression/SHVRC/c/output.json";

	FILE* fp;

	if(!(fp = fopen(json_file_path, "w+")))
	{
		fprintf(stderr, "File not found: %s\n", json_file_path);
		exit(-1);
	}

	fprintf(fp, "{\n\t\"trials\": %d,\n", TRIALS);

	for(int testfile = 0; testfile < TESTFILE_COUNT; ++testfile)
	{
		fprintf(fp, "\t\"%s\":\n\t\t{\n", filetypes[testfile]);
		for(int algorithm = 0; algorithm < ALGORITHM_COUNT; ++algorithm)
		{
			fprintf(fp, "\t\t\t\"%s\":\n\t\t\t\t{\n\t\t\t\t\t\"ratio\": %f,\n\t\t\t\t\t\"speed\": %f\n\t\t\t\t}", algorithm_names[algorithm], average_compression_ratio[testfile][algorithm], average_compression_speed[testfile][algorithm]);
			if(algorithm < ALGORITHM_COUNT - 1)
			{
				fputc(',', fp);
			}
			fputc('\n', fp);
		}
		fprintf(fp, "\t\t}");
		if(testfile < TESTFILE_COUNT - 1)
		{
			fputc(',', fp);
		}
		fputc('\n', fp);
	}

	fputs("}\n", fp);
}