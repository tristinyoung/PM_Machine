//Tristin Young
//COP 3402 Systems Software
//Professor Montagne
// Sep 20 2020

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//defining max stack height / max code length as constants
#define MAX_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500

//struct for instructions
typedef struct instruction
{
    int op;
    int r;
    int l;
    int m;
} instruction;

//declaration of base function
int base(int l, int base);

//creating ptr to the input file
FILE *inputPtr;
//creating max size for stack and text
instruction text[MAX_CODE_LENGTH];
int stack[MAX_STACK_HEIGHT];
int RF[8];
const char instructionName[22][4] = {"LIT", "RTN", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SYS", "NEG", "ADD", "SUB", "MUL",
                                     "DIV", "ODD", "MOD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ"};

int main(int argc, char *argv[])
{
    int untouched = 0;
    //creating sp, bp, pc, and ir
    int sp = MAX_STACK_HEIGHT;
    int bp = sp - 1;
    int pc = 0;
    int ir = 0;

    //setting all register file values to zero
    for (int i = 0; i < 8; i++)
    {
        RF[i] = 0;
    }

    //setting all stack values to zero
    for (int i = 0; i < MAX_STACK_HEIGHT; i++)
    {
        stack[i] = 0;
    }

    //setting all text values to zero
    for (int i = 0; i < MAX_CODE_LENGTH; i++)
    {
        text[i].op = 0;
        text[i].r = 0;
        text[i].l = 0;
        text[i].m = 0;
    }

    //setting haltFlag to 1
    int haltFlag = 1;

    //creating memory for user to input file name
    char *inputFileName;
    inputFileName = calloc(100, sizeof(int));
    //getting filename from user
    printf("Enter the name of the input file to be used (including '.txt'):\n");
    scanf("%s", inputFileName);
    //giving the input file ptr meaning
    inputPtr = fopen(inputFileName, "r");
    //if the file does not exist
    if (inputPtr == NULL)
    {
        //print error and terminate
        printf("File name is spelled incorrectly or does not exist.\n");
        return 1;
    }

    //transferring each instruction from the input file to the text array
    int read_status;
    int t1, t2, t3, t4;
    read_status = fscanf(inputPtr, "%d %d %d %d", &t1, &t2, &t3, &t4);
    do
    {
        text[pc].op = t1;
        text[pc].r = t2;
        text[pc].l = t3;
        text[pc].m = t4;
        pc++;
        read_status = fscanf(inputPtr, "%d %d %d %d", &t1, &t2, &t3, &t4);
    } while (read_status != EOF);
    //var to quickly know how many instructions we have
    int numOfInstructions = pc;
    //reset pc
    pc = 0;
    printf("\n\nLINE   OP  R  L  M\n\n");
    //print instructions out with opcode meaning
    for (int i = 0; i < numOfInstructions; i++)
    {
        printf("  %d   ", i);
        printf("%s  ", instructionName[(text[i].op - 1)]);
        //switch to tell between opcodes
        printf("%d  ", text[i].r);
        printf("%d  ", text[i].l);
        printf("%d\n", text[i].m);
    }
    //variables to hold contents of text array
    int OP, R, L, M;
    //printing sequence performed after every execution
    printf("                                   pc      bp     sp\n");
    printf("Initial values                     %d      %d     %d\n", pc, bp, sp);
    printf("Registers: ");
    for (int i = 0; i < 8; i++)
    {
        printf("%d ", RF[i]);
    }
    printf("\n");
    printf("Stack: ");
    for (int i = 0; i < 15; i++)
    {
        printf("%d ", stack[i]);
    }
    printf("\n\n");

    printf("                                   pc      bp     sp\n");
    //while haltflag does not equal zero
    while (haltFlag == 1)
    {
        //assign vars their text counterpart
        OP = text[pc].op;
        R = text[pc].r;
        L = text[pc].l;
        M = text[pc].m;

        //swich to handle R L and M based on opcode
        //printing sequence performed after every execution
        
        

        switch (OP)
        {
        case 1:
            RF[R] = M;
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 2:
            sp = bp + 1;
            bp = stack[sp - 2];
            pc = stack[sp - 3];
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 3:
            RF[R] = stack[base(L, bp) - M];
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 4:
            stack[base(L, bp) - M] = RF[R];
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 5:
            stack[sp - 1] = base(L, bp);
            stack[sp - 2] = bp;
            stack[sp - 3] = pc;
            bp = sp - 1;
            pc = M - 1;
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 6:
            sp = sp - M;
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 7:
            pc = M - 1;
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 8:
            if (RF[R] == 0)
            {
                pc = M - 1;
            }
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;

        case 9:
            if (M == 1)
            {
                printf("%d", RF[R]);
            }
            else if (M == 2)
            {
                scanf("%d", &RF[R]);
            }
            else if (M == 3)
            {
                haltFlag = 0;
            }
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;

        case 10:
            RF[R] = -RF[R];
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 11:
            RF[R] = RF[L] + RF[M];
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 12:
            RF[R] = RF[L] - RF[M];
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 13:
            RF[R] = RF[L] * RF[M];
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 14:
            RF[R] = RF[L] / RF[M];
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 15:
            RF[R] = RF[R] % 2;
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 16:
            RF[R] = RF[L] % RF[M];
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 17:
            if (RF[L] == RF[M])
            {
                RF[R] = 1;
            }
            else
                RF[R] = 0;
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 18:
            if (RF[L] != RF[M])
            {
                RF[R] = 1;
            }
            else
                RF[R] = 0;
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 19:
            if (RF[L] < RF[M])
            {
                RF[R] = 1;
            }
            else
                RF[R] = 0;
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 20:
            if (RF[L] <= RF[M])
            {
                RF[R] = 1;
            }
            else
                RF[R] = 0;
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 21:
            if (RF[L] > RF[M])
            {
                RF[R] = 1;
            }
            else
                RF[R] = 0;
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        case 22:
            if (RF[L] >= RF[M])
            {
                RF[R] = 1;
            }
            else
                RF[R] = 0;
            printf("%d %s %d %d %d", pc, instructionName[text[pc].op - 1], text[pc].r, text[pc].l, text[pc].m);
            break;
        }
        printf("                                   %d      %d     %d\n", pc, bp, sp);

        printf("Registers: ");
        for (int i = 0; i < 8; i++)
        {
            printf("%d ", RF[i]);
        }
        printf("\n");
        printf("Stack: ");
        fflush(stdout);
        for (int i = 0; i < 15; i++)
        {
            printf("%d ", stack[i]);
        }
        printf("\n\n");
        //increment pc
        pc++;
    }

    return (0);
}

//base function
int base(l, base)
{
    int b1;
    b1 = base;
    while (l > 0)
    {
        b1 = stack[b1];
        l--;
    }
    return b1;
}