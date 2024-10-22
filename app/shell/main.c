
#include <interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stm32.h>
#include <gpio.h>
#include <syscall.h>
#include <elf.h>
#include <shell.h>
#define FILE            lfs_file_t
#define DIR             lfs_dir_t


 
syscall_t syscallp;



/*------------------------------------------------------------------------
 * lexan  -  Ad hoc lexical analyzer to divide command line into tokens
 *------------------------------------------------------------------------
 */

int32	lexan (
	  char		*line,		/* Input line terminated with	*/
					/*   NEWLINE or NULLCH		*/
	  int32		len,		/* Length of the input line,	*/
					/*   including NEWLINE		*/
	  char		*tokbuf,	/* Buffer into which tokens are	*/
					/*   stored with a null		*/
					/*   following each token	*/
	  int32		*tlen,		/* Place to store number of	*/
					/*   chars in tokbuf		*/
	  int32		tok[],		/* Array of pointers to the	*/
					/*   start of each token	*/
	  int32		toktyp[]	/* Array that gives the type	*/
					/*   of each token		*/
	)
{
	char	quote;			/* Character for quoted string  */
	uint32	ntok;			/* Number of tokens found	*/
	char	*p;			/* Pointer that walks along the	*/
					/*   input line			*/
	int32	tbindex;		/* Index into tokbuf		*/
	char	ch;			/* Next char from input line	*/

	/* Start at the beginning of the line with no tokens */

	ntok = 0;
	p = line;
	tbindex = 0;

	/* While not yet at end of line, get next token */

	while ( (*p != NULLCH) && (*p != SH_NEWLINE) ) {

		/* If too many tokens, return error */

		if (ntok >= SHELL_MAXTOK) {
			return SYSERR;
		}

		/* Skip whitespace before token */

		while ( (*p == SH_BLANK) || (*p == SH_TAB) ) {
			p++;
		}

		/* Stop parsing at end of line (or end of string) */

		ch = *p;
		if ( (ch==SH_NEWLINE) || (ch==NULLCH) ) {
			*tlen = tbindex;
			return ntok;
		}

		/* Set next entry in tok array to be an index to the	*/
		/*   current location in the token buffer		*/

		tok[ntok] = tbindex;	/* the start of the token	*/

		/* Set the token type */

		switch (ch) {

		    case SH_AMPER:	toktyp[ntok] = SH_TOK_AMPER;
					tokbuf[tbindex++] = ch;
					tokbuf[tbindex++] = NULLCH;
					ntok++;
					p++;
					continue;

		    case SH_LESS:	toktyp[ntok] = SH_TOK_LESS;
					tokbuf[tbindex++] = ch;
					tokbuf[tbindex++] = NULLCH;
					ntok++;
					p++;
					continue;

		    /*case SH_GREATER:	toktyp[ntok] = SH_TOK_GREATER;
					tokbuf[tbindex++] = ch;
					tokbuf[tbindex++] = NULLCH;
					ntok++;
					p++;
					continue;*/

		    default:		toktyp[ntok] = SH_TOK_OTHER;
		};

		/* Handle quoted string (single or double quote) */

		if ( (ch==SH_SQUOTE) || (ch==SH_DQUOTE) ) {
			quote = ch;	/* remember opening quote */

			/* Copy quoted string to arg area */

			p++;	/* Move past starting quote */

			while ( ((ch=*p++) != quote) && (ch != SH_NEWLINE)
					&& (ch != NULLCH) ) {
				tokbuf[tbindex++] = ch;
			}
			if (ch != quote) {  /* string missing end quote	*/
				return SYSERR;
			}

			/* Finished string - count token and go on	*/

			tokbuf[tbindex++] = NULLCH; /* terminate token	*/
			ntok++;		/* count string as one token	*/
			continue;	/* go to next token		*/
		}

		/* Handle a token other than a quoted string		*/

		tokbuf[tbindex++] = ch;	/* put first character in buffer*/
		p++;

		while ( ((ch = *p) != SH_NEWLINE) && (ch != NULLCH)
			&& (ch != SH_LESS)  && (ch != SH_GREATER)
			&& (ch != SH_BLANK) && (ch != SH_TAB)
			&& (ch != SH_AMPER) && (ch != SH_SQUOTE)
			&& (ch != SH_DQUOTE) )	{
				tokbuf[tbindex++] = ch;
				p++;
		}

		/* Report error if other token is appended */

		if (       (ch == SH_SQUOTE) || (ch == SH_DQUOTE)
			|| (ch == SH_LESS)   || (ch == SH_GREATER) ) {
			return SYSERR;
		}

		tokbuf[tbindex++] = NULLCH;	/* terminate the token	*/

                ntok++;				/* count valid token	*/

	}
	*tlen = tbindex;
	return ntok;
}

 
 

struct jsonframe
{
    uint32 len;
    uint32 size;
    char *buffer;
    char *val;
    char *find;
};



int load(int argc, char *argv[])
{
  int r;
  char bfifo[16];
  if(argc < 2){
    return -1;
  }
  if(argv[1]==NULL)return -1;

  char *tmp=syscallp.full_path(argv[1]);
  if(syscallp.exist(tmp)){
    printf("%s file found!\n",tmp );
    syscallp.updatepath();//update_path();
    return -1;
  }

//  int size_malloc=atoi(argv[i]);
  uint8 *buff=syscallp.malloc(40000);
  int count=0;

  while(1){
      do{
        if (!syscall(XINU_PIN_GET,GPIOx(GPIO_A),0)){
            goto label_end_load;
        }
        //sleep(0);
      }
      while(syscall(XINU_GET_LEN)<0);
      //
      //char c=syscall(XINU_GETC) & 0xff;
      memcpy(bfifo,(void *)syscall(XINU_GETS),syscall(XINU_GET_LEN));
      char c=bfifo[0];
      printf("%02x",c);
      buff[count]=c;
      count++;
  }
  label_end_load:
  printf("writing..\n");
  //printf("dump\n");
  //hexDump2(0,buff,count);
  FILE *fs=syscallp.open(tmp,LFS_O_CREAT);
  syscallp.write(buff,count,fs);
  syscallp.close(fs);
  syscallp.updatepath();//update_path();
  syscallp.free(buff);

  return 0;
}



int main(int argc, char *argv[])
{
	syscall_init(&syscallp);
    char	buf[SHELL_BUFLEN];	/* Input line (large enough for	*/
	int32	len;			/* Length of line read		*/
	char	tokbuf[SHELL_BUFLEN +	/* Buffer to hold a set of	*/
				SHELL_MAXTOK];  /* Contiguous null-terminated	*/
	int32	tlen;			/* Current length of all data	*/
						/*   in array tokbuf		*/
	int32	tok[SHELL_MAXTOK];	/* Index of each token in	*/
	int32	toktyp[SHELL_MAXTOK];	/* Type of each token in tokbuf	*/
	int32	ntok;			/* Number of tokens on line	*/
	//int	child;			/* Process ID of spawned child	*/
	bool	backgnd;		/* Run command in background?	*/
		
	int32	i;			/* Index into array of tokens	*/
	int32	j;			/* Index into array of commands	*/
		
	//char	*src, *cmp;		/* Pointers used during name	*/
						/*   comparison			*/
	//bool8	diff;			/* Was difference found during	*/
	char	*args[SHELL_MAXTOK];

  exec_img ximg;
	int (*p)(int,char **);
	void *fptr;

  again:
  if(!syscallp.exist("shell/shell.json")){
      printf("error init shell\n");
      return -1;
  }
  FILE *fs=syscallp.open("shell/shell.json",LFS_O_RDONLY);

	int size = syscallp.size(fs);
	char *jsonbuff=syscallp.malloc(size+1);
	char jsonp[100];
	syscallp.read(jsonbuff,size,fs);
	syscallp.close(fs);

  char *val;

  while(1){
  	printf(SHELL_PROMPT);
	do{
           syscallp.sleep(0);
    }
	while(syscall(XINU_GET_LEN)<=1);

	if (syscall(XINU_GET_LEN) <= 1) {
		continue;
	}
    memcpy(buf,(void *)syscall(XINU_GETS),syscall(XINU_GET_LEN));

	buf[syscall(XINU_GET_LEN)] = SH_NEWLINE;
    ntok = lexan(buf, syscall(XINU_GET_LEN), tokbuf, &tlen, tok, toktyp);
    if (ntok == SYSERR) {
		printf("%s\n", SHELL_SYNERRMSG);
		continue;
	}
    if (ntok == 0) {
			printf("\n");
			continue;
		}

		if (toktyp[ntok-1] == SH_TOK_AMPER) {
			  ntok-- ;
			  tlen-= 2;
			  backgnd = TRUE;
		} else {
			  backgnd = FALSE;
		}

		for (i=0; i<ntok; i++) {
			if (toktyp[i] != SH_TOK_OTHER) {
				break;
			}
		}
		if ((ntok == 0) || (i < ntok)) {
			printf(SHELL_SYNERRMSG);
			continue;
		}


		for (i=0; i<ntok; i++) {
			args[i] = &tokbuf[tok[i]];
			printf("%s\n", args[i]);
		}
    printf("\n");

    if(!strncmp(args[0],"load",4)){
         printf("load file ...waiting..\n");
         load(ntok,&args[0]);
         continue;
    }

    memset(jsonp,0,100);
    struct jsonframe json_t;
    struct jsonframe *json_res;
    json_t.len=len;
    json_t.size=size;
    json_t.buffer=jsonbuff;
    json_t.find=args[0];

    //val = syscallp.js0n(args[0],0,jsonbuff,size,&len);
    json_res = (struct jsonframe*)syscallp.js0n(&json_t);
    len = json_res->len;
    val = json_res->val;

	memcpy(jsonp,val,len);
	if(strlen(jsonp)<5)continue;//"".elf
    
    
    //printf("->%s\n",jsonp );
    int ret =syscallp.elf_execve(jsonp,&ximg);
    if(ret == -1){
       printf("command %s not found %s\n", tokbuf,jsonp);
       
			 continue;
		}
      
    if (backgnd == FALSE){
    	  p= (int *)ret;
    	 // uint32_t q = irq_disable();
    	  //printf("->\n");
        int r=p(ntok,&args[0]);
        //printf("exit %d\n",args[0] );
        syscallp.free(ximg.start);	
       // irq_restore(q);
    }else{
    	  //uint32_t q = irq_disable();
    	  //printf(">>\n");
    	  p = (int *)ret;
        int child = syscallp.create((void *)p, 2048, 3, args[0]);
    	  task_t *prptr=syscallp.self(child);	
        prptr->elf = TRUE;
        prptr->img = (void *)ximg.start;
        context_t *ctx = (context_t*)prptr->sp;
        ctx->r0 = (uint32) ntok;
        ctx->r1 = (uint32)&args[0];
	      syscallp.ready(child);
	     // irq_restore(q);
	    continue;
    }

    }
	return 0;
}