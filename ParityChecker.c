#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct{
    int    *chrom;
    double fitness;  
}INDIVIDUAL;

typedef INDIVIDUAL *IPTR;


typedef struct{
    IPTR op; //array of individuals form an evolving population
    IPTR np;
    
    int chromLen; //chromosome length
    int gen; //current generation
    
    double sumFitness;
    double max;
    double avg;
    double min;
    
    double pCross;
    double pMut;
    
    double highestEverFitness;
    int    highestEverGen;
    int    highestEverIndex;
    
    int maxGen;
    int popSize;
    int maxi;
    int mini;
                        
}POPULATION;


unsigned int Gate2InFunction(unsigned int gateNum, unsigned int A, unsigned int B)
{
    unsigned int out = 2;
    
    switch(gateNum)
    {
        
        case 0:  
                out = 0;
                break;
        
        case 1:  
                if(A == 1 && B == 0)
                    out = 1;
                else
                    out = 0; 
                break;
                
        case 2:  
                out = A;
                break;
                
        case 3:  
                if(A == 0 && B == 1)
                    out = 1;
                else
                    out = 0;
                break;
                
        case 4:  
                out = B;
                break;
                
        case 5:  
                out = 1 - B;
                break;
                
        case 6:  
                if(A == 0 && B == 1)
                    out = 0;
                else
                    out = 1;
                break;
                
        case 7:  
                out = 1 - A;
                break;
                
        case 8:  
                if(A == 1 && B == 0)
                    out = 0;
                else
                    out = 1;
                break;
                
        case 9:  
                out = 1;
                break;
        
        case 10: //AND  
                out = A & B;
                break;
        
        case 11: //OR  
                out = A | B;
                break;
        
        case 12: //NAND  
                out = !(A & B);
                break;
        
        case 13: //NOR 
                out = !(A | B);
                break;
        
        case 14: //XOR  
                out = A ^ B;
                break;
        
        case 15:  //XNOR
                out = !(A ^ B);
                break;
                
        default:
                out = 2;
                break;
    }
    
    return (out);        
}

/*

*/
unsigned int Bin2Dec(int *gate, int N)
{
    int i;
    unsigned int gateNum = 0;
    
    for(i = 0; i < N; i++)
    {
        gateNum += gate[i] * pow(2, (N-1-i));
    }
    return (gateNum);
}

/*
    
*/
void GateNumber(int *chrom, unsigned int *gateNumber, unsigned int *connectUp)
{
    int i, j, gatePos;
    int gate[30][5];
    
    gatePos = 0;
    for(i = 0; i < 30; i++)
    {
        for(j = 0; j < 5; j++)
        {
            gate[i][j] = chrom[gatePos++];
        }
    }
    
    for(i = 0; i < 30; i++)
    {
        gateNumber[i] = Bin2Dec(gate[i], 4);
        connectUp[i]  = gate[i][4]; 
    }
}

/*
    
*/
void GenerateSamples(unsigned int bitLength, unsigned int testVec[][6])
{
    unsigned int count = 0; 
    int i, j;
    
    unsigned int mask[6] = {0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
    
    for(i = 0; i < pow(2, bitLength); i++)
    {
        for(j = 0; j < bitLength; j++)
        {
            testVec[i][j] = (count & mask[j]) ? 1 : 0;
        }
        count++;
    }
}


/*
        
*/
unsigned int Eval(int *chrom, int lenChrom, unsigned int *input)
{
    int           gate[30][5];
    int           i, j;
    int           gatePos;
    
    unsigned int output[30];
    unsigned int inputA, inputB;
    unsigned int gateNum, isUp;
    unsigned int genOutput;
    
    gatePos = 0;
    for(i = 0; i < 30; i++)
    {
        //printf("\nGate[%d] = ", i);
        for(j = 0; j < 5; j++)
        {
            gate[i][j] = chrom[gatePos++];
            //printf("%d", gate[i][j]);        
        }
    }
    
    for(i = 0; i < 5; i++)
    {
        gatePos = i;
        for(j = 0; j < 6; j++)
        {
            gateNum = Bin2Dec(gate[gatePos], 4);
            isUp    = gate[gatePos][4];
            //printf("\ngateNum is: %d, gatePos is: %d, isUp is: %d\n", gateNum, gatePos, isUp);
             
             if(i == 0)
             {
                if(gatePos == 0 || gatePos == 5)
                {
                    inputA = input[0];
                    inputB = input[1];
                    output[gatePos] = Gate2InFunction(gateNum, inputA, inputB);
                    //printf("\ninputA = %d and inputB = %d", inputA, inputB);
                    //printf("\noutput[%d] = %d\n", gatePos, output[gatePos]);
                }
                else if(gatePos == 10 || gatePos == 15)
                {
                    inputA = input[2];
                    inputB = input[3];
                    output[gatePos] = Gate2InFunction(gateNum, inputA, inputB);
                    //printf("\ninputA = %d and inputB = %d", inputA, inputB);
                    //printf("\noutput[%d] = %d\n", gatePos, output[gatePos]);
                }
                else if(gatePos == 20 || gatePos == 25)
                {
                    inputA = input[4];
                    inputB = input[5];
                    output[gatePos] = Gate2InFunction(gateNum, inputA, inputB);
                    //printf("\ninputA = %d and inputB = %d", inputA, inputB);
                    //printf("\noutput[%d] = %d\n", gatePos, output[gatePos]);
                }
                else;
            }
            else if(j == 0 && i != 0)
            {
                inputA = output[gatePos - 1];
                inputB = (isUp)? output[gatePos + 9] : output[gatePos + 4]; //(gateNum -1) - 5
                output[gatePos] = Gate2InFunction(gateNum, inputA, inputB); 
                //printf("\ninputA = %d and inputB = %d", inputA, inputB);
                //printf("\noutput[%d] = %d\n", gatePos, output[gatePos]);               
            }
            else if(j == 5 && i != 0)
            {
                inputA = output[gatePos - 1];
                inputB = (isUp)? output[gatePos - 11] : output[gatePos - 6]; //(gateNum -1) - 5
                output[gatePos] = Gate2InFunction(gateNum, inputA, inputB); 
                //printf("\ninputA = %d and inputB = %d", inputA, inputB);
                //printf("\noutput[%d] = %d\n", gatePos, output[gatePos]);               
            }
            else
            {
                inputA = output[gatePos - 1];
                inputB = (isUp)? output[gatePos - 6] : output[gatePos + 4]; //(gateNum -1) - 5
                output[gatePos] = Gate2InFunction(gateNum, inputA, inputB); 
                //printf("\ninputA = %d and inputB = %d", inputA, inputB);
                //printf("\noutput[%d] = %d\n", gatePos, output[gatePos]);   
            }
            
            gatePos = gatePos + 5;   
        }    
    }
    
    genOutput = output[4];
    
    return(genOutput);
}

unsigned int EvalFitness(int *chrom, int lenChrom)
{
    unsigned int testVec[64][6];
    unsigned int genFitness[64], expFitness[64];
    int i, j;
    
    unsigned int fitness = 0;
    
    GenerateSamples(6, testVec);
    
    for(i = 0; i < 64; i++)
    {
        genFitness[i] = Eval(chrom, lenChrom, testVec[i]);
        expFitness[i] = 0;
    }
    
    for(i = 0; i < 64; i++)
    {
        for(j = 0; j < 6; j++)
        {
            expFitness[i] ^= testVec[i][j];
        }
    }
    
    for(i = 0; i < 64; i++)
    {
        if(genFitness[i] == expFitness[i])
        {
            fitness++;
        }
    }
    
    return(fitness);
}


/*
    Initialize a random population
*/

void InitPop(POPULATION *p)
{
    IPTR pi, pj;
    int  i, j;
    
    /*Initializing the data of the population*/
    printf("\nEnter the probability of crossover: ");
    scanf("%lf", &(p->pCross));
    
    printf("\nEnter the probability of mutation: ");
    scanf("%lf", &(p->pMut));
    
    printf("\nEnter the length of chromosome: ");
    scanf("%d", &(p->chromLen));
    
    printf("\nEnter the population size: ");
    scanf("%d", &(p->popSize));
    
    printf("\nEnter the maximum generation: ");
    scanf("%d", &(p->maxGen));
    
    /*Set progress indicator to zero*/
    p->highestEverFitness = 0.0; 
    p->highestEverGen     = 0;
    p->highestEverIndex   = 0;
    p->gen                = 0;
    p->maxi               = 0;
    p->mini               = 0;
    
    p->op = (IPTR) calloc(p->popSize, sizeof(INDIVIDUAL));
    p->np = (IPTR) calloc(p->popSize, sizeof(INDIVIDUAL));
    
    p->sumFitness = 0.0;
    
    for(i = 0; i < p->popSize; i++)
    {
        pi = &(p->op[i]);
        pi->chrom = (int*) calloc(p->chromLen, sizeof(int));
        
        pj = &(p->np[i]);
        pj->chrom = (int*) calloc(p->chromLen, sizeof(int));
        
        for(j = 0; j < p->chromLen; j++)
        {
            pi->chrom[j] = rand()%2;
        }
        
        pi->fitness = EvalFitness(&(pi->chrom[0]), p->chromLen); 
        
        p->sumFitness += pi->fitness;
    }
}

/*
   select a single individual by roulette wheel selection     
*/

int Roulette(IPTR pop, double sumFitness, int popsize)
{

    double prob021, 
           randNum, 
           partsum;
    int i;

    partsum = 0.0; 
    i = 0;

    prob021 = (double)(rand()%10000)/10000.0;

    randNum = prob021 * sumFitness; 

    i = -1;
    do
    {
        i++;
        partsum += pop[i].fitness;
    } 
    while (partsum < randNum && i < popsize - 1);

    return i;        
}

/*
    
*/
int muteX(POPULATION *p, int pa, int pb)
{
  double pMutation;
  
  pMutation = (double)(rand()%1000000)/1000000.0;

  if(pMutation <= p->pMut)
  {
    printf("\n=========MUTATION============\n");
  }
  
  return ((pMutation <= p->pMut) ? 1 - pa  : pa);
}

/*
        
*/
void CrossoverAndMutate(POPULATION *p, IPTR p1, IPTR p2, IPTR c1, IPTR c2)
{
    int *pi1,*pi2,*ci1,*ci2;
    int xp, i;

    double pSelect;

    pi1 = p1->chrom;
    pi2 = p2->chrom;
    ci1 = c1->chrom;
    ci2 = c2->chrom;

    pSelect = (double)(rand()%100000)/100000.0;
    
    //printf("\npSelect = %lf\n", pSelect);

    if(pSelect <= (p->pCross))
    {
        xp = rand()%p->chromLen;
        //printf("\ncross over point = %d\n", xp);
        
        for(i = 0; i < xp; i++)
        {
            ci1[i] = muteX(p, pi1[i],pi2[i]);
            ci2[i] = muteX(p, pi2[i],pi1[i]);
        }
        for(i = xp; i < p->chromLen; i++)
        {
            ci1[i] = muteX(p, pi2[i],pi1[i]);
            ci2[i] = muteX(p, pi1[i],pi2[i]);
        }
    } 
    else 
    {
        for(i = 0; i < p->chromLen; i++)
        {
            ci1[i] = muteX(p, pi1[i],pi2[i]);
            ci2[i] = muteX(p, pi2[i],pi1[i]);
        }
    }
}


void Generation(POPULATION *p)
{
    IPTR p1, p2, c1, c2;
    
    int selectP1, selectP2;
    
    int i;
    
    for(i = 0; i < p->popSize; i+= 2)
    {
        selectP1 = Roulette(p->op, p->sumFitness, p->popSize);
        selectP2 = Roulette(p->op, p->sumFitness, p->popSize);

        p1 = &(p->op[selectP1]);
        p2 = &(p->op[selectP2]);
        
        c1 = &(p->np[i]);
        c2 = &(p->np[i+1]);
        
        CrossoverAndMutate(p, p1, p2, c1, c2);    
        
        c1->fitness = EvalFitness(&(c1->chrom[0]), p->chromLen);
        c2->fitness = EvalFitness(&(c2->chrom[0]), p->chromLen);
    }
}

void Statistics(POPULATION *p, IPTR currentPop)
{
    int i;
    
    p->sumFitness = currentPop[0].fitness;
    p->max        = p->sumFitness;
    p->min        = p->sumFitness;
    p->avg        = p->sumFitness;
    
    for(i = 1; i < p->popSize; i++)
    {
        p->sumFitness += currentPop[i].fitness;
        
        if(p->max < currentPop[i].fitness)
        {
            p->max = currentPop[i].fitness; 
            p->maxi = i;
        }
        
        if(p->max > currentPop[i].fitness)
        {
            p->min = currentPop[i].fitness; 
            p->mini = i;
        }
    }
    
    p->avg = p->sumFitness/(double) p->popSize;
    
    if(p->highestEverFitness < p->max)
    {
        p->highestEverFitness = p->max;
        p->highestEverGen     = p->gen;
        p->highestEverIndex   = p->maxi;
    }
}

void Report(POPULATION *p)
{
    printf("\n======= For Generation: %d =========\n", p->gen);
    printf("\nMaximum fitness: %lf", p->max);
    printf("\nMinimum fitness: %lf", p->min);
    printf("\nAverage fitness: %lf", p->avg);
    printf("\nHighest Ever Fitness: %lf", p->highestEverFitness);
    printf("\nHighest Ever Generation: %d", p->highestEverGen);
    printf("\nHighest Ever Index: %d", p->highestEverIndex);
    printf("\n");
}

/*
    
*/
int main(void)
{
    POPULATION population;
    
    POPULATION *p = &population;
    
    unsigned int gateNumber[30];
    
    unsigned int connectUp[30];
    
    int i;
    
    IPTR temp;
    
    time_t tSec;
    
    /*Initialize the random number generator*/
    srand((unsigned int) time(&tSec));
    
    p->gen = 0;
    
    InitPop(p);
    
    /* Generate the statistical report */
        Statistics(p, p->np);
       
    /* Display the report for the generation */
    Report(p);
    
    while(p->gen < p->maxGen)
    {
        /* Increase generation count */
        p->gen++;
        
        /* Generate next generation */
        Generation(p);
        
        /* Generate the statistical report */
        Statistics(p, p->np);
       
        /* Display the report for the generation */
        Report(p);
        
        temp = p->op;
        p->op = p->np;
        p->np = temp;        
    }
    
    temp = &(p->op[p->highestEverIndex]);
    
    printf("\nFit Chromosome = ");
    for(i = 0; i < p->chromLen; i++)
    {
        printf("%d", temp->chrom[i]);
    }
    printf("\nFitness = %lf", temp->fitness);
    
    GateNumber(temp->chrom, gateNumber, connectUp);
    
    printf("\nChromosome in decimal: ");
    for(i = 0; i < 30; i++)
    {
        printf("%d [isUp = %d]\t", gateNumber[i], connectUp[i]);
    }
    
    return 0;
}

