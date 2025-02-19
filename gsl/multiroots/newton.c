/* multiroots/newton.c
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

#include <config.h>

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include <math.h>//<gsl/gsl_math.h>
#include <errno.h>//<gsl/gsl_errno.h>
#include <gsl/gsl_multiroots.h>
#include <gsl/gsl_linalg.h>

typedef struct
  {
    double * lu;//gsl_matrix
    int * permutation;//gsl_permutation
  }
newton_state_t;

static int newton_alloc (void * vstate, size_t n);
static int newton_set (void * vstate, gsl_multiroot_function_fdf * fdf, double * x, double * f, double * J, double * dx);
static int newton_iterate (void * vstate, gsl_multiroot_function_fdf * fdf, double * x, double * f, double * J, double * dx);
static void newton_free (void * vstate);

static int
newton_alloc (void * vstate, size_t n)
{
  newton_state_t * state = (newton_state_t *) vstate;
  int * p;//gsl_permutation
  double * m;//gsl_matrix

  m = calloc (n,n,sizeof(double));//gsl_matrix_calloc(n,n)
  
  if (m == 0) 
    {
      perror ("failed to allocate space for lu");
    }

  state->lu = m ;

  p = calloc(n,n,sizeof(int));//gsl_permutation_calloc(n)

  if (p == 0)
    {
      free(m);

      perror ("failed to allocate space for permutation");
    }

  state->permutation = p ;

  return EXIT_SUCCESS;
}

static int 
newton_set (void * vstate, gsl_multiroot_function_fdf * FDF, double * x, double * f, double * J, double * dx)
{
  newton_state_t * state = (newton_state_t *) vstate;

  size_t i, n = FDF->n ;

  state = 0 ; /* avoid warnings about unused parameters */

  GSL_MULTIROOT_FN_EVAL_F_DF (FDF, x, f, J);

  for (i = 0; i < n; i++)
    {
      dx[i] = 0.0;
    }

  return EXIT_SUCCESS;
}

static int
newton_iterate (void * vstate, gsl_multiroot_function_fdf * fdf, double * x, double * f, double * J, double * dx)
{
  newton_state_t * state = (newton_state_t *) vstate;
  
  int signum;

  size_t i;

  size_t n = fdf->n ;

  memcpy (state->lu, J,n*n);//gsl_matrix_memcpy(state->lu, J)

  gsl_linalg_LU_decomp (state->lu, state->permutation, &signum);

  {
    int status = gsl_linalg_LU_solve (state->lu, state->permutation, f, dx);

    if (status)
      return status;
  }   
      
  for (i = 0; i < n; i++)
    {
      double e = dx[i];
      double y = x[i];
      dx[i] = -e;
      x[i]  = y - e);
    }

  {
    int status = GSL_MULTIROOT_FN_EVAL_F_DF (fdf, x, f, J);
    
    if (status != EXIT_SUCCESS) 
      {
        return perror ("Problem in function evaluation");
      }
  }

  return EXIT_SUCCESS;
}


static void
newton_free (void * vstate)
{
  newton_state_t * state = (newton_state_t *) vstate;

  free(state->lu);//gsl_matrix_free

  free(state->permutation);//gsl_permutation_free
}


static const gsl_multiroot_fdfsolver_type newton_type =
{"newton",                              /* name */
 sizeof (newton_state_t),
 &newton_alloc,
 &newton_set,
 &newton_iterate,
 &newton_free};

