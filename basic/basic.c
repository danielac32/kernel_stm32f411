#include <xinu.h>
#include <fat_filelib.h>

#define FS_MAXSIZE	8192
#define STACKSIZE	32

char *p;
int var[26];
int sp, err, size;
char *stack[STACKSIZE];
char buff[FS_MAXSIZE+1];
char state=0;
#define FOR_NEST 25
struct for_stack {
  int var; /* counter variable */
  int target;  /* target value */
  char *loc;
} fstack[FOR_NEST]; /* stack for FOR/NEXT loop */
int ftos;  /* index to top of FOR stack */

 

int parseExpression();
void parseStatment();
void skipSpaces();


int isDigit(char c){
	if(c>='0' && c<='9') return 1;
	else return 0;
}

int isAlpha(char c){
	if(c>='A' && c<='Z') return 1;
	if(c>='a' && c<='z') return 1;
	return 0;
}

void error(char *msg){
	int ln=0;
	char *t=buff;
	if(!err){
		err=1;
		while(t!=p && *t){
			if(*t=='\n') ++ln;
			++t;
		}
		printf("\nERROR: %s: line %d.\n", msg, ln+1);
	}
}

void skipLabel(){
	char *t=p;
	skipSpaces();
	while(isAlpha(*p) || isDigit(*p)) ++p;
	if(*p==':'){
		++p;
	}else{
		p=t;
	}
}

void skipSpaces(){
	while(*p==' ' || *p=='\t') ++p;
}

int parseNumber(){
	int i=0;
	char b[16];
	while(*p>='0' && *p<='9'){
		b[i++]=*p++;
	}
	b[i]=0;
	return atoi(b);
}

int parseFactor(){
	int a;
	if(*p>='0' && *p<='9'){
		a=parseNumber();
	}else if(*p>='A' && *p<='Z'){
		a=var[(int)(*p-'A')];
		++p;
	}else if(*p=='('){
		++p;
		a=parseExpression();
		if(*p!=')') error("Missing bracket");
		else ++p;
	}else{
		error("Syntax error");
	}
	return a;
}

int parseTerm(){
	int a, b;
	a=parseFactor();
	while(1){
		if(*p=='*'){
			p++;
			b=parseFactor();
			a*=b;
		}else if(*p=='/'){
			p++;
			b=parseFactor();
			if(b!=0) a/=b;
			else error("Divide by zero");
		}else if(*p=='&'){
			p++;
			b=parseFactor();
			a=a&&b;
		}else if(*p=='|'){
			p++;
			b=parseFactor();
			a=a||b;
		}else if(*p=='%'){
			p++;
			b=parseFactor();
			a=(!a && b) || (a && !b);
		}else if(*p=='='){
			p++;
			b=parseFactor();
			a=(a==b);
		}else if(*p=='>'){
			p++;
			b=parseFactor();
			a=(a>b);
		}else if(*p=='<'){
			p++;
			b=parseFactor();
			a=(a<b);
		}else if(*p=='!'){
			p++;
			b=parseFactor();
			a=(a!=b);
		}else{
			return a;
		}
	}
}

int parseExpression(){
	int a, b;
	skipSpaces();
	if(*p=='-' || *p=='+') a=0;
	else a=parseTerm();

	while(1){
    	if(*p=='-'){
      		p++;
			b=parseTerm();
			a-=b;
		}else if(*p=='+'){
			p++;
			b=parseTerm();
			a+=b;
		}else{
			return a;
		}
		skipSpaces();
	}
}

void parseCR(){
	skipSpaces();
	if(*p==0) error("Try to execute out of memory");
	else if(*p=='\n') ++p;
	else error("Syntax error");
}

void parseString(){
	if(*p=='"'){
		++p;
		while(*p!='"' && *p!='\n' && *p!='\0'){
			printf("%c", *p++);
		}
		if(*p!='"') error("Syntax error");
		else ++p;
	}
}

void parsePrint(){
	while(1){
		skipSpaces();
		if(*p=='"') parseString();
		else printf("%d", parseExpression());
		skipSpaces();
		if(*p==',') ++p;
		else break;
	}
	if(*p==';') ++p;
	else printf("\n");
}

void parseEnd(){
	printf("\n");
	if(*p!='\n' && *p!=0) error("Syntax error");
	else err=1;
}

void parseInput(){
	int n;
	if(*p>='A' && *p<='Z'){
		//scanf("%d", &n);
		var[(int)(*p-'A')]=n;
		++p;
	}else{
		error("Syntax error");
	}
}

void parseLet(){
	int v;
	if(*p>='A' && *p<='Z'){
		v=(int)(*p-'A');
		++p;
		if(*p!='=') error("Syntax error let");
		++p;
		 
		var[v]=parseExpression();
	}else{
		error("Syntax error");
	}	
}

void parseIf(){
		state=0;
	if(!parseExpression()){
		while(*p!='\n' && *p!='\0') ++p;
		skipSpaces();
		state=1;
		return;
	}
	skipSpaces();		
	if(strncmp(p, "then ", 5)!=0){
		printf("%s\n",p );
		error("Syntax error if");
		return;
	}
	p+=5;
	skipSpaces();
	parseStatment();
}

void parseElse(){
   if(state==1){
      state=0;
	  skipSpaces();
	  parseStatment();
   }else{
	  return;
   }
}

void parseGoto(){
	int ln, i=0;
	char *t=p;
	char label[32];
	while(isAlpha(*p) || isDigit(*p)){
		label[i++]=*p++;
	}
	label[i++]=':';
	label[i]=0;
	skipSpaces();
	if(*p!=0 && *p!='\n'){
		error("Syntax error goto");
		return;
	}

	p=buff;
	ln=0;
	while(1){
		skipSpaces();
		if(strncmp(p, label, i)==0){
			skipLabel();
			skipSpaces();
			parseStatment();
			return;
		}
		while(*p!='\n' && *p!='\0') ++p;
		if(*p==0){
			p=t;
			error("Label not found");
			break;
		}else{
			++ln;
			++p;
		}
	}
}
/***************************************************/
void fpush(struct for_stack i){
   if(ftos>FOR_NEST){
   	  error("error ftos>FOR_NEST");
   }

  fstack[ftos]=i;
  ftos++;
}
struct for_stack fpop(){
  ftos--;
  if(ftos<0) error("error ftos<0");
  return(fstack[ftos]);
}

void parseFor(){
struct for_stack i;
int value;
	if(*p>='A' && *p<='Z'){
		i.var=(int)(*p-'A');
		++p;
		if(*p!='=') error("Syntax error for (=)");
		++p;
		var[i.var]=parseExpression();
		value=var[i.var];

	}else{
		error("Syntax error for");
	}	
	skipSpaces();

    if(strncmp(p, "to ", 2)!=0){
		printf("%s\n",p );
		error("Syntax error for(to)");
		printf("%s\n",p );
		return;
	}
	p+=2;
	skipSpaces();
    i.target=parseExpression();
    if(value>=var[i.var]) {
       i.loc = p;
       fpush(i);
    }
    skipSpaces();
	parseStatment();
}

void parseNext(){
  struct for_stack i;

  i = fpop(); /* read the loop info */

  var[i.var]++; /* increment control variable */
  if(var[i.var]>i.target) return;  /* all done */
  fpush(i);  /* otherwise, restore the info */
  p = i.loc;  /* loop */
  skipSpaces();
  parseStatment();
}
/*******************************************************/
void parseGosub(){
	char *t=p;
	while(*t!='\n' && *t!=0) ++t;
	stack[sp++]=++t;
	if(sp>=STACKSIZE){
		error("Stack overflow");
		return;
	}
	parseGoto();
}

void parseRet(){
	if(*p!='\n' && *p!=0){
		error("Syntax error");
		return;
	}
	--sp;
	if(sp<0){
		error("RET without GOSUB");
		return;
	}
	p=stack[sp];
	skipLabel();
	parseStatment();
}


void parseList(){
   while(1){
		skipSpaces();
		if(*p=='"'){
			++p;
			char s[100];
			int count=0;

			while(*p!='"' && *p!='\n' && *p!='\0'){
				   s[count++]=*p++;
			}
			s[count]='\0';
			printf("%s\n",s );
         {
         	 /*char buff[256];
			    strcpy(buff, s);
			    scan_files(buff);*/
         }
			if(*p!='"') error("Syntax error");
			else ++p;

		}else printf("%d", parseExpression());
		skipSpaces();
		if(*p==',') ++p;
		else break;
	}
	if(*p==';') ++p;
	else printf("\n");
}


void parseStatment(){
	skipSpaces();
	if(strncmp(p, "rem ", 4)==0){
		p+=4;
		while(*p!='\n' && *p!='\0') ++p;
	}else if(strncmp(p, "print ", 6)==0){
		p+=6;
		skipSpaces();
		parsePrint();
	}else if(strncmp(p, "scan_files", 10)==0){
		p+=10;
		skipSpaces();
		parseList();
	}else if(strncmp(p, "else ", 4)==0){
		p+=4;
		skipSpaces();
		parseElse();
	}else if(strncmp(p, "for ", 3)==0){
		p+=3;
		skipSpaces();
		parseFor();
	}else if(strncmp(p, "next ", 4)==0){
		p+=4;
		skipSpaces();
		parseNext();
	}else if(strncmp(p, "input ", 6)==0){
		p+=6;
		skipSpaces();
		parseInput();
	}else if(strncmp(p, "goto ", 5)==0){
		p+=5;
		skipSpaces();
		parseGoto();
	}else if(strncmp(p, "gosub ", 6)==0){
		p+=6;
		skipSpaces();
		parseGosub();
	}else if(strncmp(p, "if ", 3)==0){
		p+=3;
		skipSpaces();
		parseIf();
	}else if(strncmp(p, "let ", 4)==0){
		p+=4;
		skipSpaces();
		parseLet();
	}else if(strncmp(p, "ret", 3)==0){
		p+=3;
		skipSpaces();
		parseRet();
	}else if(strncmp(p, "end", 3)==0){
		p+=3;
		skipSpaces();
		parseEnd();
	}
}


int basic(int argc, char **argv){
	FILE *f;
	if(argc!=2){
		printf("WARNING: Illegal arguments. Usage: tb [file]\n");
		return 1;
	}

	char *tmp=full_path(argv[1]);
    if (tmp==NULL)return 1;

	if(!(f=fopen(tmp, "rb"))){
		printf("WARNING: File not exist.\n");
		return 2;
	}
   
   fseek(f, 0, SEEK_END);
   unsigned int fileLength = ftell(f);
   fseek(f, 0, SEEK_SET);

	size=fread(buff, fileLength, 1, f);
	fclose(f);
	buff[size]=0;
	p=buff;
	err=0;
	sp=0;

	while(!err){
		skipLabel();
		parseStatment();
		parseCR();
	}
	return 0;
}

/*	GRAMMAR
line := (label:)? statment? CR
statment := PRINT (str|expr) (, (str|expr))* ;?
			IF expr THEN statment
			GOTO label
			INPUT var
			LET var=expr
			GOSUB label
			REM ...
			RET
			END

expr := (+|-|e) term ((+|-) term)*
term := factor ((*|/) factor)*
factor := var|num|(expr)
str := "..."
label := ...
*/
