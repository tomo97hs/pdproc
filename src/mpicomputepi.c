#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double myrand(void) { return (double)rand()/(double)RAND_MAX; }

double calc_pi(int seed, int trial)
{
  int i, n= 0;
  srand(seed);
  for (i=0; i<trial; i++) {
    double x = myrand();
    double y = myrand();
    if (x*x + y*y < 1.0) {
	n++;
    }
  }
  return 4.0*(double)n/(double)trial;
}

int main(int argc, char *argv[])
{
  int nprocs, rank, namelen;
  char name[MPI_MAX_PROCESSOR_NAME];
  double pi, sum;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(name, &namelen);
  pi = calc_pi(rank, 1000000);
  printf("%16s(%2d/%2d) pi = %f\n", name, rank, nprocs, pi);

  MPI_Barrier(MPI_COMM_WORLD);
  sum = 0;
  MPI_Reduce(&pi, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    sum /= (double)nprocs;
    printf("average = %f\n", sum);
  }

  MPI_Finalize();

  return 0;
}
