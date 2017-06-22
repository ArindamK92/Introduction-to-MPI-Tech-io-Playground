#include <iostream>
#include <unistd.h>
#include <mpi.h>

int rank, size;
constexpr int buffer_count = 10;
int buffer[buffer_count];

void print_buffer(double time, int step, int *buffer, int n_elements) {
  std::cout << "Synchronisation at " << time+MPI_Wtime() << "ms; At step " << step << "; Buffer = (";
  for (int i=0; i < n_elements; ++i)
    std::cout << buffer[i] << (i==n_elements-1 ? ")" : " ");
  std::cout << std::endl;
}

#include "non_blocking.cpp"
#include "blocking.cpp"

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0)
    std::cout << "========= Running blocking scenario =========" << std::endl;
  play_blocking_scenario();

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0)
    std::cout << "======= Running non-blocking scenario =======" << std::endl;
  play_non_blocking_scenario();

  MPI_Finalize();
  
  return 0;
}