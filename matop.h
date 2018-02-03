#ifndef MATOP_H
#define MATOP_H
/**
 * @brief 
 * Local trandpose of matrix
 * @param x[IN/OUT] N x N matrix
 * @param N[IN]     dimension of matrix
 */
extern void DSPF_dp_mat_trans_local(double *restrict x, const int N);

/**
 * @brief 
 * Cross product of two vectors
 * @param x1[IN] pointer to 3 x 1 vector
 * @param x2[IN] pointer to 3 x 1 vector
 * @param y[OUT] pointer to 3 x 1 vector
 */
extern void DSPF_dp_vec_cross(double *x1, double *x2, double *restrict y);


/**
 * @brief 
 * Linear combination of two matrix
 * @param x1[IN]    r x c matrix
 * @param alpha[IN] scale factor of x1
 * @param x2[IN]    r x c matrix
 * @param beta[IN]  scale factor of x2
 * @param r[IN]     rows
 * @param c[IN]     columns
 * @param y[OUT]    r x c matrix
 */
extern void DSPF_dp_mat_linear_comb(double *x1, double alpha,double *x2, double beta, 
                            const int r, const int c, double *restrict y);

/**
 * @brief 
 * Invert matrix calculation of 3 x 3 matrix
 * @param x[IN]   3 x 3 matrix 
 * @param c[IN]  number ofcolumns of a row
 * @return det of matrix
 */
extern double DSPF_dp_mat_det(double *x,const int c);

/**
 * @brief 
 * Invert matrix calculation of 3 x 3 matrix
 * @param x[IN]   3 x 3 matrix 
 * @param c[IN]   number ofcolumns of a row
 * @param y[OUT]  3 x 3 matrix
 */
extern void DSPF_dp_mat_inv(double *x,const int c, double * y);

/**
 * @brief 
 * Matrix multiply (max dimension should be equal to or lower than 4)
 * @param x1[IN]  pointer to r1 x c1 matrix
 * @param r1[IN]  number of rows of x1
 * @param c1[IN]  number of columns of x1
 * @param x2[IN]  pointer to c1 x c2 matrix
 * @param c2[IN]  number of columns of x2
 * @param y[OUT]  pointer to r1 x c2 matrix
 */
extern void DSPF_dp_mat_mul_any(double *x1,const double a, const int r1, const int c1,
    double *x2, const int c2, double * y);

#endif