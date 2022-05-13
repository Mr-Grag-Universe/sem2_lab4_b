#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include "KGetLine.h"
#include "ctype.h"

void tabs_to_spaces(char * line) {
	if (line == NULL) return;
	if (line[0] == '\0') return;
	int i = 0;
	while (line[i] != '\0') {
		if (line[i] == '\t')
			line[i] = ' ';
		i++;
	}
}

int is_letter(char * line) {
	if (line == NULL)
		return -1;
	if (line[1] != '\0')
		return 0;
	if ((line[0] < 'a' || line[0] > 'z') && (line[0] < 'A' || line[0] > 'Z'))
		return 0;
	return 1;
}

void delete_double_spaces(char ** line) {
    if (*line == NULL)
        return;
    
    //int i = 0;
    char flag = 0;
    int j = 0;
    int old_len = (int) strlen(*line);
    char * res = malloc(sizeof(char) * (old_len+1));
    /*
    while ((*line)[i] != '\0') {
        // printf("%s\n", *line);
        if ((*line)[i] == ' ') {
            if (flag) { // delete spaces
                int ind = i;
                while ((*line)[ind] == ' ')
                    ind++;
                int len = strlen((*line));

                // 0 1 (<-flag) 2 3 4 (<-' ') 5 \0: len = 7
                // printf("here\n");
                // getchar();
                memcpy((*line) + i, (*line) + ind, sizeof(char) * (len-ind+1));
                (*line) = (char*) realloc((*line), sizeof(char) * (len+1-(ind-i)));
                // getchar();
                // printf("%s\n", (*line));
                i--;
                flag = 0;
            }
            else
                flag = 1;
        }
        else
            flag = 0;
        i++;
    }
     */
    for (int i = 0; i < old_len; ++i) {
        if ((*line)[i] == ' ') {
            if (flag == 0 && j) {
                flag = 1;
                res[j] = (*line)[i];
                j++;
            }
        }
        else {
            flag = 0;
            res[j] = (*line)[i];
            j++;
        }
    }
    //j++;
    res[j] = '\0';
    memcpy(*line, res, old_len);
    free(res);
    (*line) = realloc((*line), sizeof(char) * (j+1));
}

unsigned char is_digit(char * s) {
	size_t i = 0;
	while (s[i]) {
		if (s[i] == '-') {
			if (i || strlen(s) == 1)
				return 0;
		}
		else if ((s[i] > '9' || s[i] < '1') && s[i] != '0')
			return 0;
		++i;
	}
	return 1;
}

unsigned char is_float(char * s) {
	size_t i = 0;
	char flag = 0;
	// char min = 0;
	while (s[i]) {
		if ((s[i] > '9' || s[i] < '1') && s[i] != '0') {
			if (s[i] == '.') {
				if (flag)
					return 0;
				flag++;
			}
			else if (s[i] == '-') {
				if (i == 0)
					continue;
				return 0;
			}
			else
				return 0;
		}
		++i;
	}
	return 1;
}

unsigned char is_operator(char * s) {
	char mult[] = "*";
	char sum[] = "+";
	char min[] = "-";
	char dev[] = "/";

	if (s == NULL)
		return 0;

	if (strcmp(s, mult) == 0) return 1;
	else if (strcmp(s, sum) == 0) return 1;
	else if (strcmp(s, min) == 0) return 1;
	else if (strcmp(s, dev) == 0) return 1;
	return 0;
}

char * str_copy(char * line) {
	size_t len = strlen(line);
	char * copy = (char*) malloc((len+1) * sizeof(char));
	memmove(copy, line, sizeof(char) * (len + 1));
	return copy;
}

char ** split(char * line, char * separator) {
	char ** res = NULL;
	int n = 0;

	if (line == NULL)
		return NULL;
	if (line[0] == '\0')
		return NULL;

	char * copy = str_copy(line);

	// getchar();

	char * word = strtok(copy, separator);
	while (word) {
		// printf("%ld, %ld\n", word, line);
		n++;
		res = realloc(res, sizeof(char*) * n);
		res[n-1] = malloc(sizeof(char) * (strlen(word)+1));
		memcpy(res[n-1], word, sizeof(char)*(strlen(word)+1));
		word = strtok(NULL, separator);
	}
	free(copy);
	res = realloc(res, sizeof(char*) * (n+1));
	res[n] = NULL;

	return res;
}

int get_int() {
    int res = 0;
    printf("-> ");
    char * line = get_line();
    while (line == NULL || is_digit(line) == 0) {
        printf("wrong input\n-> ");
        if (line)
            free(line);
        line = get_line();
    }
    res = atoi(line);
    free(line);
    return res;
}

unsigned int get_u_int() {
    unsigned int res = 0;
    printf("-> ");
    char * line = get_line();
    while (line == NULL || is_digit(line) == 0 || line[0] == '-') {
        printf("wrong input\n-> ");
        if (line)
            free(line);
        line = get_line();
    }
    res = (unsigned int) atol(line);
    free(line);
    return res;
}

/// размер без 0 терминотора
char * generate_word(size_t size) {
    char * word = malloc(sizeof(char) * (size+1));
    for (int j = 0; j < size; ++j) {
        char x = rand()%2;
        word[j] = (char)((int)(x ? 'A' : 'a') + rand() % 26);
    }
    word[size] = '\0';
    return word;
}

void line_to_lower(char * line) {
    if (line == NULL)
        return;

    setlocale(LC_ALL, "Rus");

    int i = 0;
    while (line[i++]) {
        char c = line[i-1];
        line[i-1] = (char) tolower(c);
    }
}

int y_n_question_answer(char * question) {
    printf("%s (y/n): ", question);
    char * answer = get_line();
    if (answer == NULL || strcmp("y", answer) != 0) {
        free(answer);
        return 0;
    }
    free(answer);
    return 1;
}