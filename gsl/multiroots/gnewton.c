/* multiroots/gnewton.c
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

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <string.h>

#include <errno.h>
#include <multiroots.h>

#include "enorm.c"

/* Simple globally convergent Newton method (rejects uphill steps) */

typedef struct
  {
    double phi;
    double * x_trial;
    double * d;
    double * lu;
    int * permutation;
  }
gnewton_state_t;

static int gnewton_alloc (void * vstate, size_t n);
static int gnewton_set (void * vstate, gsl_multiroot_function_fdf * fdf, double * x, double * f, double * J, double * dx);
static int gnewton_iterate (void * vstate, gsl_multiroot_function_fdf * fdf, double * x, double * f, double * J, double * dx);
static void gnewton_free (void * vstate);

static int
gnewton_alloc (void * vstate, size_t n)
{
  gnewton_state_t * state = (gnewton_state_t *) vstate;
  double * d, * x_trial ;
  int * p;
  double * m;

  m = calloc (n*n,sizeof(double));//gsl_matrix_calloc(n,n)
  
  if (m == 0) 
    {
      fprintf(stderr,"%s (%s:%d): ", __func__,__FILE__,__LINE__);
      perror ("failed to allocate space for lu");
    }

  state->lu = m ;

  p = calloc(n*n,sizeof(int));//gsl_permutation_calloc(n);

  if (p == 0)
    {
      free(m);//gsl_matrix_free

      fprintf(stderr,"%s (%s:%d): ", __func__,__FILE__,__LINE__);
      perror ("failed to allocate space for permutation");
    }

  state->permutation = p ;

  d = calloc (n,sizeof(double));//gsl_vector_calloc

  if (d == 0)
    {
      free(p);//gsl_permutation_free
      free(m);//gsl_matrix_free

      fprintf(stderr,"%s (%s:%d): ", __func__,__FILE__,__LINE__);
      perror ("failed to allocate space for d");
    }

  state->d = d;

  x_trial = calloc (n,sizeof(double));//gsl_vector_calloc

  if (x_trial == 0)
    {
      free(m);//gsl_vector_free
      free(p);//gsl_permutation_free
      free(m);//gsl_matrix_free

      fprintf(stderr,"%s (%s:%d): ", __func__,__FILE__,__LINE__);
      perror ("failed to allocate space for x_trial");
    }

  state->x_trial = x_trial;

  return EXIT_SUCCESS;
}


static int
gnewton_set (void * vstate, gsl_multiroot_function_fdf * FDF, double * x, double * f, double * J, double * dx)
{
  gnewton_state_t * state = (gnewton_state_t *) vstate;
  size_t i, n = FDF->n ;

  /*
  fprintf(stderr,"\n gnewton_set : Print vector x before set: ");
  for (i = 0; i < n; i++)
      fprintf(stderr,"%f, ", x[i]);
  fprintf(stderr,"\n end vector x before set\n ");
  */
  GSL_MULTIROOT_FN_EVAL_F_DF (FDF, x, f, J);
  /*
  fprintf(stderr,"\n gnewton_set : Print vector f after set: ");
  for (i = 0; i < n; i++)
      fprintf(stderr,"%f, ", f[i]);
  fprintf(stderr,"\n end vector f after set\n ");

  fprintf(stderr,"\n gnewton_set : Print matrix J after set: ");
  for (i = 0; i < n; i++)
    for (size_t j = 0; j < n; j++)
    {
      fprintf(stderr,"%f, ", J[i,j]);
    }
  fprintf(stderr,"\n end matrix J after set\n ");
  */
  for (i = 0; i < n; i++)
    {
      dx[i]= 0.0;
    }

  state->phi = enorm(f,n);

  return EXIT_SUCCESS;
}

static int
gnewton_iterate (void * vstate, gsl_multiroot_function_fdf * fdf, double * x, double * f, double * J, double * dx)
{
  gnewton_state_t * state = (gnewton_state_t *) vstate;
  
  double t, phi0, phi1;

  size_t i;//iteration variable

  int n = fdf->n ;

  int nrhs=1;//number of right hand side vectors

  int lda=n;//number of lines of the matrix A

  int ldb=n;//number of lines of the RHS b

  int info=0;

  memcpy (state->lu, J, n*n*sizeof(double));//gsl_matrix_memcpy (state->lu, J);

  memcpy (state->d, f, n*sizeof(double));

  dgesv_( &n, &nrhs, state->lu, &lda, state->permutation, state->d, &ldb, &info);

  if (info<0)
    {
      fprintf(stderr,"%s (%s:%d): ", __func__,__FILE__,__LINE__);
      printf("Argument %i had an illegal value", -info);
      perror("Invalid argument (illegal value)" ); 
      return EXIT_FAILURE;
    }
  else if (info>0)
    {
      fprintf(stderr,"%s (%s:%d): ", __func__,__FILE__,__LINE__);
      perror("gnewton : The factorization LU has been completed, but the factor U is exactly singular, so the solution could not be computed " ); 
      return EXIT_FAILURE;
    }

  t = 1;

  phi0 = state->phi;

new_step:

  for (i = 0; i < n; i++)
    {
      double di = state->d[i];
      double xi = x[i];
      state->x_trial[i]= xi - t*di;
    }
  
  { 
    int status = GSL_MULTIROOT_FN_EVAL_F (fdf, state->x_trial, f);
    
    if (status != EXIT_SUCCESS)
      {
        fprintf(stderr,"%s (%s:%d): ", __func__,__FILE__,__LINE__);
        return EXIT_FAILURE;
      }
  }
  
  phi1 = enorm (f,n);

  if (phi1 > phi0 && t > LDBL_EPSILON)  
    {
      /* full step goes uphill, take a reduced step instead */

      double theta = phi1 / phi0;
      double u = (sqrt(1.0 + 6.0 * theta) - 1.0) / (3.0 * theta);

      t *= u ;
     
      goto new_step;
    }

  /* copy x_trial into x */

  memcpy (x, state->x_trial, n*sizeof(double));//gsl_vector_memcpy (x, state->x_trial);

  for (i = 0; i < n; i++)
    {
      double di = state->d[i];
      dx[i]= -t*di;
    }

  { 
    int status = GSL_MULTIROOT_FN_EVAL_DF (fdf, x, J);
    
    if (status != EXIT_SUCCESS)
      {
        fprintf(stderr,"%s (%s:%d): ", __func__,__FILE__,__LINE__);
        return EXIT_FAILURE;
      }
  }

  state->phi = phi1;

  return EXIT_SUCCESS;
}


static void
gnewton_free (void * vstate)
{
  gnewton_state_t * state = (gnewton_state_t *) vstate;

  free(state->d);//gsl_vector_free
  free(state->x_trial);//gsl_vector_free
  free(state->lu);//gsl_matrix_free
  free(state->permutation);//gsl_permutation_free
}


static const gsl_multiroot_fdfsolver_type gnewton_type =
{"gnewton",                             /* name */
 sizeof (gnewton_state_t),
 &gnewton_alloc,
 &gnewton_set,
 &gnewton_iterate,
 &gnewton_free};

const gsl_multiroot_fdfsolver_type  * gsl_multiroot_fdfsolver_gnewton = &gnewton_type;
