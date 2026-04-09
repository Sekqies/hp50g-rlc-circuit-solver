#include "matrix_operations.h"

void multiply_matrix(const double complex** A, const double complex** B, const uint8_t size, double complex** res){
    complex double dot_sum = 0;
    for(uint8_t i = 0; i < size; ++i){
        for(uint8_t j = 0; j < size; ++j){
            dot_sum = 0;
            for(uint8_t k = 0; k < size; ++k){
                dot_sum += A[i][k] * B[k][j];
            }
            res[i][j] = dot_sum;
        }
    }
}

void multiply_matrix_vector(const double complex** A, const double complex* v, const uint8_t size, double complex* res){
    complex double sum = 0;
    for(uint8_t i = 0; i < size; ++i){
        sum = 0;
        for(uint8_t j = 0; j < size; ++j){
            sum+= A[i][j] * v[j];
        }
        res[i] = sum;
    }
}

void transpose_matrix(const double complex** A, const uint8_t size, double complex** res){
    for(uint8_t i = 0; i < size/2; ++i){
        for(uint8_t j = 0; j < size/2; ++j){
            res[j][i] = A[i][j];
            res[i][j] = A[j][i];
        }
    }
}


static double complex temp_a[MATRIX_SIZE][MATRIX_SIZE];
static uint8_t indxc[MATRIX_SIZE];
static uint8_t indxr[MATRIX_SIZE];
static uint8_t ipiv[MATRIX_SIZE];
// Markowitz Pivoting
void invert_matrix(const double complex** matrix_a, const uint8_t size, double complex** res) {
    for (uint8_t i = 0; i < size; ++i) {
        ipiv[i] = 0;
        for (uint8_t j = 0; j < size; ++j) {
            temp_a[i][j] = matrix_a[i][j];
        }
    }

    for (uint8_t i = 0; i < size; ++i) {
        uint8_t pivot_r = 0;
        uint8_t pivot_c = 0;
        uint32_t min_markowitz = 0xFFFFFFFF;
        double max_val = -1.0;

        for (uint8_t j = 0; j < size; ++j) {
            if (ipiv[j] != 1) {
                uint8_t row_nz = 0;
                for (uint8_t k = 0; k < size; ++k) {
                    if (ipiv[k] != 1 && cabs(temp_a[j][k]) > 1e-12) {
                        row_nz++;
                    }
                }

                for (uint8_t k = 0; k < size; ++k) {
                    if (ipiv[k] == 0) {
                        double val_mag = cabs(temp_a[j][k]);
                        if (val_mag > 1e-12) {
                            uint8_t col_nz = 0;
                            for (uint8_t m = 0; m < size; ++m) {
                                if (ipiv[m] != 1 && cabs(temp_a[m][k]) > 1e-12) {
                                    col_nz++;
                                }
                            }

                            uint32_t markowitz = (row_nz > 0 ? row_nz - 1 : 0) * (col_nz > 0 ? col_nz - 1 : 0);

                            if (markowitz < min_markowitz || (markowitz == min_markowitz && val_mag > max_val)) {
                                min_markowitz = markowitz;
                                max_val = val_mag;
                                pivot_r = j;
                                pivot_c = k;
                            }
                        }
                    }
                }
            }
        }

        ipiv[pivot_c] += 1;

        if (pivot_r != pivot_c) {
            for (uint8_t l = 0; l < size; ++l) {
                double complex temp = temp_a[pivot_r][l];
                temp_a[pivot_r][l] = temp_a[pivot_c][l];
                temp_a[pivot_c][l] = temp;
            }
        }

        indxr[i] = pivot_r;
        indxc[i] = pivot_c;

        double complex piv_inv = 1.0 / temp_a[pivot_c][pivot_c];
        temp_a[pivot_c][pivot_c] = 1.0;

        for (uint8_t l = 0; l < size; ++l) {
            temp_a[pivot_c][l] *= piv_inv;
        }

        for (uint8_t m = 0; m < size; ++m) {
            if (m != pivot_c) {
                double complex dummy = temp_a[m][pivot_c];
                temp_a[m][pivot_c] = 0.0;
                for (uint8_t l = 0; l < size; ++l) {
                    temp_a[m][l] -= temp_a[pivot_c][l] * dummy;
                }
            }
        }
    }

    for (int16_t l = size - 1; l >= 0; --l) {
        if (indxr[l] != indxc[l]) {
            for (uint8_t k = 0; k < size; ++k) {
                double complex temp = temp_a[k][indxr[l]];
                temp_a[k][indxr[l]] = temp_a[k][indxc[l]];
                temp_a[k][indxc[l]] = temp;
            }
        }
    }

    for (uint8_t i = 0; i < size; ++i) {
        for (uint8_t j = 0; j < size; ++j) {
            res[i][j] = temp_a[i][j];
        }
    }
}