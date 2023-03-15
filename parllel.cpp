#include <iostream>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <omp.h>

using namespace std;

// A function that multiplies two matrices
vector<vector<int>> matrix_multiplication(vector<vector<int>> matrix_a, vector<vector<int>> matrix_b)
{
    // Get the number of rows and columns in matrix A
    int num_rows_a = matrix_a.size();
    int num_cols_a = matrix_a[0].size();
    // Get the number of rows and columns in matrix B
    int num_rows_b = matrix_b.size();
    int num_cols_b = matrix_b[0].size();

    // Check if the matrices can be multiplied
    if (num_cols_a != num_rows_b)
    {
        throw invalid_argument("Matrices cannot be multiplied");
    }

    // Create the result matrix
    vector<vector<int>> result_matrix(num_rows_a, vector<int>(num_cols_b, 0));

// Parallelize the outer loop that iterates over the rows of matrix A
#pragma omp parallel for
    for (int i = 0; i < num_rows_a; i++)
    {
        // Loop through each column in matrix B
        for (int j = 0; j < num_cols_b; j++)
        {
            // Loop through each element in the current row of matrix A
            for (int k = 0; k < num_cols_a; k++)
            {
                // Multiply the current element in matrix A with the current element in matrix B
                result_matrix[i][j] += matrix_a[i][k] * matrix_b[k][j];
            }
        }
    }

    // Return the result matrix
    return result_matrix;
}

int main(int argc, char *argv[])
{
    // Check if the number of arguments is correct
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " matrix_size num_threads" << endl;
        return 1;
    }

    // Parse the matrix size and number of threads from the command-line arguments
    int matrix_size = stoi(argv[1]);
    int num_threads = stoi(argv[2]);

    // Set the number of threads to use
    omp_set_num_threads(num_threads);

    // Define the input matrices
    vector<vector<int>> matrix_a(matrix_size, vector<int>(matrix_size, 1));
    vector<vector<int>> matrix_b(matrix_size, vector<int>(matrix_size, 1));

    try
    {
        // Call the matrix multiplication function and measure the time taken
        auto start_time = chrono::steady_clock::now();
        vector<vector<int>> result_matrix = matrix_multiplication(matrix_a, matrix_b);
        auto end_time = chrono::steady_clock::now();
        auto time_taken = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

        // Print the time taken to multiply the matrices
        cout << "Time taken: " << time_taken.count() << " ms" << endl;
    }
    catch (invalid_argument &e)
    {
        // Print an error message if the matrices cannot be multiplied
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
