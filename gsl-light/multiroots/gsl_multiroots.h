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

#ifndef __GSL_MULTIROOTS_H__
#define __GSL_MULTIROOTS_H__

#include <stdlib.h>
#include <gsl/gsl_types.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

__BEGIN_DECLS

/* From err/gsl_errno.h */
enum { 
  GSL_SUCCESS  = 0, 
  GSL_FAILURE  = -1,
  GSL_CONTINUE = -2,  /* iteration has not converged */
  //GSL_EDOM     = 1,   /* input domain error, e.g sqrt(-1) */
  //GSL_ERANGE   = 2,   /* output range error, e.g. exp(1e100) */
  //GSL_EFAULT   = 3,   /* invalid pointer */
  //GSL_EINVAL   = 4,   /* invalid argument supplied by user */
  //GSL_EFAILED  = 5,   /* generic failure */
  //GSL_EFACTOR  = 6,   /* factorization failed */
  //GSL_ESANITY  = 7,   /* sanity check failed - shouldn't happen */
  GSL_ENOMEM   = 8,   /* malloc failed */
  GSL_EBADFUNC = 9,   /* problem with user-supplied function */
  //GSL_ERUNAWAY = 10,  /* iterative process is out of control */
  //GSL_EMAXITER = 11,  /* exceeded max number of iterations */
  //GSL_EZERODIV = 12,  /* tried to divide by zero */
  GSL_EBADTOL  = 13,  /* user specified an invalid tolerance */
  //GSL_ETOL     = 14,  /* failed to reach the specified tolerance */
  //GSL_EUNDRFLW = 15,  /* underflow */
  //GSL_EOVRFLW  = 16,  /* overflow  */
  //GSL_ELOSS    = 17,  /* loss of accuracy */
  //GSL_EROUND   = 18,  /* failed because of roundoff error */
  //GSL_EBADLEN  = 19,  /* matrix, vector lengths are not conformant */
  //GSL_ENOTSQR  = 20,  /* matrix not square */
  //GSL_ESING    = 21,  /* apparent singularity detected */
  //GSL_EDIVERGE = 22,  /* integral or series is divergent */
  //GSL_EUNSUP   = 23,  /* requested feature is not supported by the hardware */
  //GSL_EUNIMPL  = 24,  /* requested feature not (yet) implemented */
  //GSL_ECACHE   = 25,  /* cache limit exceeded */
  //GSL_ETABLE   = 26,  /* table limit exceeded */
  //GSL_ENOPROG  = 27,  /* iteration is not making progress towards solution */
  //GSL_ENOPROGJ = 28,  /* jacobian evaluations are not improving the solution */
  //GSL_ETOLF    = 29,  /* cannot reach the specified tolerance in F */
  //GSL_ETOLX    = 30,  /* cannot reach the specified tolerance in X */
  //GSL_ETOLG    = 31,  /* cannot reach the specified tolerance in gradient */
  //GSL_EOF      = 32   /* end of file */
} ;

/* From err/error.c */
const char *
gsl_strerror (const int gsl_errno)
{
  switch (gsl_errno)
    {
    case GSL_SUCCESS:
      return "success" ;
    case GSL_FAILURE:
      return "failure" ;
    case GSL_CONTINUE:
      return "the iteration has not converged yet";
    case GSL_EDOM:
      return "input domain error" ;
    case GSL_ERANGE:
      return "output range error" ;
    case GSL_EFAULT:
      return "invalid pointer" ;
    case GSL_EINVAL:
      return "invalid argument supplied by user" ;
    case GSL_EFAILED:
      return "generic failure" ;
    case GSL_EFACTOR:
      return "factorization failed" ;
    case GSL_ESANITY:
      return "sanity check failed - shouldn't happen" ;
    case GSL_ENOMEM:
      return "malloc failed" ;
    case GSL_EBADFUNC:
      return "problem with user-supplied function";
    case GSL_ERUNAWAY:
      return "iterative process is out of control";
    case GSL_EMAXITER:
      return "exceeded max number of iterations" ;
    case GSL_EZERODIV:
      return "tried to divide by zero" ;
    case GSL_EBADTOL:
      return "specified tolerance is invalid or theoretically unattainable" ;
    case GSL_ETOL:
      return "failed to reach the specified tolerance" ;
    case GSL_EUNDRFLW:
      return "underflow" ;
    case GSL_EOVRFLW:
      return "overflow" ;
    case GSL_ELOSS:
      return "loss of accuracy" ;
    case GSL_EROUND:
      return "roundoff error" ;
    case GSL_EBADLEN:
      return "matrix/vector sizes are not conformant" ;
    case GSL_ENOTSQR:
      return "matrix not square" ;
    case GSL_ESING:
      return "singularity or extremely bad function behavior detected" ;
    case GSL_EDIVERGE:
      return "integral or series is divergent" ;
    case GSL_EUNSUP:
      return "the required feature is not supported by this hardware platform";
    case GSL_EUNIMPL:
      return "the requested feature is not (yet) implemented";
    case GSL_ECACHE:
      return "cache limit exceeded";
    case GSL_ETABLE:
      return "table limit exceeded";
    case GSL_ENOPROG:
      return "iteration is not making progress towards solution";
    case GSL_ENOPROGJ:
      return "jacobian evaluations are not improving the solution";
    case GSL_ETOLF:
      return "cannot reach the specified tolerance in F";
    case GSL_ETOLX:
      return "cannot reach the specified tolerance in X";
    case GSL_ETOLG:
      return "cannot reach the specified tolerance in gradient";
    case GSL_EOF:
      return "end of file";
    default:
      return "unknown error code" ;
    }
}

/* From err/error.c */
void
gsl_error (const char * reason, const char * file, int line, int gsl_errno)
{
  if (gsl_error_handler) 
    {
      (*gsl_error_handler) (reason, file, line, gsl_errno);
      return ;
    }

  gsl_stream_printf ("ERROR", file, line, reason);

  fflush (stdout);
  fprintf (stderr, "Default GSL error handler invoked.\n");
  fflush (stderr);

  abort ();
}

/* GSL_ERROR: call the error handler, and return the error code */
/* From err/gsl_errno.h */
#define GSL_ERROR(reason, gsl_errno) \
       do { \
       gsl_error (reason, __FILE__, __LINE__, gsl_errno) ; \
       return gsl_errno ; \
       } while (0)

/* From gsl_machine.h */
/* -*-MACHINE CONSTANTS-*-
 *
 * PLATFORM: Whiz-O-Matic 9000
 * FP_PLATFORM: IEEE-Virtual
 * HOSTNAME: nnn.lanl.gov
 * DATE: Fri Nov 20 17:53:26 MST 1998
 */
#define GSL_DBL_EPSILON        2.2204460492503131e-16

/* Definition of vector-valued functions with parameters based on gsl_vector */

struct gsl_multiroot_function_struct
{
  int (* f) (const gsl_vector * x, void * params, gsl_vector * f);
  size_t n;
  void * params;
};

typedef struct gsl_multiroot_function_struct gsl_multiroot_function ;

#define GSL_MULTIROOT_FN_EVAL(F,x,y) (*((F)->f))(x,(F)->params,(y))

int gsl_multiroot_fdjacobian (gsl_multiroot_function * F,
                              const gsl_vector * x, const gsl_vector * f,
                              double epsrel, gsl_matrix * jacobian);


typedef struct
  {
    const char *name;
    size_t size;
    int (*alloc) (void *state, size_t n);
    int (*set) (void *state, gsl_multiroot_function * function, gsl_vector * x, gsl_vector * f, gsl_vector * dx);
    int (*iterate) (void *state, gsl_multiroot_function * function, gsl_vector * x, gsl_vector * f, gsl_vector * dx);
    void (*free) (void *state);
  }
gsl_multiroot_fsolver_type;

typedef struct
  {
    const gsl_multiroot_fsolver_type * type;
    gsl_multiroot_function * function ;
    gsl_vector * x ;
    gsl_vector * f ;
    gsl_vector * dx ;
    void *state;
  }
gsl_multiroot_fsolver;

gsl_multiroot_fsolver * 
gsl_multiroot_fsolver_alloc (const gsl_multiroot_fsolver_type * T, 
                                     size_t n); 

void gsl_multiroot_fsolver_free (gsl_multiroot_fsolver * s);

int gsl_multiroot_fsolver_set (gsl_multiroot_fsolver * s, 
                               gsl_multiroot_function * f, 
                               const gsl_vector * x);

int gsl_multiroot_fsolver_iterate (gsl_multiroot_fsolver * s);

const char * gsl_multiroot_fsolver_name (const gsl_multiroot_fsolver * s);
gsl_vector * gsl_multiroot_fsolver_root (const gsl_multiroot_fsolver * s);
gsl_vector * gsl_multiroot_fsolver_dx (const gsl_multiroot_fsolver * s);
gsl_vector * gsl_multiroot_fsolver_f (const gsl_multiroot_fsolver * s);

/* Definition of vector-valued functions and gradient with parameters
   based on gsl_vector */

struct gsl_multiroot_function_fdf_struct
{
  int (* f) (const gsl_vector * x, void * params, gsl_vector * f);
  int (* df) (const gsl_vector * x, void * params, gsl_matrix * df);
  int (* fdf) (const gsl_vector * x, void * params, gsl_vector * f, gsl_matrix *df);
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
    int (*set) (void *state, gsl_multiroot_function_fdf * fdf, gsl_vector * x, gsl_vector * f, gsl_matrix * J, gsl_vector * dx);
    int (*iterate) (void *state, gsl_multiroot_function_fdf * fdf, gsl_vector * x, gsl_vector * f, gsl_matrix * J, gsl_vector * dx);
    void (*free) (void *state);
  }
gsl_multiroot_fdfsolver_type;

typedef struct
  {
    const gsl_multiroot_fdfsolver_type * type;
    gsl_multiroot_function_fdf * fdf ;
    gsl_vector * x;
    gsl_vector * f;
    gsl_matrix * J;
    gsl_vector * dx;
    void *state;
  }
gsl_multiroot_fdfsolver;

gsl_multiroot_fdfsolver *
gsl_multiroot_fdfsolver_alloc (const gsl_multiroot_fdfsolver_type * T,
                                      size_t n);

int
gsl_multiroot_fdfsolver_set (gsl_multiroot_fdfsolver * s, 
                             gsl_multiroot_function_fdf * fdf,
                             const gsl_vector * x);

int
gsl_multiroot_fdfsolver_iterate (gsl_multiroot_fdfsolver * s);

void
gsl_multiroot_fdfsolver_free (gsl_multiroot_fdfsolver * s);

const char * gsl_multiroot_fdfsolver_name (const gsl_multiroot_fdfsolver * s);
gsl_vector * gsl_multiroot_fdfsolver_root (const gsl_multiroot_fdfsolver * s);
gsl_vector * gsl_multiroot_fdfsolver_dx (const gsl_multiroot_fdfsolver * s);
gsl_vector * gsl_multiroot_fdfsolver_f (const gsl_multiroot_fdfsolver * s);

int gsl_multiroot_test_delta (const gsl_vector * dx, const gsl_vector * x, 
                              double epsabs, double epsrel);

int gsl_multiroot_test_residual (const gsl_vector * f, double epsabs);

GSL_VAR const gsl_multiroot_fdfsolver_type * gsl_multiroot_fdfsolver_newton;
GSL_VAR const gsl_multiroot_fdfsolver_type * gsl_multiroot_fdfsolver_gnewton;
GSL_VAR const gsl_multiroot_fdfsolver_type * gsl_multiroot_fdfsolver_hybridj;
GSL_VAR const gsl_multiroot_fdfsolver_type * gsl_multiroot_fdfsolver_hybridsj;


__END_DECLS

#endif /* __GSL_MULTIROOTS_H__ */
