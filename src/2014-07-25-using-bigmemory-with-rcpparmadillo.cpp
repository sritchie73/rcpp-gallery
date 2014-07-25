// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
/**
 * @title Using RcppArmadillo with
 * @author Scott Ritchie
 * @license GPL (>= 2)
 * @tags armadillo bigmemory matrix
 * @summary This example shows how to use RcppArmadillo with `big.matrix` objects from the `bigmemory` package.
 *
 */

#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo, BH, bigmemory)]]

using namespace Rcpp ;

// The function which performs the linear algebra you want
//
void someArmaFunction(const arma::Mat<T>& aBigMat) {
  // Do some linear algebra with RcppArmadillo
}

// Dispatch function for someArmaFunction
//
// We need to write this wrapper to handle the different types used
// when constructing a big.matrix
//
// [[Rcpp::export]]
void someArmaFunction(SEXP pBigMat) {
  XPtr<BigMatrix> xpMat(pBigMat);

  // Normally we would use the MatrixAccessor class to access the matrix
  // associated with the external pointer. The matrix is stored as the
  // `matrix` field of `xpMat`, and the MatrixAccessor simply provides
  // convenient methods for accessing its elements.
  //
  // This is just a pointer to an array, which is laid out in memory in
  // the column major format. Armadillo matrices are also stored in column
  // major format. We can therefore use the advanced `arma::mat` constructor
  // with `copy_aux_mem` set to `false` to effectively "cast" this memory
  // to an object RcppArmadillo understands.
  //
  // Warning about safety and accessing the memory appropriately, along
  // with the advanced option of `strict=true` if you want to modify the
  // size of the matrix later.
  //
  unsigned int type = xpMat->matrix_type();
  if (type == 1) {
    someArmaFunction<char>(
      arma::Mat<char>((char *)xpDat->matrix(), xpDat->nrow(), xpDat->ncol(), false)
    );
  } else if (type == 2) {
    someArmaFunction<short>(
      arma::Mat<short>((short *)xpDat->matrix(), xpDat->nrow(), xpDat->ncol(), false)
    );
  } else if (type == 4) {
    someArmaFunction<int>(
      arma::Mat<int>((int *)xpDat->matrix(), xpDat->nrow(), xpDat->ncol(), false)
    );
  } else if (type == 8) {
    someArmaFunction<double>(
      arma::Mat<double>((double *)xpDat->matrix(), xpDat->nrow(), xpDat->ncol(), false)
    );
  } else {
    throw Rcpp::exception("Undefined type for provided big.matrix");
  }
}

/*** R
M <- as.big.matrix(matrix(1:6, nrow=2))
someArmaFunction(M@address)
*/

