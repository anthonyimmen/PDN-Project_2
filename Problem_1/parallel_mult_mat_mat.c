#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <omp.h>

// This will be the size of the buffer
#define BUFSIZE 8192

#define DEBUG 0

/* ----------- Project 2 - Problem 1 - Matrix Mult -----------

    This file will multiply two matricies.
    Complete the TODOs in order to complete this program.
    Remember to make it parallelized!
*/ // ------------------------------------------------------ //


// TODO: Parallelize the matrix-matrix multiplication

// PROMPTED CHATGPT: "write me an algorithm to perform matrix multiplication in C 
// using two 2D matrices that are represented in a 1D array."

long int* matrix_multiply(long int* matrix1, long int* matrix2, int m, int n, int p, int threads) {
  int i, j, k;
  long int *result = (long int *)malloc(m * p * sizeof(long int));
  // PROMPTED CHATGPT: "can you parallelize the code you just wrote?"
  #pragma omp parallel for shared(matrix1, matrix2, result) private(i, j, k) num_threads(threads)
  for (i = 0; i < m; i++) {
    for (j = 0; j < p; j++) {
      int sum = 0;
      for (k = 0; k < n; k++) {
        sum += matrix1[i * n + k] * matrix2[k * p + j];
      }
      result[i * p + j] = sum;
    }
  }
  return result;
}

// END PROMPTED CODE

int main(int argc, char* argv[])
{
    // Catch console errors
    if (argc != 10)
    {
        printf("%d", argc);
        printf("USE LIKE THIS: parallel_mult_mat_mat   mat_1.csv n_row_1 n_col_1   mat_2.csv n_row_2 n_col_2   num_threads   results_matrix.csv   time.csv\n");
        return EXIT_FAILURE;
    }

    // Get the input files
    FILE* inputMatrix1 = fopen(argv[1], "r");
    FILE* inputMatrix2 = fopen(argv[4], "r");

    char* p1;
    char* p2;

    // Get matrix 1's dims
    int n_row1 = strtol(argv[2], &p1, 10);
    int n_col1 = strtol(argv[3], &p2, 10);

    // Get matrix 2's dims
    int n_row2 = strtol(argv[5], &p1, 10);
    int n_col2 = strtol(argv[6], &p2, 10);

    // Get num threads
    int thread_count = strtol(argv[7], NULL, 10);

    // Get output files
    FILE* outputFile = fopen(argv[8], "w");
    FILE* outputTime = fopen(argv[9], "w");


    // TODO: malloc the two input matrices and the output matrix
    // Please use long int as the variable type

    // Create the variables needed for the buffer
    char *buffer = (char*) malloc(BUFSIZE);
    char *ptr, *line;

    // Allocate a 1D array to store all the elements in the 2D array
    // for the matrix
    long int* matrix1 = (long int*)malloc((n_row1 * n_col1) * sizeof(long int));

    // Allocate a 1D array to store all the elements in the 2D array
    // for the matrix
    long int* matrix2 = (long int*)malloc((n_row2 * n_col2) * sizeof(long int));

    // TODO: Parse the input csv files and fill in the input matrices


    // Read the matrices from the input file
    // Write values into array for matrix1
    for(long int row = 0; row < n_row1; row++) {
        line = fgets(buffer, BUFSIZE, inputMatrix1);
        ptr = strtok(line, ",");
        for(long int column = 0; column < n_col1; column++) {
            matrix1[row * n_col1 + column] = atoi(ptr);
            ptr = strtok(NULL, ",");
            if (ptr == NULL) {
                break;
            }
	}
    }

    // Read the matrices from the input file
    // Write values into array for matrix2
    for(long int row = 0; row < n_row2; row++) {
        line = fgets(buffer, BUFSIZE, inputMatrix2);
        ptr = strtok(line, ",");
        for(long int column = 0; column < n_col2; column++) {
            matrix2[row * n_col2 + column] = atoi(ptr);
            ptr = strtok(NULL, ",");
            if (ptr == NULL) {
                break;
            }
	    }
    }

    // PROMPTED CHATGPT: "write me an algorithm to perform matrix multiplication in C 
    // using two 2D matrices that are represented in a 1D array."
    
    // Write the result matrix to the file

    int m = n_row1;
    int n = n_col1;
    int p = n_col2;

    // We are interesting in timing the matrix-matrix multiplication only
    // Record the start time
    double start = omp_get_wtime();

    long int *result = matrix_multiply(matrix1, matrix2, m, n, p, thread_count);

    // Record the finish time        
    double end = omp_get_wtime();

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            fprintf(outputFile, "%ld", result[i * p + j]);
            if (j < p-1) {
                fprintf(outputFile, "%s", ",");
            }
        }
        if (i < m-1) {  
            fprintf(outputFile, "\n");
        }
    }

    // END PROMPTED CODE
    
    // Time calculation (in seconds)
    double time_passed = end - start;

    // Save time to file
    fprintf(outputTime, "%f", time_passed);

    // Cleanup and free buffers
    fclose(inputMatrix1);
    fclose(inputMatrix2);
    fclose(outputFile);
    fclose(outputTime);
    free(buffer);

    return 0;
}


