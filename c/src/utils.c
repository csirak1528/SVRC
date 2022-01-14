#include <stdio.h>
#include <stdlib.h>

void fail(const char* message, const int code)
{
	fprintf(stderr, "%s\n", message);
	exit(code);
}

int file_read(const char* file_path, char** content)
{
	FILE* fp;

	if(!(fp = fopen(file_path, "rb")))  // Open the file in binary mode
	{
		fprintf(stderr, "File not found: %s\n", file_path);
		exit(-1);
	}

	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	rewind(fp);

	if(!(*content = (char*)calloc(size, sizeof(unsigned char))))
	{
		fail("Failed to allocate memory\n", -1);
	}

	fread(*content, size, 1, fp);
	fclose(fp);
}
