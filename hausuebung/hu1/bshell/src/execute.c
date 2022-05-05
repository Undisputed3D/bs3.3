#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"
#include "helper.h"
#include "command.h"
#include <signal.h>
#include <errno.h>
#include <pwd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "statuslist.h"
#include "debug.h"
#include "execute.h"

/* do not modify this */
#ifndef NOLIBREADLINE
#include <readline/history.h>
#endif /* NOLIBREADLINE */

extern int shell_pid;
extern int fdtty;

/* do not modify this */
#ifndef NOLIBREADLINE
static int builtin_hist(char ** command){

    register HIST_ENTRY **the_list;
    register int i;
    printf("--- History --- \n");

    the_list = history_list ();
    if (the_list)
        for (i = 0; the_list[i]; i++)
            printf ("%d: %s\n", i + history_base, the_list[i]->line);
    else {
        printf("history could not be found!\n");
    }

    printf("--------------- \n");
    return 0;
}
#endif /*NOLIBREADLINE*/
void unquote(char * s){
	if (s!=NULL){
		if (s[0] == '"' && s[strlen(s)-1] == '"'){
	        char * buffer = calloc(sizeof(char), strlen(s) + 1);
			strcpy(buffer, s);
			strncpy(s, buffer+1, strlen(buffer)-2);
                        s[strlen(s)-2]='\0';
			free(buffer);
		}
	}
}

void unquote_command_tokens(char ** tokens){
    int i=0;
    while(tokens[i] != NULL) {
        unquote(tokens[i]);
        i++;
    }
}

void unquote_redirect_filenames(List *redirections){
    List *lst = redirections;
    while (lst != NULL) {
        Redirection *redirection = (Redirection *)lst->head;
        if (redirection->r_type == R_FILE) {
            unquote(redirection->u.r_file);
        }
        lst = lst->tail;
    }
}

void unquote_command(Command *cmd){
    List *lst = NULL;
    switch (cmd->command_type) {
        case C_SIMPLE:
        case C_OR:
        case C_AND:
        case C_PIPE:
        case C_SEQUENCE:
            lst = cmd->command_sequence->command_list;
            while (lst != NULL) {
                SimpleCommand *cmd_s = (SimpleCommand *)lst->head;
                unquote_command_tokens(cmd_s->command_tokens);
                unquote_redirect_filenames(cmd_s->redirections);
                lst = lst->tail;
            }
            break;
        case C_EMPTY:
        default:
            break;
    }
}

static int execute_fork(SimpleCommand *cmd_s, int background){
    char ** command = cmd_s->command_tokens;
    pid_t pid, wpid;
    pid = fork();
    if (pid==0){
        /* child */
        signal(SIGINT, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        /*
         * handle redirections here
         */
        if (cmd_s->redirections != NULL){
            printf("Handling of redirection is still missing ... implement it!\n");
            exit(0);
        }
        if (execvp(command[0], command) == -1){
            fprintf(stderr, "-bshell: %s : command not found \n", command[0]);
            perror("");
        }
        /*exec only return on error*/
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("shell");

    } else {
        /*parent*/
        setpgid(pid, pid);
        if (background == 0) {
            /* wait only if no background process */
            tcsetpgrp(fdtty, pid);

            /**
             * the handling here is far more complicated than this!
             * vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
             */

            wpid= waitpid(pid, NULL, 0);

            //^^^^^^^^^^^^^^^^^^^^^^^^^^^^

            tcsetpgrp(fdtty, shell_pid);
            return 0;
        }
    }

    return 0;
}


static int do_execute_simple(SimpleCommand *cmd_s, int background){
    if (cmd_s==NULL){
        return 0;
    }
    if (strcmp(cmd_s->command_tokens[0],"exit")==0){
        exit(0);

/* do not modify this */
#ifndef NOLIBREADLINE
    } else if (strcmp(cmd_s->command_tokens[0],"hist")==0){
        return builtin_hist(cmd_s->command_tokens);
#endif /* NOLIBREADLINE */
    } else {
        return execute_fork(cmd_s, background);
    }
    fprintf(stderr, "This should never happen!\n");
    exit(1);
}

/*
 * check if the command is to be executed in back- or foreground.
 *
 * For sequences, the '&' sign of the last command in the
 * sequence is checked.
 *
 * returns:
 *      0 in case of foreground execution
 *      1 in case of background execution
 *
 */
int check_background_execution(Command * cmd){
    List * lst = NULL;
    int background =0;
    switch(cmd->command_type){
    case C_SIMPLE:
        lst = cmd->command_sequence->command_list;
        background = ((SimpleCommand*) lst->head)->background;
        break;
    case C_OR:
    case C_AND:
    case C_PIPE:
    case C_SEQUENCE:
        /*
         * last command in sequence defines whether background or
         * forground execution is specified.
         */
        lst = cmd->command_sequence->command_list;
        while (lst !=NULL){
            background = ((SimpleCommand*) lst->head)->background;
            lst=lst->tail;
        }
        break;
    case C_EMPTY:
    default:
        break;
    }
    return background;
}


int execute(Command * cmd){
    unquote_command(cmd);

    int res=0;
    List * lst=NULL;

    int execute_in_background=check_background_execution(cmd);
    switch(cmd->command_type){
    case C_EMPTY:
        break;
    case C_SIMPLE:
        res=do_execute_simple((SimpleCommand*) cmd->command_sequence->command_list->head, execute_in_background);
        fflush(stderr);
        break;

    case C_OR:
    case C_AND:
    case C_SEQUENCE:
        printf("[%s] AND, OR and SEQUENCES  are not yet implemented ... do it ... \n", __func__);
        /** Iteration through the command list!
         * lst = cmd->command_sequence->command_list;
         * while (lst !=NULL){
         *
         *    ^^
         *    ||
         *    || code is missing here
         *    ||
         *    vv
         *    
         *    lst=lst->tail;
         *
         * }
         */
        break;
    case C_PIPE:
        printf("[%s] PIPES are not yet implemented ... do it ... \n", __func__);
        break;
    default:
        printf("[%s] unhandled command type [%i]\n", __func__, cmd->command_type);
        break;
    }
    return res;
}

