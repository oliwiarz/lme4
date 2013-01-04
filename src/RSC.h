// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// RSC.h: regular sparse matrix representation
//
// Copyright (C)       2012-2013 Douglas Bates, Martin Maechler and Ben Bolker
//
// This file is part of lme4.

#ifndef LME4_RSC_H
#define LME4_RSC_H

#include <Rcpp.h>

namespace lme4 {    
    class RSC { /**< mixed-effects predictor in regular sparse column format */
    protected:
        Rcpp::S4                  d_obj;
        const Rcpp::IntegerMatrix d_i; /**< rowvals indices for ZtXt */
        const Rcpp::NumericMatrix d_x; /**< matrix of non-zeros in ZtXt */
        const Rcpp::NumericVector d_theta; /**< variance component parameter vector */
        const Rcpp::NumericVector d_lower; /**< lower bounds for theta components */
// unfortunately, cannot include "CHM.h" in this header as definitions conflict
// with some in RcppEigen.h
//      CHM::dsCMatrix            d_A; /**< system matrix for PLS problem */
        Rcpp::S4                  d_A4;
        const Rcpp::NumericVector d_ubeta; /**< coefficient vector */
        const Rcpp::IntegerVector d_uboff; /**< 0-based offsets into ubeta */
        const int d_k;   /**< number of random effects per obs */
        const int d_kpp; /**< number of rows (k + p) in d_x and d_i  */
        const int d_n;   /**< number of observations */
        const int d_p;   /**< number of fixed-effects coefficients */
        const int d_qpp; /**< size of A and length of ubeta (q + p) */
        const int d_q;   /**< total number of random effects */
    public:
        RSC(const Rcpp::IntegerMatrix&, const Rcpp::NumericMatrix&,
            const Rcpp::NumericVector&, const Rcpp::NumericVector&,
            Rcpp::S4&,
            const Rcpp::NumericVector&, const Rcpp::IntegerVector&);

        RSC(Rcpp::S4);

        const Rcpp::IntegerMatrix&      i() const {return d_i;}
        const Rcpp::NumericMatrix&      x() const {return d_x;}
        const Rcpp::NumericVector&  theta() const {return d_theta;}
        const Rcpp::NumericVector&  lower() const {return d_lower;}
        const Rcpp::NumericVector&  ubeta() const {return d_ubeta;}
        const Rcpp::IntegerVector&  uboff() const {return d_uboff;}

        Rcpp::NumericVector &apply_lambda(Rcpp::NumericVector&, double) const;
        Rcpp::List               update_A(const Rcpp::NumericVector&,
					  const Rcpp::NumericVector&,
					  const Rcpp::NumericVector&);
        Rcpp::List         scale_update_A(const Rcpp::NumericVector&,
					  const Rcpp::NumericMatrix&);
        Rcpp::NumericVector         Ldiag();
        Rcpp::NumericVector        fitted(const Rcpp::NumericVector&) const;

        int rv(int i, int j) const {return (i < d_k) ? d_i(i,j) : d_uboff[i];}
    };
}

#endif // LME4_RSC_H
