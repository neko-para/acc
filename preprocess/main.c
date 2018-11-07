#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

char* readSrc(const char* iFileName, size_t* srcLen) {
	char* src;
	FILE* iFile;
	iFile = fopen(iFileName, "r");
	fseek(iFile, 0, SEEK_END);
	*srcLen = ftell(iFile);
	fseek(iFile, 0, SEEK_SET);
	src = (char*)malloc(sizeof(char) * (*srcLen + 1));
	fread(src, 1, *srcLen, iFile);
	fclose(iFile);
	src[*srcLen] = 0;
	return src;
}

char* removeSplash(char* src, size_t* srcLen) { // This will free src and reallocate new one
	char* newSrc;
	int i, p = 0;
	newSrc = (char*)malloc(sizeof(char) * (*srcLen + 1));
	for (i = 0; i < *srcLen; ++i) {
		if (src[i] == '\\' && i + 1 < *srcLen && src[i + 1] == '\n') {
			++i;
		} else {
			newSrc[p++] = src[i];
		}
	}
	free(src);
	newSrc[p] = 0;
	*srcLen = p;
	return newSrc;
}

char* parse(char* src, size_t* srcLen) {

}

int main(int argc, char* argv[]) {
	const char* oFileName;
	const char* iFileName;
	FILE* oFile;
	char* src;
	size_t srcLen;
	int argpos = 1;
	if (argc < 3) {
		fprintf(stderr, "Usage:\n\tacc.preprocess [[-i] input] [-o output] [-I include]");
		exit(1);
	}
	while (argpos <= argc) {
		if (argv[argpos][0] == '-' && argpos == argc) {
			fprintf(stderr, "ACC-PREPROCESS: [FATAL] Please provide a path\n");
		}
		if (!strcmp(argv[argpos], "-i")) {
			if (argpos)
		}
	}
	src = readSrc(argv[1], &srcLen);
	src = parse(src, &srcLen);
	return 0;
}

