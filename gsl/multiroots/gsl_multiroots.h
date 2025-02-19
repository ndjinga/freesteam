/* multiroots/gsl_multiroots.h
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000, 2007 Brian Gough
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __MULTIROOTS_H__
#define __MULTIROOTS_H__

#include <stdlib.h>
#include <math.h>

/* Definition of vector-valued functions and gradient with parameters
   based on double */

struct gsl_multiroot_function_fdf_struct
{
  int (* f) (const gsl_vector * x, void * params, double * f);
  int (* df) (const double * x, void * params, double * df);
  int (* fdf) (const double * x, void * params, double * f, double *df);
  size_t n;
  void * params;
};

typedef struct gsl_multiroot_function_fdf_struct gsl_multiroot_function_fdf ;

#define GSL_MULTIROOT_FN_EVAL_F(F,x,y) ((*((F)->f))(x,(F)->params,(y)))
#define GSL_MULTIROOT_FN_EVAL_DF(F,x,dy) ((*((F)->df))(x,(F)->params,(dy)))
#define GSL_MULTIROOT_FN_EVAL_F_DF(F,x,y,dy) ((*((F)->fdf))(x,(F)->params,(y),(dy)))

typedef struct
  {
    const char *name;
    size_t size;
    int (*alloc) (void *state, size_t n);
    int (*set) (void *state, gsl_multiroot_function_fdf * fdf, double * x, double * f, double * J, double * dx);
    int (*iterate) (void *state, gsl_multiroot_function_fdf * fdf, double * x, double * f, double * J, double * dx);
    void (*free) (void *state);
  }
gsl_multiroot_fdfsolver_type;

typedef struct
  {
    const gsl_multiroot_fdfsolver_type * type;
    gsl_multiroot_function_fdf * fdf ;
    double * x;
    double * f;
    double * J;
    double * dx;
    void *state;
  }
gsl_multiroot_fdfsolver;

gsl_multiroot_fdfsolver *
gsl_multiroot_fdfsolver_alloc (const gsl_multiroot_fdfsolver_type * T,
                                      size_t n);

int
gsl_multiroot_fdfsolver_set (gsl_multiroot_fdfsolver * s, 
                             gsl_multiroot_function_fdf * fdf,
                             const double * x);

int
gsl_multiroot_fdfsolver_iterate (gsl_multiroot_fdfsolver * s);

void
gsl_multiroot_fdfsolver_free (gsl_multiroot_fdfsolver * s);

const char * gsl_multiroot_fdfsolver_name (const gsl_multiroot_fdfsolver * s);
double * gsl_multiroot_fdfsolver_root (const gsl_multiroot_fdfsolver * s);
double * gsl_multiroot_fdfsolver_dx (const gsl_multiroot_fdfsolver * s);
double * gsl_multiroot_fdfsolver_f (const gsl_multiroot_fdfsolver * s);

int gsl_multiroot_test_delta (const double * dx, const double * x, 
                              double epsabs, double epsrel);

int gsl_multiroot_test_residual (const double * f, double epsabs);

#endif /* __MULTIROOTS_H__ */
