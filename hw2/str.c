// name: 최진성
// # of assignment: 2
// name of the file: str.c


#include <assert.h> /* to use assert() */
#include <stdio.h>
#include "str.h"

/* Your task is:
   1. Rewrite the body of "Part 1" functions - remove the current
      body that simply calls the  corresponding C standard library
      function.
   2. Write appropriate comment per each function
*/

/* Part 1 */
/*------------------------------------------------------------------------*/
size_t StrGetLength(const char* pcSrc)
{
    const char *pcEnd;
    assert(pcSrc); /* NULL address, 0, and FALSE are identical. */
    pcEnd = pcSrc;

    while (*pcEnd) /* null character and FALSE are identical. */
        pcEnd++;

    return (size_t)(pcEnd - pcSrc);
}

/*------------------------------------------------------------------------*/
char *StrCopy(char *pcDest, const char* pcSrc)
{
    assert(pcSrc); /* NULL address, 0, and FALSE are identical. */

    char *checkpoint=pcDest; // checkpoint for the starting point of pcDest

    while(*pcSrc!='\0'){ // perform until *pcSrc reaches null byte
        *pcDest=*pcSrc; // copy each character of pcSrc to pcDest
        pcDest++;
        pcSrc++;
    }

    *pcDest='\0'; //add null byte at the end of pcDest

    return checkpoint; // return starting point of pcDest
}

/*------------------------------------------------------------------------*/
int StrCompare(const char* pcS1, const char* pcS2)
{
    assert(pcS1); /* NULL address, 0, and FALSE are identical. */
    assert(pcS2); /* NULL address, 0, and FALSE are identical. */

    while(*pcS1==*pcS2 && (*pcS1!='\0' && *pcS2!='\0')){
        /* perform while each character of pcS1 is same to pcS2
           and *pcS1 and *pcS2 is not null byte
        */
        pcS1++;
        pcS2++;
    }

    return *pcS1-*pcS2;
    /* positive integer if pcS1 is greater than pcS2
       0 if both strings are identical
       negative integer if pcS1 is smaller than pcS2
    */
}
/*------------------------------------------------------------------------*/
char *StrSearch(const char* pcHaystack, const char *pcNeedle)
{
    assert(pcHaystack); /* NULL address, 0, and FALSE are identical. */

    if(*pcNeedle=='\0'){ // if pcNeedle is empty string, return pcHaystack
        return (char *)pcHaystack;
    }

    while(*pcHaystack!='\0'){ // perform until pcHaycheck reaches null byte
        const char *checkpoint=pcHaystack;
        const char *initialpcNeedle=pcNeedle;
        //checkpoint for possible point pcNeedle appears

        while(*pcHaystack==*pcNeedle && (*pcHaystack!='\0' && *pcNeedle!='\0')){
            /* when pcNeedle first character matches the character
               in pcHaystack in some point, it checks for whole string
            */
            pcHaystack++;
            pcNeedle++;
        }

        if(*pcNeedle=='\0')
            return (char *)checkpoint;
        /* after performing while loop, if pcNeedle reaches null byte,
           return checkpoint
        */

        else{
            pcHaystack=checkpoint+1;
            pcNeedle=initialpcNeedle;
        }
        /* if matching fails, increment the checkpoint,
           initialize the pcNeedle
        */
    }

    return NULL; // if pcNeedle is not found, return null
}
/*------------------------------------------------------------------------*/
char *StrConcat(char *pcDest, const char* pcSrc)
{
    assert(pcDest); /* NULL address, 0, and FALSE are identical. */
    assert(pcSrc); /* NULL address, 0, and FALSE are identical. */

    char *checkpoint=pcDest; // checkpoint for the starting point of pcDest

    while(*pcDest!='\0'){
        pcDest++;
    }
    // move the pointer to the end of string

    StrCopy(pcDest, pcSrc);
    // copy pcSrc and paste it at the end of pcDest

    return checkpoint; // return the point where string starts
}
