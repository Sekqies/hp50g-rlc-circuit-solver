#pragma once

#include "constants.h"
#include "types.h"


void multiply_matrix(const double complex** A, const double complex** B, const uint8_t size, double complex** ans);

void transpose_matrix(const double complex** A, const uint8_t size, double complex** res);

void transpose_matrix(const double complex** A, const uint8_t size, double complex** res);

void invert_matrix(const double complex** A, const uint8_t size, double complex** res);

void multiply_matrix_vector(const double complex** A, const double complex* v, const uint8_t size, double complex* res);