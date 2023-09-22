#include "dynarray.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>

/*--------------------------------------------------------------------*/

enum {MAX_LINE_SIZE = 1024};

enum {FALSE, TRUE};

enum TokenType {TOKEN_NORMAL, TOKEN_SPECIAL};

int specialindicator_array[MAX_LINE_SIZE+1];

int pipeindicator_array[MAX_LINE_SIZE+1];

int redirectindicator_array[MAX_LINE_SIZE+1];

int commandindicaor_array[MAX_LINE_SIZE+1];

int special_count=0;

int total_count=0;

int pipe_count=0;

int redirect_count=0;

int command_num=1;

/*--------------------------------------------------------------------*/

struct Token
{
    enum TokenType eType;

    char *pcValue;
};

/*--------------------------------------------------------------------*/

struct argv
{
    int arv_num;

    char **argv_value;
};

/*--------------------------------------------------------------------*/

static void tonotcoredumped()

{
    exit(1);
}

/*--------------------------------------------------------------------*/

static void sigquithandler()

{
    fprintf(stdout, "\nType Ctrl-\\ again within 5 seconds to exit.\n");
    alarm(5);
    signal(SIGQUIT,tonotcoredumped);
}

/*--------------------------------------------------------------------*/

void sigalrmhandler()

{
    signal(SIGQUIT,sigquithandler);
}

/*--------------------------------------------------------------------*/

static void freeToken(void *pvItem, void *pvExtra)

{
    struct Token *psToken = (struct Token*)pvItem;
    free(psToken->pcValue);
    free(psToken);
}

/*--------------------------------------------------------------------*/

static struct Token *makeToken(enum TokenType eTokenType,
                               char *pcValue)

{
    struct Token *psToken;

    psToken = (struct Token*)malloc(sizeof(struct Token));
    if (psToken == NULL)
        return NULL;

    psToken->eType = eTokenType;

    psToken->pcValue = (char*)malloc(strlen(pcValue) + 1);
    if (psToken->pcValue == NULL)
    {
        free(psToken);
        return NULL;
    }

    strcpy(psToken->pcValue, pcValue);

    return psToken;
}

/*--------------------------------------------------------------------*/

static int *makeargument_array(DynArray_T oTokens, const int *commandlocation, int command_num)

{
    int *argumentlength_array;

    argumentlength_array = (int*)malloc(sizeof(int)*command_num);
    if (argumentlength_array == NULL)
        return NULL;

    int argumentlength;

    struct Token *psToken;

    char *str_pipe = "|";


    for (int i=0;i<command_num;i++)
    {
        if(commandlocation[i] < total_count){
            for (int j=commandlocation[i]; j<total_count; j++)
            {
                psToken=DynArray_get(oTokens, j);
                if ((strcmp(psToken->pcValue, str_pipe) == 0) && (psToken->eType == TOKEN_SPECIAL)){
                    argumentlength = j-commandlocation[i];
                    argumentlength_array[i]=argumentlength;
                    break;
                }

                else {
                    argumentlength = total_count-commandlocation[i];
                    argumentlength_array[i]=argumentlength;
                }
            }
        }

        else if (commandlocation[i] == total_count){
            argumentlength = 0;
            argumentlength_array[i] = argumentlength;
        }

    }

    return argumentlength_array;

}


/*--------------------------------------------------------------------*/

static struct argv *makeargv_array(DynArray_T oTokens, const int *commandlocation,
                                   const int *argumentlength, int numcommand)

{
    struct Token *psToken;
    char *argument;

    struct argv *argumentvalue;
    argumentvalue = (struct argv*)malloc(sizeof(struct argv)*numcommand);
    if (argumentvalue == NULL)
        return NULL;

    for (int i=0; i<numcommand; i++)
    {
        argumentvalue[i].argv_value = (char **)malloc(sizeof(char*)*(argumentlength[i]+2));
        if (argumentvalue[i].argv_value == NULL){
            free(argumentvalue);
            return NULL;
        }
        if (commandlocation[i] < total_count){
            for (int j=commandlocation[i]-1; j<commandlocation[i]+argumentlength[i]; j++)
            {
                psToken=DynArray_get(oTokens, j);
                argument = (char*)malloc(sizeof(char)*MAX_LINE_SIZE);
                if (argument == NULL)
                    return NULL;
                strcpy(argument, psToken->pcValue);
                argumentvalue[i].arv_num = i+1;
                argumentvalue[i].argv_value[j-commandlocation[i]+1] = argument;
            }
        }

        if (commandlocation[i] == commandlocation[i]+argumentlength[i]){
            psToken=DynArray_get(oTokens, commandlocation[i]-1);
            argument = (char*)malloc(sizeof(char)*MAX_LINE_SIZE);
            if (argument == NULL)
                return NULL;
            strcpy(argument, psToken->pcValue);
            argumentvalue[i].arv_num = i+1;
            argumentvalue[i].argv_value[0] = argument;
        }

        argumentvalue[i].argv_value[argumentlength[i]+1] = NULL;

    }

    return argumentvalue;

}



/*--------------------------------------------------------------------*/

static int lexLine(const char *pcLine, DynArray_T oTokens)

{
    enum LexState {STATE_START, STATE_IN_STRING, STATE_IN_PIPE, STATE_IN_REDIRECT, STATE_IN_QUOTES};

    enum LexState eState = STATE_START;

    int iLineIndex = 0;
    int iValueIndex = 0;
    char c;
    char acValue[MAX_LINE_SIZE];
    struct Token *psToken;

    commandindicaor_array[0]=1;

    assert(pcLine != NULL);
    assert(oTokens != NULL);

    for (;;)
    {
        c = pcLine[iLineIndex++];

        switch (eState)
        {
            case STATE_START:
                if ((c == '\n') || (c == '\0'))
                    return TRUE;
                else if (isspace(c) || (c == '\t'))
                {
                    eState = STATE_START;
                }
                else if (c == '|')
                {
                    acValue[iValueIndex++] = c;
                    eState = STATE_IN_PIPE;
                }
                else if ((c == '<') || (c == '>'))
                {
                    acValue[iValueIndex++] = c;
                    eState = STATE_IN_REDIRECT;
                }
                else if (c == '"')
                {
                    eState = STATE_IN_QUOTES;
                }
                else
                {
                    acValue[iValueIndex++] = c;
                    eState= STATE_IN_STRING;
                }
                break;

            case STATE_IN_PIPE:
                if ((c == '\n') || (c == '\0'))
                {
                    acValue[iValueIndex] = '\0';
                    psToken = makeToken(TOKEN_SPECIAL, acValue);
                    if (psToken == NULL)
                    {
                        return FALSE;
                    }
                    if (! DynArray_add(oTokens, psToken))
                    {
                        return FALSE;
                    }
                    iValueIndex = 0;
                    total_count++;
                    special_count++;
                    pipe_count++;
                    command_num++;
                    specialindicator_array[special_count]=total_count;
                    pipeindicator_array[pipe_count]=total_count;
                    commandindicaor_array[pipe_count]=total_count+1;

                    return TRUE;
                }
                else if ((c == '|') || (c == '<') || (c == '>'))
                {
                    return FALSE;
                }
                else if (isspace(c) || (c == '\t'))
                {
                    acValue[iValueIndex] = '\0';
                    psToken = makeToken(TOKEN_SPECIAL, acValue);
                    if (psToken == NULL)
                    {
                        return FALSE;
                    }
                    if (! DynArray_add(oTokens, psToken))
                    {
                        return FALSE;
                    }
                    iValueIndex = 0;
                    total_count++;
                    special_count++;
                    pipe_count++;
                    command_num++;
                    specialindicator_array[special_count]=total_count;
                    pipeindicator_array[pipe_count]=total_count;
                    commandindicaor_array[pipe_count]=total_count+1;

                    eState = STATE_START;
                }
                else if (c=='"')
                {
                    acValue[iValueIndex] = '\0';
                    psToken = makeToken(TOKEN_SPECIAL, acValue);
                    if (psToken == NULL)
                    {
                        return FALSE;
                    }
                    if (! DynArray_add(oTokens, psToken))
                    {
                        return FALSE;
                    }
                    iValueIndex = 0;
                    total_count++;
                    special_count++;
                    pipe_count++;
                    command_num++;
                    specialindicator_array[special_count]=total_count;
                    pipeindicator_array[pipe_count]=total_count;
                    commandindicaor_array[pipe_count]=total_count+1;

                    eState = STATE_IN_QUOTES;
                }
                else
                {
                    acValue[iValueIndex] = '\0';
                    psToken = makeToken(TOKEN_SPECIAL, acValue);
                    if (psToken == NULL)
                    {
                        return FALSE;
                    }
                    if (! DynArray_add(oTokens, psToken))
                    {
                        return FALSE;
                    }
                    iValueIndex = 0;
                    total_count++;
                    special_count++;
                    pipe_count++;
                    command_num++;
                    specialindicator_array[special_count]=total_count;
                    pipeindicator_array[pipe_count]=total_count;
                    commandindicaor_array[pipe_count]=total_count+1;
                    acValue[iValueIndex++] = c;

                    eState = STATE_IN_STRING;
                }
                break;

            case STATE_IN_REDIRECT:
                if ((c == '\n') || (c == '\0'))
                {
                    acValue[iValueIndex] = '\0';
                    psToken = makeToken(TOKEN_SPECIAL, acValue);
                    if (psToken == NULL)
                    {
                        return FALSE;
                    }
                    if (! DynArray_add(oTokens, psToken))
                    {
                        return FALSE;
                    }
                    iValueIndex = 0;
                    total_count++;
                    special_count++;
                    redirect_count++;
                    specialindicator_array[special_count]=total_count;
                    redirectindicator_array[redirect_count]=total_count;

                    return TRUE;
                }
                else if ((c == '|') || (c == '<') || (c == '>'))
                {
                    return FALSE;
                }
                else if (isspace(c) || (c == '\t'))
                {
                    acValue[iValueIndex] = '\0';
                    psToken = makeToken(TOKEN_SPECIAL, acValue);
                    if (psToken == NULL)
                    {
                        return FALSE;
                    }
                    if (! DynArray_add(oTokens, psToken))
                    {
                        return FALSE;
                    }
                    iValueIndex = 0;
                    total_count++;
                    special_count++;
                    redirect_count++;
                    specialindicator_array[special_count]=total_count;
                    redirectindicator_array[redirect_count]=total_count;

                    eState = STATE_START;
                }
                else if (c=='"')
                {
                    acValue[iValueIndex] = '\0';
                    psToken = makeToken(TOKEN_SPECIAL, acValue);
                    if (psToken == NULL)
                    {
                        return FALSE;
                    }
                    if (! DynArray_add(oTokens, psToken))
                    {
                        return FALSE;
                    }
                    iValueIndex = 0;
                    total_count++;
                    special_count++;
                    redirect_count++;
                    specialindicator_array[special_count]=total_count;
                    redirectindicator_array[redirect_count]=total_count;

                    eState = STATE_IN_QUOTES;
                }
                else
                {
                    acValue[iValueIndex] = '\0';
                    psToken = makeToken(TOKEN_SPECIAL, acValue);
                    if (psToken == NULL)
                    {
                        return FALSE;
                    }
                    if (! DynArray_add(oTokens, psToken))
                    {
                        return FALSE;
                    }
                    iValueIndex = 0;
                    total_count++;
                    special_count++;
                    redirect_count++;
                    specialindicator_array[special_count]=total_count;
                    redirectindicator_array[redirect_count]=total_count;
                    acValue[iValueIndex++] = c;

                    eState = STATE_IN_STRING;
                }
                break;

            case STATE_IN_STRING:
                if ((c == '\n') || (c == '\0'))
                {
                    acValue[iValueIndex] = '\0';
                    psToken = makeToken(TOKEN_NORMAL, acValue);
                    if (psToken == NULL)
                    {
                        return FALSE;
                    }
                    if (! DynArray_add(oTokens, psToken))
                    {
                        return FALSE;
                    }
                    iValueIndex = 0;
                    total_count++;

                    return TRUE;
                }
                else if (c == '|')
                {
                    acValue[iValueIndex] = '\0';
                    psToken = makeToken(TOKEN_NORMAL, acValue);
                    if (psToken == NULL)
                    {
                        return FALSE;
                    }
                    if (! DynArray_add(oTokens, psToken))
                    {
                        return FALSE;
                    }
                    iValueIndex = 0;
                    total_count++;
                    acValue[iValueIndex++] = c;

                    eState = STATE_IN_PIPE;
                }
                else if ((c == '<') || (c == '>'))
                {
                    acValue[iValueIndex] = '\0';
                    psToken = makeToken(TOKEN_NORMAL, acValue);
                    if (psToken == NULL)
                    {
                        return FALSE;
                    }
                    if (! DynArray_add(oTokens, psToken))
                    {
                        return FALSE;
                    }
                    iValueIndex = 0;
                    total_count++;
                    acValue[iValueIndex++] = c;

                    eState = STATE_IN_REDIRECT;
                }
                else if (isspace(c) || (c == '\t'))
                {
                    acValue[iValueIndex] = '\0';
                    psToken = makeToken(TOKEN_NORMAL, acValue);
                    if (psToken == NULL)
                    {
                        return FALSE;
                    }
                    if (! DynArray_add(oTokens, psToken))
                    {
                        return FALSE;
                    }
                    iValueIndex = 0;
                    total_count++;

                    eState = STATE_START;
                }
                else if (c == '"')
                {
                    eState = STATE_IN_QUOTES;
                }
                else
                {
                    acValue[iValueIndex++]=c;
                    eState = STATE_IN_STRING;
                }
                break;

            case STATE_IN_QUOTES:
                if ((c == '\n') || (c == '\0'))
                    return FALSE;
                else if (c == '"')
                {
                    eState = STATE_IN_STRING;
                }
                else
                {
                    acValue[iValueIndex++]=c;
                    eState = STATE_IN_QUOTES;
                }
                break;
        }
    }

}

/*--------------------------------------------------------------------*/


static int synLine(DynArray_T oTokens, const int *locatespecial, int count)

{
    enum SynType {STATE_START, TOKEN_BAR, TOKEN_TOLEFT, TOKEN_TORIGHT};

    enum SynType eState = STATE_START;
    struct Token *psToken;
    char *strbar = "|";
    char *strtoleft = "<";
    char *strtoright = ">";
    int save_locate;
    int oToken_length;

    oToken_length = DynArray_getLength(oTokens);
    if (oToken_length == 0){
        return TRUE;
    }

    if (locatespecial[1] == 1){
        return FALSE;
    }

    if (locatespecial[count] == total_count){
        return FALSE;
    }

    for (int i=1;i<count+1;i++)
    {
        psToken = DynArray_get(oTokens, locatespecial[i]-1);
        switch (eState)
        {
            case STATE_START:
                if (strcmp(psToken->pcValue, strbar) == 0){
                    eState = TOKEN_BAR;
                    save_locate=locatespecial[i];
                }

                else if (strcmp(psToken->pcValue, strtoleft) == 0){
                    eState = TOKEN_TOLEFT;
                    save_locate=locatespecial[i];
                }

                else if (strcmp(psToken->pcValue, strtoright) == 0){
                    eState = TOKEN_TORIGHT;
                    save_locate=locatespecial[i];
                }

                else {
                    return FALSE;
                }
                break;

            case TOKEN_BAR:
                if (locatespecial[i]-save_locate == 1){
                    return FALSE;
                }

                else if (strcmp(psToken->pcValue, strtoleft) == 0){
                    return FALSE;
                }

                else if (strcmp(psToken->pcValue, strbar) == 0){
                    eState = TOKEN_BAR;
                    save_locate=locatespecial[i];
                }
                else if (strcmp(psToken->pcValue, strtoright) == 0){
                    eState = TOKEN_TORIGHT;
                    save_locate=locatespecial[i];
                }
                else {
                    return FALSE;
                }
                break;

            case TOKEN_TOLEFT:
                if (locatespecial[i]-save_locate == 1){
                    return FALSE;
                }

                else if (strcmp(psToken->pcValue, strtoleft) == 0){
                    return FALSE;
                }

                else if (strcmp(psToken->pcValue, strbar) == 0){
                    eState = TOKEN_BAR;
                    save_locate=locatespecial[i];
                }
                else if (strcmp(psToken->pcValue, strtoright) == 0){
                    eState = TOKEN_TORIGHT;
                    save_locate=locatespecial[i];
                }
                else {
                    return FALSE;
                }
                break;

            case TOKEN_TORIGHT:
                if (locatespecial[i]-save_locate == 1){
                    return FALSE;
                }

                else if (strcmp(psToken->pcValue, strbar) == 0){
                    return FALSE;
                }

                else if (strcmp(psToken->pcValue, strtoright) == 0){
                    return FALSE;
                }

                else if (strcmp(psToken->pcValue, strtoleft) == 0){
                    eState = TOKEN_TOLEFT;
                    save_locate=locatespecial[i];
                }
                else {
                    return FALSE;
                }
                break;
        }
    }


    return TRUE;

}

/*--------------------------------------------------------------------*/

static int built_in_execute(DynArray_T oTokens, int *locatecommand, int commandcount)

{
    struct Token *psToken;
    struct Token *psarg1;
    struct Token *psarg2;
    char *str_cd = "cd";
    char *str_setenv = "setenv";
    char *str_unsetenv = "unsetenv";
    char *str_exit = "exit";
    char *HOME_dir = getenv("HOME");
    int *lengthargument;
    int x;

    lengthargument = makeargument_array(oTokens, locatecommand, commandcount);

    for (int i=0; i<commandcount; i++)
    {
        psToken = DynArray_get(oTokens, locatecommand[i]-1);

        if(lengthargument[i] == 1)
        {
            psarg1 = DynArray_get(oTokens, locatecommand[i]);
        }

        if(lengthargument[i] == 2)
        {
            psarg1 = DynArray_get(oTokens, locatecommand[i]);
            psarg2 = DynArray_get(oTokens, locatecommand[i]+1);
        }

        if (strcmp(psToken->pcValue, str_cd) == 0){
            if (lengthargument[i] > 1){
                return FALSE;
            }
            else if(lengthargument[i] == 0){
                x=chdir(HOME_dir);
                if(x==-1)
                    return FALSE;
            }
            else{
                x=chdir(psarg1->pcValue);
                if(x==-1)
                    return FALSE;
            }
        }

        else if (strcmp(psToken->pcValue, str_setenv) == 0){
            if (lengthargument[i] > 2 || lengthargument[i] == 0){
                return FALSE;
            }
            else if(lengthargument[i] == 1){
                x=setenv(psarg1->pcValue, "", 0);
                if(x==-1)
                    return FALSE;
            }
            else {
                x=setenv(psarg1->pcValue,psarg2->pcValue,0);
                if(x==-1)
                    return FALSE;
            }
        }

        else if (strcmp(psToken->pcValue, str_unsetenv) == 0){
            if (lengthargument[i] > 1 || lengthargument[i] == 0){
                return FALSE;
            }
            else {
                x=unsetenv(psarg1->pcValue);
                if(x==-1)
                    return FALSE;
            }
        }

        else if (strcmp(psToken->pcValue, str_exit) == 0){
            if (lengthargument[i] != 0){
                return FALSE;
            }
            else {
                exit(1);
            }
        }
        else {
            return FALSE;
        }

    }

    return TRUE;

}

/*--------------------------------------------------------------------*/

static int other_execute(DynArray_T oTokens, int *locatecommand, int commandcount)

{
    int pid,pipe_descriptor[2],save_pipe_descriptor=0,i=0;
    struct Token *psToken;
    struct argv *psarg;
    int *lengthargument;

    lengthargument = makeargument_array(oTokens, locatecommand, commandcount);
    psarg = makeargv_array(oTokens, locatecommand, lengthargument, commandcount);

    if (commandcount==1)
    {
        psToken = DynArray_get(oTokens, locatecommand[0]-1);
        if ((pid=fork()) == -1) {
            exit(-1);
        }
        else if (pid == 0) {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            execvp(psToken->pcValue, psarg[0].argv_value);
            return FALSE;
        }
        else {
            wait(NULL);
        }
    }

    else {
        for (i=0; i<commandcount; i++){
            pipe(pipe_descriptor);

            if ((pid=fork()) == -1) {
                exit(1);
            }
            else if (pid == 0) {

                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);

                dup2(save_pipe_descriptor,0);

                if (i+1<commandcount) {
                    dup2(pipe_descriptor[1],1);
                }

                close(pipe_descriptor[0]);

                psToken = DynArray_get(oTokens, locatecommand[i]-1);
                execvp(psToken->pcValue, psarg[i].argv_value);
                exit(1);
            }
            else {
                wait(NULL);
                close(pipe_descriptor[1]);
                save_pipe_descriptor = pipe_descriptor[0];
            }
        }
    }

    return TRUE;
}

/*--------------------------------------------------------------------*/

int main(void)

{
    char acLine[MAX_LINE_SIZE];
    char fileLine[MAX_LINE_SIZE];
    DynArray_T oTokens;
    struct Token *psToken;
    int iSuccessful1;
    int iSuccessful2;
    int iSuccessful3;
    int oToken_length;
    char *str_cd = "cd";
    char *str_setenv = "setenv";
    char *str_unsetenv = "unsetenv";
    char *str_exit = "exit";
    char *HOME_dir = getenv("HOME");
    char *filename = "/.ishrc";
    FILE *pFile = NULL;
    sigset_t signal_set;
    void (*sigint)(int);
    void (*sigquit)(int);
    void (*sigalrm)(int);

    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGALRM);
    sigprocmask(SIG_UNBLOCK, &signal_set, NULL);

    char *target_file = strcat(HOME_dir, filename);

    pFile = fopen(target_file, "r");
    if (pFile != NULL){
        while(fgets(fileLine, MAX_LINE_SIZE, pFile) != '\0'){
            printf("%% %s", fileLine);
            sigint = signal(SIGINT, SIG_IGN);
            sigquit = signal(SIGQUIT, sigquithandler);
            sigalrm = signal(SIGALRM, sigalrmhandler);
            assert(sigint != SIG_ERR);
            assert(sigquit != SIG_ERR);
            assert(sigalrm != SIG_ERR);
            oTokens = DynArray_new(0);
            if (oTokens == NULL)
            {
                fprintf(stderr, "ourish: Cannot allocate memory.\n");
                exit(EXIT_FAILURE);
            }

            iSuccessful1 = lexLine(fileLine, oTokens);
            if(iSuccessful1)
            {
                iSuccessful2 = synLine(oTokens, specialindicator_array, special_count);
                if (iSuccessful2){

                    oToken_length = DynArray_getLength(oTokens);
                    if (oToken_length == 0){
                        continue;
                    }

                    else{
                        psToken = DynArray_get(oTokens, commandindicaor_array[0]-1);
                    }

                    if ((strcmp(psToken->pcValue,str_cd) == 0) || (strcmp(psToken->pcValue,str_setenv) == 0) || (strcmp(psToken->pcValue,str_unsetenv) == 0) || (strcmp(psToken->pcValue,str_exit) == 0))
                        iSuccessful3 = built_in_execute(oTokens, commandindicaor_array, command_num);

                    else
                        iSuccessful3 = other_execute(oTokens, commandindicaor_array, command_num);

                    if(!iSuccessful3)
                        fprintf(stderr, "ourish: execution failed.\n");
                }
                else
                    fprintf(stderr, "ourish: Syntactic analysis failed.\n");
            }
            else
                fprintf(stderr, "ourish: lexical analysis failed.\n");

            for(int i=1; i<special_count+1; i++)
                specialindicator_array[i]=0;

            for(int i=1; i<pipe_count+1; i++)
                pipeindicator_array[i]=0;

            for(int i=1; i<redirect_count+1; i++)
                redirectindicator_array[i]=0;

            for(int i=0; i<command_num; i++)
                commandindicaor_array[i]=0;

            total_count=0;
            special_count=0;
            pipe_count=0;
            redirect_count=0;
            command_num=1;

            DynArray_map(oTokens, freeToken, NULL);
            DynArray_free(oTokens);


        }
    }

    while (TRUE)
    {
        printf("%% ");
        sigint = signal(SIGINT, SIG_IGN);
        sigquit = signal(SIGQUIT, sigquithandler);
        sigalrm = signal(SIGALRM, sigalrmhandler);
        assert(sigint != SIG_ERR);
        assert(sigquit != SIG_ERR);
        assert(sigalrm != SIG_ERR);
        fgets(acLine, MAX_LINE_SIZE, stdin);
        oTokens = DynArray_new(0);
        if (oTokens == NULL)
        {
            fprintf(stderr, "ourish: Cannot allocate memory.\n");
            exit(EXIT_FAILURE);
        }

        iSuccessful1 = lexLine(acLine, oTokens);
        if(iSuccessful1)
        {
            iSuccessful2 = synLine(oTokens, specialindicator_array, special_count);
            if (iSuccessful2){

                oToken_length = DynArray_getLength(oTokens);
                if (oToken_length == 0){
                    continue;
                }

                else{
                    psToken = DynArray_get(oTokens, commandindicaor_array[0]-1);
                }

                if ((strcmp(psToken->pcValue,str_cd) == 0) || (strcmp(psToken->pcValue,str_setenv) == 0) ||
                    (strcmp(psToken->pcValue,str_unsetenv) == 0) || (strcmp(psToken->pcValue,str_exit) == 0))
                    iSuccessful3 = built_in_execute(oTokens, commandindicaor_array, command_num);

                else
                    iSuccessful3 = other_execute(oTokens, commandindicaor_array, command_num);

                if(!iSuccessful3)
                    fprintf(stderr, "ourish: execution failed.\n");
            }
            else
                fprintf(stderr, "ourish: Syntactic analysis failed.\n");
        }
        else
            fprintf(stderr, "ourish: lexical analysis failed.\n");

        for(int i=1; i<special_count+1; i++)
            specialindicator_array[i]=0;

        for(int i=1; i<pipe_count+1; i++)
            pipeindicator_array[i]=0;

        for(int i=1; i<redirect_count+1; i++)
            redirectindicator_array[i]=0;

        for(int i=0; i<command_num; i++)
            commandindicaor_array[i]=0;

        total_count=0;
        special_count=0;
        pipe_count=0;
        redirect_count=0;
        command_num=1;

        DynArray_map(oTokens, freeToken, NULL);
        DynArray_free(oTokens);
    }

    return 0;
}



