#define D 50
#define WIN 3
#define N 150
#define H 60
#define C 2
#define P 315073697412756811

typedef struct protocolIO
{
  long long R1, R2, R1_, R2_;
  long long x[WIN*D], we[N][D], wx[WIN*D][H], w[H][C];
  int res;
}protocolIO;

long long mul_inverse;

void initiateRandoms(void* args);
void computeNN(void* args);
void extended_euclid(long long A1, long long A2, long long A3, long long B1, long long B2,long long B3);
obliv long long mul(obliv long long a, obliv long long b);
