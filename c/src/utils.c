#include <stdio.h>
#include <stdlib.h>

void fail(const char* message, const int code)
{
	fprintf(stderr, "%s \n", message);
	exit(code);
}

void file_read(const char* filepath, char **content)
{
	FILE* fp;

	if(!(fp = fopen(filepath, "rb")))  // Open the file in binary mode
	{
		fail("file not found", -1);
	}

	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (!(*content = (char*)malloc(len * sizeof(char)))) {
		fail("Failed to allocate memory\n", -1);
	}

	fread(*content, len, 1, fp);
	fclose(len);
}
