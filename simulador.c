
%{
#include <stdlib.h>	
#include <string.h>

char *ptr;
long val;
int tokens[5], i = 0, pc = 0;

%}

%%
"add" {tokens[i] = 0; i++;}
"addi" {tokens[i] = 1; i++;}
"sub" {tokens[i] = 2; i++;}
"subi" {tokens[i] = 3; i++;}
"blt" {tokens[i] = 4; i++;}
"bgt" {tokens[i] = 5; i++;}
"beq" {tokens[i] = 6; i++;}
"lw" {tokens[i] = 7; i++;}
"sw" {tokens[i] = 8; i++;}
"mov" {tokens[i] = 9; i++;}
"r0" {tokens[i] = 10; i++;}
"r1" {tokens[i] = 11; i++;}
"r2" {tokens[i] = 12; i++;}
"r3" {tokens[i] = 13; i++;}
"r4" {tokens[i] = 14; i++;}
"r5" {tokens[i] = 15; i++;}
"r6" {tokens[i] = 16; i++;}
"r7" {tokens[i] = 17; i++;}
"r8" {tokens[i] = 18; i++;}
[^r][0-9]+ { val = strtol(yytext, &ptr, 10); tokens[i] = val; i++;}
. {}
%%


void add(int regist[], int memo[]){
	regist[tokens[1]%10] = regist[tokens[2]%10] + regist[tokens[3]%10];
}

void addi(int regist[], int memo[]){
 	regist[tokens[1]%10] = regist[tokens[2]%10] + regist[tokens[3]];
}

void sub(int regist[], int memo[]){
	regist[tokens[1]%10] = regist[tokens[2]%10] - regist[tokens[3]%10];
}

void subi(int regist[], int memo[]){
	regist[tokens[1]%10] = regist[tokens[2]%10] - regist[tokens[3]];
}

void blt(int regist[], int memo[]){
	if(regist[tokens[1]%10] > regist[tokens[2]%10])
		pc = tokens[3];
}

void bgt(int regist[], int memo[]){
	if(regist[tokens[1]%10] < regist[tokens[2]%10])
		pc = tokens[3];
}

void beq(int regist[], int memo[]){
	if(regist[tokens[1]%10] == regist[tokens[2]%10])
		pc = tokens[3];
}

void lw(int regist[], int memo[]){
	int aux = tokens[2] + regist[tokens[3]%10];
	regist[tokens[1]%10] = memo[aux];
}

void sw(int regist[], int memo[]){
	int aux = tokens[2] + regist[tokens[3]%10];
	memo[aux] = regist[tokens[1]%10];
}

void mov(int regist[], int memo[]){
	regist[tokens[1]%10] = tokens[2];
}

void printRegist(int regist[]){
	printf("\n==============================\n");
	printf("\nRegistradores de uso geral\n");
	for(i=0; i<9; i++){
		printf("r%d = %d\t", i, regist[i]);
		if(i == 2 || i == 5 || i== 8)
			printf("\n");
	}
}

void printMemory(int memo[]){
	printf("\n=================================");
	printf("=======================================");
	printf("=======================================");
	printf("=====================\n");
	printf("\nMemoria Principal\n");
	for(i=0; i<100; i++){
		printf("M[%d] = %d\t", i, memo[i]);
		if((i%9) == 0 && i != 0)
			printf("\n");
	}
}

void printGenReg(char ir[1][255]){
	printf("\n=====================================\n");
	printf("\nRegistradores de controle de estado\n");
	printf("PC --> %d\n", pc);
	printf("IR --> %s\n", ir[0]);
}

int main(void){
	void (*f[])(int *, int *) = {add, addi, sub, subi, blt, bgt, beq, lw, sw, mov}; 
	char instr[100][255], ir[1][255]; //instruções
	int regist[9], memo[100], j = 0;
	regist[0] = 0;

	FILE *fp;
	YY_BUFFER_STATE bp;
	fp = fopen("teste.txt", "r");
	while(fgets(instr[j], 255, fp) != NULL)
		j++;
	fclose(fp);

	while(pc < j){
		strcpy(ir[0], instr[pc]);
		bp = yy_scan_string(ir[0]);
		yy_switch_to_buffer(bp);
		yylex();
		yy_flush_buffer(bp);
		pc++;
		f[tokens[0]](regist, memo);
    	i = 0;
    	printMemory(memo);
    	printRegist(regist);
    	printGenReg(ir);
	}
    yy_delete_buffer(bp);
	return 0;
}