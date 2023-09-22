// name: 최진성
// # of assignment: 2
// name of the file: sgrep.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for skeleton code */
#include <unistd.h> /* for getopt */
#include "str.h"
#include <assert.h> /* to use assert() */

#define FIND_STR        "-f"
#define REPLACE_STR     "-r"
#define DIFF_STR        "-d"

#define MAX_STR_LEN 1023

#define FALSE 0
#define TRUE  1

typedef enum {
    INVALID,
    FIND,
    REPLACE,
    DIFF
} CommandType;

/*
 * Fill out your functions here (If you need)
 */

/* copies the first len number of characters */
char *StrnCopy(char *pcDest, const char* pcSrc, size_t len)
{
    assert(pcSrc); /* NULL address, 0, and FALSE are identical. */

    char *checkpoint=pcDest; // checkpoint for the starting point of pcDest
    int i=1;

    for(;i<=len;i++){ // perform until len number of characters
        *pcDest=*pcSrc; // copy each character of pcSrc to pcDest
        pcDest++;
        pcSrc++;
    }

    *pcDest='\0'; //add null byte at the end of pcDest

    return checkpoint; // return starting point of pcDest
}

/*--------------------------------------------------------------------*/
/* PrintUsage()
   print out the usage of the Simple Grep Program                     */
/*--------------------------------------------------------------------*/
void
PrintUsage(const char* argv0)
{
    const static char *fmt =
            "Simple Grep (sgrep) Usage:\n"
            "%s [COMMAND] [OPTIONS]...\n"
            "\nCOMMNAD\n"
            "\tFind: -f [search-string]\n"
            "\tReplace: -r [string1] [string2]\n"
            "\tDiff: -d [file1] [file2]\n";

    printf(fmt, argv0);
}
/*-------------------------------------------------------------------*/
/* DoFind()
   Your task:
   1. Do argument validation
   - String or file argument length is no more than 1023
   - If you encounter a command-line argument that's too long,
   print out "Error: argument is too long"

   2. Read the each line from standard input (stdin)
   - If you encounter a line larger than 1023 bytes,
   print out "Error: input line is too long"
   - Error message should be printed out to standard error (stderr)

   3. Check & print out the line contains a given string (search-string)

   Tips:
   - fgets() is an useful function to read characters from file. Note
   that the fget() reads until newline or the end-of-file is reached.
   - fprintf(sderr, ...) should be useful for printing out error
   message to standard error

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
DoFind(const char *pcSearch)
{
    char buf[MAX_STR_LEN + 2];
    int len;

    /* Read the line by line from stdin, Note that this is an example */
    while (fgets(buf, sizeof(buf), stdin)) {
        /* check input line length */
        if ((len = StrGetLength(buf)) > MAX_STR_LEN) {
            fprintf(stderr, "Error: input line is too long\n");
            return FALSE;
        }

        if (StrSearch(buf, pcSearch)!=0){
            // if the line finds pcSearch string, print the line
            printf("%s", buf);
        }

    }

    printf("\n");
    return TRUE;
}
/*-------------------------------------------------------------------*/
/* DoReplace()
   Your task:
   1. Do argument validation
      - String length is no more than 1023
      - If you encounter a command-line argument that's too long,
        print out "Error: argument is too long"
      - If word1 is an empty string,
        print out "Error: Can't replace an empty substring"

   2. Read the each line from standard input (stdin)
      - If you encounter a line larger than 1023 bytes,
        print out "Error: input line is too long"
      - Error message should be printed out to standard error (stderr)

   3. Replace the string and print out the replaced string

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
DoReplace(const char *pcString1, const char *pcString2)
{
    int string1len=StrGetLength(pcString1); // length of pcString1
    int string2len=StrGetLength(pcString2); // length of pcString2
    int numofstring1; // number of pcString1 found in each line
    char *pointfound; // point where pcString1 is found
    char *pointcont; // point in line to continue searching
    char *temp; // temporal point pointing the string of

    char buf[MAX_STR_LEN + 2];
    int linelen; // length of each line

    if(*pcString1=='\0'){
        // if pcString is empty string, stop the program
        fprintf(stderr, "Can't replace an empty substring\n");
        return FALSE;
    }

    if(string1len>MAX_STR_LEN){
        // checking string1len
        fprintf(stderr, "Error: argument is too long\n");
        return FALSE;
    }

    if(string2len>MAX_STR_LEN){
        // checking string2len
        fprintf(stderr, "Error: argument is too long\n");
        return FALSE;
    }

    /* Read the line by line from stdin */
    while (fgets(buf, sizeof(buf), stdin)) {
        char replacedstring[MAX_STR_LEN+1]; // newly created string after replacing

        /* check input line length */
        if ((linelen = StrGetLength(buf)) > MAX_STR_LEN) {
            fprintf(stderr, "Error: input line is too long\n");
            return FALSE;
        }

        if (StrSearch(buf, pcString1) == NULL) {
            // if there is no pcString1 in the line, print the original line
            printf("%s", buf);
        }

        else {
            numofstring1=0;
            pointcont=buf;

            /* calculating number of pcString1 found in the line */
            while((pointfound=StrSearch(pointcont, pcString1))!=NULL){
                numofstring1++;
                pointcont=pointfound+string1len;
            }

            temp=replacedstring;
            pointcont=buf; // initializing pointcont to the starting point

            while(numofstring1>0){
                /* copy the original string until the pcString1 */
                pointfound = StrSearch(pointcont, pcString1);
                size_t interval=pointfound-pointcont;
                temp=StrnCopy(temp, pointcont, interval);

                /* checking the length of new string */
                if (StrGetLength(replacedstring) +
                string2len > MAX_STR_LEN) {
                    fprintf(stderr, "Error: output line is too long\n");
                    return FALSE;
                }

                else{
                    temp = StrConcat(temp, pcString2);
                    temp+=interval+string2len;
                    pointcont+=interval+string1len;
                    // copy the pcString2
                }

                numofstring1--; // reducing the reamining number of pcString1

            }

            /* checking the length of new string */
            if(StrGetLength(replacedstring)+
            StrGetLength(pointcont)>MAX_STR_LEN){
                fprintf(stderr, "Error: output line is too long\n");
                return FALSE;
            }

            else{
                /* copy the remaining component of the original string */
                StrConcat(temp, pointcont);
                printf("%s", replacedstring); // print the new string

            }

        }

    }

    printf("\n");
    return TRUE;
}
/*-------------------------------------------------------------------*/
/* DoDiff()
   Your task:
   1. Do argument validation
     - file name length is no more than 1023
     - If a command-line argument is too long,
       print out "Error: arugment is too long" to stderr

   2. Open the two files
      - The name of files are given by two parameters
      - If you fail to open either file, print out error messsage
      - Error message: "Error: failed to open file [filename]\n"
      - Error message should be printed out to stderr

   3. Read the each line from each file
      - If you encounter a line larger than 1023 bytes,
        print out "Error: input line [filename] is too long"
      - Error message should be printed out to stderr

   4. Compare the two files (file1, file2) line by line

   5. Print out any different line with the following format
      file1@linenumber:file1's line
      file2@linenumber:file2's line

   6. If one of the files ends earlier than the other, print out an
      error message "Error: [filename] ends early at line XX", where
      XX is the final line number of [filename].

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/
int
DoDiff(const char *file1, const char *file2)
{
    /* check filename length */
    if(StrGetLength(file1)>MAX_STR_LEN){
        fprintf(stderr, "Error: argument is too long\n");
        return FALSE;
    }

    /* check filename length */
    if(StrGetLength(file2)>MAX_STR_LEN){
        fprintf(stderr, "Error: argument is too long\n");
        return FALSE;
    }

    FILE *fpfile1; // file pointer for file1
    FILE *fpfile2; // file pointer for file2
    fpfile1=fopen(file1, "r"); // open file1
    fpfile2=fopen(file2, "r"); // open file2

    /* check if file successfully opened */
    if(fpfile1==NULL){
        fprintf(stderr, "failed to open file %s\n", file1);
        return FALSE;
    }

    /* check if file successfully opened */
    if(fpfile2==NULL){
        fprintf(stderr, "failed to open file %s\n", file2);
        return FALSE;
    }

    char buf1[MAX_STR_LEN + 2];
    char buf2[MAX_STR_LEN + 2];

    int linenum=0; //indicator for line number
    int linelen1; // length of each line for file1 text
    int linelen2; // length of each line for file2 text
    int check; // return value of StrCompare

    /* Read the line by line from file1 and file2 */
    while(fgets(buf1, sizeof(buf1), fpfile1) &&
            fgets(buf2, sizeof(buf2), fpfile2)){

        linenum++;

        /* check file1 line length */
        if ((linelen1 = StrGetLength(buf1)) > MAX_STR_LEN) {
            fprintf(stderr, "Error: input line is too long\n");
            return FALSE;
        }

        /* check file2 line length */
        if ((linelen2 = StrGetLength(buf2)) > MAX_STR_LEN) {
            fprintf(stderr, "Error: input line is too long\n");
            return FALSE;
        }

        check=StrCompare(buf1, buf2); // compare each line of file1 and file2

        /* if there is any difference, print the lines */
        if (check!=0){
            printf("%s@%d:%s", file1, linenum, buf1);
            printf("%s@%d:%s", file2, linenum, buf2);
        }

        /* if file 1 reaches the end of file first, print out an error message */
        if(feof(fpfile1) && !feof(fpfile2)){
            fflush(stdout); // to print out all the stdout before printing an error
            fprintf(stderr, "\nError: %s ends early at line %d\n", file1, linenum);
            return FALSE;
        }

        /* if file 1 reaches the end of file first, print out an error message */
        if(feof(fpfile2) && !feof(fpfile1)){
            fflush(stdout); // to print out all the stdout before printing an error
            fprintf(stderr, "\nError: %s ends early at line %d\n", file2, linenum);
            return FALSE;
        }

    }


    printf("\n");
    return TRUE;
}
/*-------------------------------------------------------------------*/
/* CommandCheck()
   - Parse the command and check number of argument.
   - It returns the command type number
   - This function only checks number of argument.
   - If the unknown function is given or the number of argument is
   different from required number, this function returns FALSE.

   Note: You SHOULD check the argument rule later                    */
/*-------------------------------------------------------------------*/
int
CommandCheck(const int argc, const char *argv1)
{
    int cmdtype = INVALID;

    /* check minimum number of argument */
    if (argc < 3)
        return cmdtype;

    /* check command type */
    if (strcmp(argv1, FIND_STR) == 0) {
        if (argc != 3)
            return FALSE;
        cmdtype = FIND;
    }
    else if (strcmp(argv1, REPLACE_STR) == 0) {
        if (argc != 4)
            return FALSE;
        cmdtype = REPLACE;
    }
    else if (strcmp(argv1, DIFF_STR) == 0) {
        if (argc != 4)
            return FALSE;
        cmdtype = DIFF;
    }

    return cmdtype;
}
/*-------------------------------------------------------------------*/
int
main(const int argc, const char *argv[])
{
    int type, ret;

    /* Do argument check and parsing */
    if (!(type = CommandCheck(argc, argv[1]))) {
        fprintf(stderr, "Error: argument parsing error\n");
        PrintUsage(argv[0]);
        return (EXIT_FAILURE);
    }

    /* Do appropriate job */
    switch (type) {
        case FIND:
            ret = DoFind(argv[2]);
            break;
        case REPLACE:
            ret = DoReplace(argv[2], argv[3]);
            break;
        case DIFF:
            ret = DoDiff(argv[2], argv[3]);
            break;
    }

    return (ret)? EXIT_SUCCESS : EXIT_FAILURE;
}
