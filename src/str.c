#include <stdint.h>

#include "kernel.h"

char* itoa(int val, int base){
  if(val == 0){
    return "0";
  }else{
    static char buf[32] = {0};
  
    int i = 30;
  
    for(; val && i ; --i, val /= base)
      
      buf[i] = "0123456789abcdef"[val % base];
  
    return &buf[i+1];
  }
}

int atoi(char *str){
    int res = 0; // Initialize result
  
    // Iterate through all characters of input string and
    // update result
    for (int i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
  
    // return result.
    return res;
}

int strcmp(const char *s1, const char *s2){
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    while(*p1 != '\0'){
        if (*p2 == '\0') return  1;
        if (*p2 > *p1)   return -1;
        if (*p1 > *p2)   return  1;

        p1++;
        p2++;
    }

    if(*p2 != '\0')return -1;

    return 0;
}

char *strchr(const char *s, int c){
    while (*s != (char)c)
        if (!*s++)
            return 0;
    return (char *)s;
}