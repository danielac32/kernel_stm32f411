/* shell.c  -  shell */

#include <xinu.h>
#include <string.h>
#include "shprototypes.h"

buf_t path, curdir;


void update_path(){
	strcpy(path, curdir);
}

char* full_path(const char* name) {
    if (name == NULL)
        return NULL;
    if (name[0] == '/') {
        strcpy(path, name);
        return path;
    }
    if (strncmp(name, "./", 2) == 0)
        name += 2;
    strcpy(path, curdir);
    if (strncmp(name, "../", 3) != 0) {
        if (name[0])
            strcat(path, name);
    } else {
        name += 3; // root doen't have a parent
        char* cp = strrchr(path, '/');
        if (cp != NULL)
            *cp = 0;
        cp = strrchr(path, '/');
        if (cp != NULL)
            *(cp + 1) = 0;
        strcat(path, name);
    }
    return path;
}


/************************************************************************/
/* Table of Xinu shell commands and the function associated with each	*/
/************************************************************************/
const	struct	cmdent	cmdtab[] = {
	{"reboot",	FALSE,	xsh_reboot},
	{"devdump",	FALSE,	xsh_devdump},
	{"echo",	TRUE,	xsh_echo},
	{"exit",	TRUE,	xsh_exit},
	{"help",	FALSE,	xsh_help},
	{"kill",	TRUE,	xsh_kill},
	{"memdump",	FALSE,	xsh_memdump},
	{"memstat",	TRUE,	xsh_memstat}, /* Make built-in */
	{"ps",		FALSE,	xsh_ps},
	{"?",		FALSE,	xsh_help},
	{"blink",   FALSE,  xsh_blink},
	{"cd",      TRUE,   xsh_cd},
	{"ls",      TRUE,   xsh_ls},
	{"cat",     FALSE,  xsh_cat},
	{"dump",    FALSE,  xsh_dump},
	{"mkdir",   TRUE,   xsh_mkdir},
	{"pwd",     TRUE,   xsh_pwd},
	{"rm",      TRUE,   xsh_rm},
	{"touch",   TRUE,   xsh_touch},
	{"run",     FALSE,  xsh_run},
    {"format",  FALSE,  xsh_format},
	{"test",    FALSE,  xsh_test},
	{"loadkernel",    FALSE,  xsh_loadkernel},
	{"cpu",    FALSE,  xsh_cpu},
};

uint32	ncmd = sizeof(cmdtab) / sizeof(struct cmdent);

/************************************************************************/
/* shell  -  Provide an interactive user interface that executes	*/
/*	     commands.  Each command begins with a command name, has	*/
/*	     a set of optional arguments, has optional input or		*/
/*	     output redirection, and an optional specification for	*/
/*	     background execution (ampersand).  The syntax is:		*/
/*									*/
/*		   command_name [args*] [redirection] [&]		*/
/*									*/
/*	     Redirection is either or both of:				*/
/*									*/
/*				< input_file				*/
/*			or						*/
/*				> output_file				*/
/*									*/
/************************************************************************/

process	shell (
		int nargs, char *arg[]///did32	dev		/* ID of tty device from which	*/
	)				/*   to accept commands		*/
{
	char	buf[SHELL_BUFLEN];	/* Input line (large enough for	*/
	int32	len;			/* Length of line read		*/
	char	tokbuf[SHELL_BUFLEN +	/* Buffer to hold a set of	*/
			SHELL_MAXTOK];  /* Contiguous null-terminated	*/
	int32	tlen;			/* Current length of all data	*/
					/*   in array tokbuf		*/
	int32	tok[SHELL_MAXTOK];	/* Index of each token in	*/
	int32	toktyp[SHELL_MAXTOK];	/* Type of each token in tokbuf	*/
	int32	ntok;			/* Number of tokens on line	*/
	pid32	child;			/* Process ID of spawned child	*/
	bool8	backgnd;		/* Run command in background?	*/
	char	*outname, *inname;	/* Pointers to strings for file	*/
					/*   names that follow > and <	*/
	did32	stdinput, stdoutput;	/* Descriptors for redirected	*/
					/*   input and output		*/
	int32	i;			/* Index into array of tokens	*/
	int32	j;			/* Index into array of commands	*/
	int32	msg;			/* Message from receive() for	*/
					/*   child termination		*/
	int32	tmparg;			/* Address of this var is used	*/
					/*   when first creating child	*/
					/*   process, but is replaced	*/
	char	*src, *cmp;		/* Pointers used during name	*/
					/*   comparison			*/
	bool8	diff;			/* Was difference found during	*/
	char	*args[SHELL_MAXTOK];	/* Argument vector passed to	*/
					/*   builtin commands		*/
    
    

    //queue_init(&ready_queue);
    //n=0;
    //strcpy(path,"/");
    
   //strcpy(cwd,"/");
   strcpy(path, "/");
    strcpy(curdir, path);
    
did32	dev=CONSOLE;
	/* Print shell banner and startup message */

	fprintf(dev, "\n\n%s%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		SHELL_BAN0,SHELL_BAN1,SHELL_BAN2,SHELL_BAN3,SHELL_BAN4,
		SHELL_BAN5,SHELL_BAN6,SHELL_BAN7,SHELL_BAN8,SHELL_BAN9,SHELL_BAN10);

	fprintf(dev, "%s\n\n", SHELL_STRTMSG);

	/* Continually prompt the user, read input, and execute command	*/
  
  
	//struct dentry	*devptr;	
	//struct	ttycblk	*typtr;	
	struct	procent	*prptr;
	while (TRUE) {

		/* Display prompt */
		fprintf(dev, SHELL_PROMPT);

         
       
		len = read(dev, buf, sizeof(buf));


		/* Exit gracefully on end-of-file */

		if (len == EOF) {
			break;
		}

		/* If line contains only NEWLINE, go to next line */

		if (len <= 1) {
			continue;
		}

		buf[len] = SH_NEWLINE;	/* terminate line */
		/* Parse input line and divide into tokens */
		ntok = lexan(buf, len, tokbuf, &tlen, tok, toktyp);

		/* Handle parsing error */

		if (ntok == SYSERR) {
			fprintf(dev,"%s\n", SHELL_SYNERRMSG);
			continue;
		}

		/* If line is empty, go to next input line */

		if (ntok == 0) {
			fprintf(dev, "\n");
			continue;
		}

		/* If last token is '&', set background */

		if (toktyp[ntok-1] == SH_TOK_AMPER) {
			ntok-- ;
			tlen-= 2;
			backgnd = TRUE;
		} else {
			backgnd = FALSE;
		}


		 

		/* Verify remaining tokens are type "other" */

		for (i=0; i<ntok; i++) {
			if (toktyp[i] != SH_TOK_OTHER) {
				break;
			}
		}
		if ((ntok == 0) || (i < ntok)) {
			fprintf(dev, SHELL_SYNERRMSG);
			continue;
		}

		stdinput = stdoutput = dev;

		/* Lookup first token in the command table */

		for (j = 0; j < ncmd; j++) {
			src = cmdtab[j].cname;
			cmp = tokbuf;
			diff = FALSE;
			while (*src != NULLCH) {
				if (*cmp != *src) {
					diff = TRUE;
					break;
				}
				src++;
				cmp++;
			}
			if (diff || (*cmp != NULLCH)) {
				continue;
			} else {
				break;
			}
		}

		/* Handle command not found */

		if (j >= ncmd) {
			fprintf(dev, "command %s not found\n", tokbuf);
			continue;
		}

		/* Handle built-in command */
 
		if (cmdtab[j].cbuiltin) { /* No background or redirect. */
			if (backgnd){
				fprintf(dev, SHELL_BGERRMSG);
				continue;
			} else {
				/* Set up arg vector for call */

				for (i=0; i<ntok; i++) {
					args[i] = &tokbuf[tok[i]];
				}

				/* Call builtin shell function */

				if ((*cmdtab[j].cfunc)(ntok, args)
							== SHELL_EXIT) {
					break;
				}
			}
			continue;
		}
 
		 
		/* Spawn child thread for non-built-in commands */

		child = create(cmdtab[j].cfunc,
			SHELL_CMDSTK, SHELL_CMDPRIO,
			cmdtab[j].cname, 2, ntok, &tmparg);

		/* If creation or argument copy fails, report error */

		if ((child == SYSERR) ||
		    (addargs(child, ntok, tok, tlen, tokbuf, &tmparg)
							== SYSERR) ) {
			fprintf(dev, SHELL_CREATMSG);
			continue;
		}
		/* Set stdinput and stdoutput in child to redirect I/O */

		proctab[child].prdesc[0] = stdinput;
		proctab[child].prdesc[1] = stdoutput;

		msg = recvclr();
		resume(child);
		
		if (! backgnd) {
			msg = receive();
			while (msg != child) {
				msg = receive();
			}
		}
    }

    /* Terminate the shell process by returning from the top level */

    fprintf(dev,SHELL_EXITMSG);
    return OK;
}

