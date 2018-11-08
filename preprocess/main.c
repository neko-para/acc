#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include "config.h"
#include "vector.h"

struct defineInfo {
	char* name;
	vector* params;
	char* src;
};

vector* includePath;
vector* defines;

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

void writeSrc(const char* oFileName, const char* src, size_t srcLen) {
	FILE* oFile;
	oFile = fopen(oFileName, "w");
	fwrite(src, 1, srcLen, oFile);
	fclose(oFile);
}

char* removeSplash(char* src, size_t* srcLen) { // This will free src and reallocate new one
	char* newSrc;
	size_t i, p = 0;
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

char* removeComment(char* src, size_t* srcLen) {
	char* newSrc;
	size_t i, p = 0;
	newSrc = (char*)malloc(sizeof(char) * (*srcLen + 1));
	for (i = 0; i < *srcLen; ++i) {
		if (src[i] == '/' && src[i + 1] == '*') {
			i += 2;
			while (1) {
				if (src[i] == '*' && src[i + 1] == '/') {
					i += 2;
					break;
				} else if (!src[i]) {
					fprintf(stderr, "ACC-PREPROCESS: [WARNING] Unterminate comment\n");
					break;
				}
				++i;
			}
			newSrc[p++] = ' ';
		} else if (src[i] == '"' || src[i] == '\'') {
			int start = i;
			char stopc = src[i++];
			while (1) {
				if (src[i] == stopc) {
					break;
				} else if (src[i] == '\\') {
					++i;
					switch (src[i]) {
					case '\'':
					case '"':
					case '\\':
					case '?':
					case 'a':
					case 'b':
					case 'f':
					case 'n':
					case 'r':
					case 't':
					case 'v':
						break;
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7': {
						int r = 2;
						while (r--) {
							if (src[i + 1] >= '0' && src[i + 1] <= '7') {
								++i;
							} else {
								break;
							}
						}
						break;
					}
					case 'x': {
						int r = 4;
						while (r--) {
							if (isxdigit(src[i + 1])) {
								++i;
							} else {
								break;
							}
						}
						break;
					}
					default:
						fprintf(stderr, "ACC-PREPROCESS: [FATAL] Unknown escape \\%c\n", src[i]);
						exit(1);
					}
					++i;
				} else if (!src[i]) {
					fprintf(stderr, "ACC-PREPROCESS: [FATAL] Unterminate %s\n", stopc == '"' ? "string" : "character");
					exit(1);
				}
			}
			memcpy(newSrc + p, src + start, i - start + 1);
			p += i - start + 1;
		}
	}
	free(src);
	newSrc[p] = 0;
	*srcLen = p;
	return newSrc;
}

char* parse(char* src, size_t* srcLen) {
	src = removeComment(removeSplash(src, srcLen), srcLen);
	// TODO: solve #... && defines
	return src;
}

void addDefineCLI(char* str) {
	char* eq = strchr(str, '=');
	if (!eq) {
		fprintf(stderr, "ACC-PREPROCESS: [FATAL] Defines should contain at least a `='\n");
		exit(1);
	}
}

int main(int argc, char* argv[]) {
	char* oFileName = NULL;
	char* iFileName = NULL;
	char* src;
	size_t srcLen;
	int argpos = 1;
	if (argc < 3) {
		fprintf(stderr, "Usage:\n\tacc.preprocess [[-i] input] [-o output] [-I include] [-D[ ]macro=define]");
		exit(1);
	}
	includePath = vector_create_init(1);
	vector_push_back(includePath, ACC_INCLUDE_PATH);
	while (argpos < argc) {
		if (argv[argpos][0] == '-' && argpos + 1 == argc) {
			if (argv[argpos][1] != 'D') {
				if (!argv[argpos][2]) {
					fprintf(stderr, "ACC-PREPROCESS: [FATAL] Please provide a defination\n");
					exit(1);
				}
			} else {
				fprintf(stderr, "ACC-PREPROCESS: [FATAL] Please provide a path\n");
				exit(1);
			}
		}
		if (!strcmp(argv[argpos], "-i")) {
			iFileName = argv[++argpos];
		} else if (!strcmp(argv[argpos], "-o")) {
			oFileName = argv[++argpos];
		} else if (!strcmp(argv[argpos], "-I")) {
			vector_push_back(includePath, argv[++argpos]);
		} else if (!strcmp(argv[argpos], "-D")) {
			addDefineCLI(argv[++argpos]);
		} else if (!strncmp(argv[argpos], "-D", 2)) {
			addDefineCLI(argv[argpos] + 2);
		} else if (!strcmp(argv[argpos], "-h") || !strcmp(argv[argpos], "--help")) {
			fprintf(stderr, "Usage:\n\tacc.preprocess [[-i] input] [-o output] [-I include] [-D[ ]macro=define]");
			exit(0);
		} else {
			fprintf(stderr, "ACC-PREPROCESS: [FATAL] Unknown command %s\n", argv[argpos]);
			fprintf(stderr, "Usage:\n\tacc.preprocess [[-i] input] [-o output] [-I include] [-D[ ]macro=define]");
			exit(1);
		}
		++argpos;
	}
	if (!iFileName) {
		fprintf(stderr, "ACC-PREPROCESS: [FATAL] You must specify a input file\n");
		exit(1);
	}
	src = readSrc(iFileName, &srcLen);
	src = parse(src, &srcLen);
	if (!oFileName) {
		oFileName = (char*)malloc(strlen(iFileName) + 2);
		strcpy(oFileName, iFileName);
		strcat(oFileName, ".i");
	}
	writeSrc(oFileName, src, srcLen);
	return 0;
}

