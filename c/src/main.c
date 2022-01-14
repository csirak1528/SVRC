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

#define TESTFILE_COUNT 3
#define ALGORITHM_COUNT 4
#define COMPRESSION_MAX 11 // max compression level + 2
#define TRIALS 10

const char* filetypes[] = {"html", "json", "js"};

const char* algorithm_names[] = {"lz4", "zstd","zlib", "snappy"};

void (* algorithms[])(char**, char**, double*, double*, size_t) = {lz4, zstd, zlib, snappy};

int compression_level_lower_bound[] = {0, 1, 1, 0};
int compression_level_upper_bound[] = {0, 9, 9, 0};

double average_compression_ratio[TESTFILE_COUNT][ALGORITHM_COUNT][COMPRESSION_MAX], average_compression_speed[TESTFILE_COUNT][ALGORITHM_COUNT][COMPRESSION_MAX];

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

			if(strlen(data) > 0)
			{
				for(int algorithm = 0; algorithm < ALGORITHM_COUNT; ++algorithm)
				{
					for(int compression_level = compression_level_lower_bound[algorithm]; compression_level <= compression_level_upper_bound[algorithm]; ++compression_level)
					{
						(*algorithms[algorithm])(&data, &compressed_data, &compression_ratio, &compression_speed, compression_level);

						average_compression_ratio[testfile][algorithm][compression_level + (compression_level != 0)] += compression_ratio;
						average_compression_speed[testfile][algorithm][compression_level + (compression_level != 0)] += compression_speed;

						free(compressed_data);
					}
				}
			}

			free(data);
		}
	}

	for(int testfile = 0; testfile < TESTFILE_COUNT; ++testfile)
	{
		for(int algorithm = 0; algorithm < ALGORITHM_COUNT; ++algorithm)
		{
			for(int compression_level = compression_level_lower_bound[algorithm]; compression_level <= compression_level_upper_bound[algorithm]; ++compression_level)
			{
				average_compression_ratio[testfile][algorithm][compression_level + (compression_level != 0)] /= TRIALS;
				average_compression_speed[testfile][algorithm][compression_level + (compression_level != 0)] /= TRIALS;
			}
		}
	}

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
			if(compression_level_lower_bound[algorithm] == 0 && compression_level_upper_bound[algorithm] == 0)
			{
				fprintf(fp, "\t\t\t\"%s\":\n\t\t\t\t{\n\t\t\t\t\t\"ratio\": %.10f,\n\t\t\t\t\t\"speed\": %.10f\n\t\t\t\t}", algorithm_names[algorithm], average_compression_ratio[testfile][algorithm][0], average_compression_speed[testfile][algorithm][0]);
			}
			else
			{
				fprintf(fp, "\t\t\t\"%s\":\n\t\t\t\t{\n", algorithm_names[algorithm]);
				for(int compression_level = compression_level_lower_bound[algorithm]; compression_level <= compression_level_upper_bound[algorithm]; ++compression_level)
				{
					fprintf(fp, "\t\t\t\t\t\"%d\":\n\t\t\t\t\t\t{\n\t\t\t\t\t\t\t\"ratio\": %.10f,\n\t\t\t\t\t\t\t\"speed\": %.10f\n\t\t\t\t\t\t}", compression_level, average_compression_ratio[testfile][algorithm][compression_level + (compression_level != 0)], average_compression_speed[testfile][algorithm][compression_level + (compression_level != 0)]);
					if(compression_level < compression_level_upper_bound[algorithm])
					{
						fputc(',', fp);
					}
					fputc('\n', fp);
				}
				fprintf(fp, "\t\t\t\t}");
			}
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