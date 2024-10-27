#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BORDER_CHAR '#'
#define HORIZONTAL_BORDER_CHAR '-'
#define CORNER_BORDER_CHAR '+'
#define VERTICAL_BORDER_CHAR '|'
#define DELIMITER ','
#define SPACE ' '
#define HELP_FLAG "--help"
//#define TOO_MANY_CHARS "."
//#define RESET "\033[0m"
//#define BLACK "\033[1;30m"
//#define RED "\033[1;31m"

int equal_strings(char *str1, char *str2) {
	if (strlen(str1) != strlen(str2)) {return 0;}
	for (int j = 0; j < (int)strlen(str1); ++j) {
		if (str1[j] != str2[j]) {
			return 0;
		}
	}
	return 1;
}


void print_char_multiple_times(char c, unsigned int q) {
	for (unsigned int i = 0; i < q; ++i) {
		printf("%c", c);
	}
}


int file_exist(char *name) {
	FILE *f = fopen(name, "rt");
	if (f == NULL) {
		return 0;
	}
	return 1;
}


int count_max_cells(char *name) {
	FILE *f = fopen(name, "rt");
	char *line;
	size_t len = 0;
	ssize_t read;
	int max_count = 0;
	while ((read = getline(&line, &len, f)) != -1) {
		int count = 0;
		for (int i = 0; i < (int)strlen(line) - 1; ++i) { // -1 cause last char is \n
			if (line[i] == DELIMITER) {
				count++;
			}
		}
		max_count = count > max_count ? count : max_count;
	}
	fclose(f);
	return max_count == 0 ? 0 : max_count + 1;
}

void get_table_csv(char *name, unsigned int width) {
	FILE *f = fopen(name, "rt");
	char *line;
	size_t len = 0;
	ssize_t read;
	int max_cells = count_max_cells(name);
	print_char_multiple_times(BORDER_CHAR, max_cells * (width + 1) + 1);
	printf("\n");
	
	while ((read = getline(&line, &len, f)) != -1) {
		unsigned int chars_written = 0;
		unsigned int cells_written = 1;
		printf("%c", BORDER_CHAR);
		for (int i = 0; i < (int)strlen(line) - 1; ++i) { // -1 cause last char is \n
			if ((line[i] != DELIMITER) && chars_written < width) {
				printf("%c", line[i]);
				++chars_written;
			}
			else if (line[i] == DELIMITER) {
				if (chars_written < width) {
					print_char_multiple_times(SPACE, width-chars_written);
				}
				cells_written++;
				chars_written = 0;
				printf("%c", BORDER_CHAR);
			}
		}
		if ((int)cells_written != max_cells) {
			for (int i = 0; i < (int)max_cells - (int)cells_written; ++i) {
				printf("%c", BORDER_CHAR);
				print_char_multiple_times(SPACE, width);
			}
		}
		printf("%c", BORDER_CHAR);
		printf("\n");
	}

	print_char_multiple_times(BORDER_CHAR, max_cells * (width + 1) + 1);
	printf("\n");
	
	fclose(f);
}


int main(int argc, char **argv){
	if (equal_strings(argv[1], HELP_FLAG)) {
		printf("usage:\nsee_csv [filename.csv]\n");
		return 0;
	}
	if (file_exist(argv[1]) == 0) {
		printf("No such file found. --help for help");
		return 0;
	}
	unsigned int width;
	printf("Enter cell width: ");
	scanf("%u", &width);
	get_table_csv(argv[1], width);
	return 0;
}
