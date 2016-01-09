#include<stdio.h>
#include<stdlib.h>
#include<obliv.h>
#include<string.h>
#include"secure_deep_learning.h"
#include <time.h>

#include"../common/util.h"

char dict[N-1][30];

long long mul_plain(long long a, long long b)
{
  bool sign;
  long long s = 0;
  if(a>=0)
  {
    if(b>=0)
      sign = false;
    else
      sign = true;
  }
  else
  {
    if(b<0)
      sign = false;
    else
      sign = true;
  }
  a = (a<0)?-a:a;
  b = (b<0)?-b:b;
  while(b)
  {
    if(b%2)
      s = (s+a)%P;
    b /= 2;
    a = (a+a)%P;
  }
  if(sign)
    return -s;
  return s;
}

int main(int argc,char *argv[])
{
  char c[30], line[300], *splitLine;
  int d, i, j, posLine[20], cnt = 1, len;
  FILE *fptr;
  ProtocolDesc pd;
  protocolIO io;
  clock_t start, end;
  double cpu_time_used;

/* Ideally initiateRandoms() will be executed only by party 2.
   Party 2 will mask 'we' by multiplying it with R1_ and R2_ and pass it to party 1 through OT protocol.
   For sample execution, party 1 is considered to hold masked 'we' here.
*/
  initiateRandoms(&io);

if(argv[1][0]=='2')
{
  fptr=fopen("parameters/wx.txt","r");
  for(i = 0; i < WIN*D; i++)
  {
    for(j = 0; j < H; j++)
    {
      fscanf(fptr,"%s", c);
      io.wx[i][j] = mul_plain(atoi(c), io.R1);
    }
  }
  fclose(fptr);
  fptr=fopen("parameters/w.txt","r");
  for(i = 0; i < H; i++)
  {
    for(j = 0; j < C; j++)
    {
      fscanf(fptr,"%s", c);
      io.w[i][j] = mul_plain(atoi(c), io.R2);
    }
  }
  fclose(fptr);
}

if(argv[1][0]=='1')
{
  fptr=fopen("parameters/we1.txt","r");
  for(i = 0; i < N; i++)
  {
    for(j = 0; j < D; j++)
    {
      fscanf(fptr,"%s", c);
      io.we[i][j] = mul_plain(mul_plain(atoi(c), io.R1_), io.R2_);
    }
  }
  fclose(fptr);

  fptr=fopen("parameters/dic.txt","r");
  while(!feof(fptr))
  {
    fscanf(fptr,"%s", c);
    fscanf(fptr,"%d", &d);
    strcpy(dict[d], c);
  }
  fclose(fptr);

  strcpy(line, "this is a sample statement with some vagueness");
  fprintf(stderr,"%s\n",line);
  splitLine = strtok(line, " ");
  posLine[0] = N-1;
  while (splitLine != NULL)
  {
    posLine[cnt] = N-1;
    for(i = 0; i < N-1; i++)
    {
	if(strcmp(splitLine, dict[i]) == 0)
	   posLine[cnt] = i;
    }
    fprintf(stderr,"%s: %d\n",splitLine, posLine[cnt]);
    cnt++;
    splitLine = strtok (NULL, " ");
  }
  posLine[cnt] = N-1;
  len = cnt + 1;
// Each word of input sentence is taken with context window of 3 (WIN) words, i.e., the word and its 2 neighbouring words
  for(d = 0; d < len-2; d++) 
    for(i = 0; i < WIN; i++) 
      for(j = 0; j < D; j++)
        io.x[i*D + j] = io.we[posLine[d+i]][j];
}

  start = clock();

/* Detect if the last word is vague or not (since value of io.x corresponds to last word of the input sentence).
   The protocol can be extended to be invoked for each word in the sentence.
*/
  protocolUseStdio(&pd);
  setCurrentParty(&pd,argv[1][0]=='1'?1:2);
  execYaoProtocol(&pd,computeNN,&io);
  cleanupProtocol(&pd);
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  fprintf(stderr,"Result: %d\n",io.res);
  fprintf(stderr,"Elapsed Time: %f\n",cpu_time_used);

  return 0;
}
