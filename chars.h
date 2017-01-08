#ifndef CHARS_H_INCLUDED
#define CHARS_H_INCLUDED

unsigned int chars_occur(char* str, char delim);
unsigned int chars_len(char* str);
int chars_find(char* str, char c, unsigned int from);
int chars_rfind(char* str, char c, unsigned int from);
char* chars_substr(char* str, unsigned int from, unsigned int to);
char** chars_split(char* str, char delim, int *parts);
int chars_finds(char* str, char* sub, int from);

#endif // CHARS_H_INCLUDED
