#include <iostream>
#include <vector>
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

int main()
{
    // Define the input matrices
    vector<vector<int>> matrix_a = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    vector<vector<int>> matrix_b = {{10, 11}, {12, 13}, {14, 15}};

    try
    {
        // Call the matrix multiplication function
        vector<vector<int>> result_matrix = matrix_multiplication(matrix_a, matrix_b);

        // Print the result matrix
        for (vector<int> row : result_matrix)
        {
            for (int element : row)
            {
                cout << element << " ";
            }
            cout << endl;
        }
    }
    catch (invalid_argument &e)
    {
        // Print an error message if the matrices cannot be multiplied
        cerr << e.what() << endl;
    }

    return 0;
}
