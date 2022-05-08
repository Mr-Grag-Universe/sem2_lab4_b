#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

char * get_line() {
	setlocale(LC_ALL, "Rus");

	char * res = NULL;
	char n = 0;
	int old_len = 0;
	char buf[101] = {0};
	
	do {
		n = scanf("%100[^\n]", buf);
		int chunck_len = strlen(buf);
		//int old_len = strlen(res);

		if (n == 1) {
			res = (char *) realloc(res, sizeof(char) * (old_len + chunck_len + 1));
			memcpy(res+old_len, buf, chunck_len);
			old_len += chunck_len;
		}

		else if (!n) scanf("%*c");

		else if (!old_len) return NULL;

	} while (n > 0);

	if (old_len > 0) res[old_len] = '\0';
	else res = (char *) calloc(1, sizeof(char));

	return res;
}
