// name: 최진성
// # of assignment: 1
// name of the file: wc209

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


enum DFAState {comment, slash_word_out, slash_word_in,
        word_out, word_in, commentend_ready};
// comment: inside comment /*...*/
// slash_word_out: "/" after space character
// slash_word_in: "/" after non-space character
// word_out: space character
// word_in: non-space character
// commentend_ready: receiving "*" inside comment

long long count_lines(char c, long long a) {
    // counting lines whenever "\n" appears
    // input: char c(from getchar()), current number of lines
    // return: number of lines
    // uses global variable c, nLines, affects global variable nLines

    if (c==10) {a++;} // ASCII code for '\n' is 10
    return a;
}

int main(void) {
    // counting lines, words, characters from input file

    int c=0; // char read by getchar() from input file
    long long error_line=0;
    // indicator for line that error occur if comment is unterminated
    long long nLines=1, nWords=0, nChar=0;
    // variable that counts # of lines, words, characters
    enum DFAState state=word_out;

    while ((c=getchar()) != EOF){

        switch (state){
            case comment:
                if (c==42) {state=commentend_ready;}
                // ASCII code for '*' is 42

                long long nLines1=0;
                // local variable to count "\n" in comment
                long long nLines2=0;
                // local variable to count "\n" in comment
                nLines1=nLines;
                nLines2=count_lines(c, nLines);
                nChar=nChar+nLines2-nLines1;
                // adding number of lines in comment to nChar
                nLines=count_lines(c, nLines);
                // adding number of lines in comment to nLines

                break;

            case commentend_ready:
                if (c==47) {state=word_out;}
                // ASCII code for '/' is 47

                else if (c==42) {state=commentend_ready;}
                // ASCII code for '*' is 42

                else if (c==10) {
                    //  ASCII code for '\n' is 10
                    state=comment;
                    nChar++;
                }

                else {state=comment;}

                nLines=count_lines(c, nLines);

                break;

            case word_out:
                if (c==47) {
                    // ASCII code for '/' is 47
                    state=slash_word_out;
                    nChar++;
                    nWords++;
                }

                else if (isspace(c)) {
                    state=word_out;
                    nChar++;
                }

                else {
                    state=word_in;
                    nChar++;
                    nWords++;
                }

                nLines=count_lines(c, nLines);

                break;

            case word_in:
                if (c==47) {
                    // ASCII code for '/' is 47
                    state=slash_word_in;
                    nChar++;
                }

                else if (!isspace(c)) {
                    state=word_in;
                    nChar++;
                }

                else {
                    state=word_out;
                    nChar++;
                }

                nLines=count_lines(c, nLines);

                break;

            case slash_word_in:
                if (c==42) {
                    // ASCII code for '*' is 42
                    state=comment;
                    error_line=nLines;
                }

                else if (c==47) {
                    // ASCII code for '/' is 47
                    state=slash_word_in;
                    nChar++;
                }

                else if (isspace(c)) {
                    state=word_out;
                    nChar++;
                }

                else {
                    state=word_in;
                    nChar++;
                }

                nLines=count_lines(c, nLines);

                break;

            case slash_word_out:
                if (c==42) {
                    // ASCII code for '*' is 42
                    state=comment;
                    nWords--;
                    error_line=nLines;
                }

                else if (c==47) {
                    // ASCII code for '/' is 47
                    state=slash_word_out;
                    nChar++;
                }

                else if (isspace(c)) {
                    state=word_out;
                    nChar++;
                }

                else {
                    state = word_in;
                    nChar++;
                }

                nLines=count_lines(c, nLines);

                break;
        }
    }

    if (state==comment){
        // if state is still in comment,
        // write error message to standard error stream
        // return EXIT_FAILURE when unsuccessful
        fprintf(stderr,
                "Error: line %lld: unterminated comment\n", error_line);
        exit(EXIT_FAILURE);
    }

    else if (nChar==0) {
        // if input file has no character at all, produce 0 0 0
        printf("%d %d %d\n", 0, 0, 0);
    }

    else {
        // print the result to output stream
        printf("%lld %lld %lld\n", nLines, nWords, nChar);
    }
    return 0;
}
