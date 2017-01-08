#include <stdio.h>
#include <stdlib.h>

unsigned int chars_occur(char* str, char delim){
	unsigned int occurences = 0;
	unsigned int counter = 0;
	while(str[counter]){
        if(str[counter] == delim)
            ++occurences;
        ++counter;
	}
	return occurences;
}

unsigned int chars_len(char* str){
    unsigned int counter = 0;
    while(str[counter]){
        ++counter;
    }
    return counter;
}


int chars_find(char* str, char c, unsigned int from){
    int counter = 0;
    unsigned int length = chars_len(str);
    if(from >= length){
        return -1;
    }
    for(counter = 0; counter < (length - from); counter++){
        if(str[from + counter] == c)
            return counter;
    }
    return -1;
}

int chars_rfind(char* str, char c, unsigned int from){
    int counter = 0;
    unsigned int length = chars_len(str);
    if(from >= length){
        return -1;
    }
    for(counter = length - from - 1; counter >= 0; counter--){
        if(str[counter] == c)
            return counter;
    }
    return -1;
}

char* chars_substr(char* str, unsigned int from, unsigned int to){
    unsigned int i = 0;
    unsigned int length = chars_len(str);
    char* retval = (char*)malloc((to - from + 1) * sizeof(char));
    if((from >= length) || (to > length)){
        return NULL;
    }
    for(i = 0; i < (to - from); i++){
        retval[i] = str[from + i];
    }
    retval[to - from] = 0;
    return retval;
}

char** chars_split(char* str, char delim, int *parts){
    unsigned int i, j;
    unsigned int start = 0, finish;
    unsigned int slices = chars_occur(str, delim) + 1;
    char** retval;
    retval = (char**)malloc(slices * sizeof(char*));
    for(i = 0; i < slices; i++){
        finish = chars_find(str, delim, start);
        if(finish == -1)
            finish = chars_len(str);
        retval[i] = (char*)malloc(sizeof(char) * (finish + 1));
        for(j = 0; j < finish; j++){
            retval[i][j] = str[start + j];
        }
        retval[i][finish] = 0;
        start = start + finish + 1;
    }
    if(parts != NULL)
        *(parts) = slices;
    return retval;
}

int chars_finds(char* str, char* sub, int from){
    int i, matches, last, pos = from;
    unsigned int lenstr = chars_len(str);
    unsigned int lensub = chars_len(sub);
    while(pos < (lenstr - lensub)){
        matches = 0;
        for(i = 0; i < lensub; i++){
            if(str[pos + i] == sub[i])
                ++matches;
        }
        if(matches == lensub)
            return pos;
        last = chars_rfind(sub, str[pos + lensub - 1], 1);
        // ++pos;
        if(last != -1)
            pos += lensub - last - 1;
        else
            pos += lensub;
    }
    return -1;
}
