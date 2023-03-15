#include <iostream>
#include <cstdlib>
#include <omp.h>

using namespace std;

// Sequential matrix multiplication
void sequential_multiply(int **A, int **B, int **C, int n)
{
    // Iterate over rows of A
    for (int i = 0; i < n; i++)
    {
        // Iterate over columns of B
        for (int j = 0; j < n; j++)
        {
            int sum = 0;
            // Iterate over corresponding elements of A and B
            for (int k = 0; k < n; k++)
            {
                sum += A[i][k] * B[k][j];
            }
            // Store the computed dot product in C
            C[i][j] = sum;
        }
    }
}

// Parallel matrix multiplication using OpenMP
void parallel_multiply(int **A, int **B, int **C, int n, int num_threads)
{
    // Set the number of OpenMP threads to use
    omp_set_num_threads(num_threads);

// Use OpenMP parallel for loop to distribute the outer two loops across threads
#pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int sum = 0;
            // Iterate over corresponding elements of A and B
            for (int k = 0; k < n; k++)
            {
                sum += A[i][k] * B[k][j];
            }
            // Store the computed dot product in C
            C[i][j] = sum;
        }
    }
}

int main(int argc, char **argv)
{
    // Check that the correct number of command line arguments were provided
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <input_size> <num_threads>" << endl;
        return 1;
    }

    // Parse command line arguments
    int n = atoi(argv[1]);           // Size of matrices
    int num_threads = atoi(argv[2]); // Number of OpenMP threads to use

    // Allocate memory for matrices
    int **A = new int *[n];
    int **B = new int *[n];
    int **C = new int *[n];

    for (int i = 0; i < n; i++)
    {
        A[i] = new int[n];
        B[i] = new int[n];
        C[i] = new int[n];
    }

    // Initialize matrices with random values
    srand(0);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }

    // Multiply matrices
    double start_time = omp_get_wtime();
    parallel_multiply(A, B, C, n, num_threads);
    double end_time = omp_get_wtime();

    // Print execution time
    cout << "Execution time: " << (end_time - start_time) * 1000 << " ms" << endl;

    // Deallocate memory
    for (int i = 0; i < n; i++)
    {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
