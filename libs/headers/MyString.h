#ifndef MSTR
#define MSTR
void tabs_to_spaces(char * line);
void delete_double_spaces(char ** line);
char ** split(char * line, char * separator);
unsigned char is_digit(char * s);
unsigned char is_float(char * s);
unsigned char is_operator(char * s);
int is_letter(char * line);
int get_int();
unsigned int get_u_int();
char * str_copy(char * line);
char * generate_word(size_t size);
void line_to_lower(char * line);
#endif