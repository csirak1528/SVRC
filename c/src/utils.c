#include <stdio.h>
#include <stdlib.h>

void fail(const char* message, const int code)
{
	fprintf(stderr, "%s\n", message);
	exit(code);
}

void file_read(const char* file_path, char** content)
{
	FILE* fp;

	if(!(fp = fopen(file_path, "rb")))  // Open the file in binary mode
	{
		fprintf(stderr, "File not found: %s\n", file_path);
		exit(-1);
	}

	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if(!(*content = (char*)malloc(len * sizeof(char))))
	{
		fail("Failed to allocate memory\n", -1);
	}

	fread(*content, len, 1, fp);
	fclose(fp);
}
