/* multiroots/test.c
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
#include <stdlib.h>
#include <stdio.h>
#include <multiroots.h>

#include "test_funcs.h"
int test_fdf (const char * desc, gsl_multiroot_function_fdf * function, initpt_function initpt, double factor, const gsl_multiroot_fdfsolver_type * T);


int 
main (void)
{
  const gsl_multiroot_fdfsolver_type * fdfsolvers[5] ;
  const gsl_multiroot_fdfsolver_type ** T2 ;

  double f;

  fdfsolvers[0] = gsl_multiroot_fdfsolver_newton;
  fdfsolvers[1] = gsl_multiroot_fdfsolver_gnewton;
  fdfsolvers[2] = gsl_multiroot_fdfsolver_hybridj;
  fdfsolvers[3] = gsl_multiroot_fdfsolver_hybridsj;
  fdfsolvers[4] = 0;

  gsl_ieee_env_setup();


  f = 1.0 ;
  
  T2 = fdfsolvers ;
  
  while (*T2 != 0) 
    {
      test_fdf ("Rosenbrock", &rosenbrock, rosenbrock_initpt, f, *T2);
      test_fdf ("Roth", &roth, roth_initpt, f, *T2);
      test_fdf ("Powell badly scaled", &powellscal, powellscal_initpt, f, *T2);
      test_fdf ("Brown badly scaled", &brownscal, brownscal_initpt, f, *T2);
      test_fdf ("Powell singular", &powellsing, powellsing_initpt, f, *T2);
      test_fdf ("Wood", &wood, wood_initpt, f, *T2);
      test_fdf ("Helical", &helical, helical_initpt, f, *T2);
      test_fdf ("Discrete BVP", &dbv, dbv_initpt, f, *T2);
      test_fdf ("Trig", &trig, trig_initpt, f, *T2);
      T2++;
    }

  exit (gsl_test_summary ());
}

void scale (double * x, double factor);

void
scale (double * x, double factor)
{
  size_t i, n = x->size;

  if (gsl_vector_isnull(x))
    {
      for (i = 0; i < n; i++)
        {
          x[i] = factor;
        }
    }
  else
    {
      for (i = 0; i < n; i++)
        {
          double xi = x[i];
          x[i] = factor * xi;
        }
    } 
}

int
test_fdf (const char * desc, gsl_multiroot_function_fdf * function, 
          initpt_function initpt, double factor,
          const gsl_multiroot_fdfsolver_type * T)
{
  int status;
  double residual = 0;
  size_t i, n = function->n, iter = 0;
  
  double *x = malloc (n*sizeof(double));//gsl_vector_alloc (n);
  double *J = malloc (n*n*sizeof(double));//gsl_matrix_alloc (n, n);

  gsl_multiroot_fdfsolver *s;

  (*initpt) (x);

  if (factor != 1.0) scale(x, factor);

  s = gsl_multiroot_fdfsolver_alloc (T, n);
  gsl_multiroot_fdfsolver_set (s, function, x);
 
  do
    {
      iter++;
      status = gsl_multiroot_fdfsolver_iterate (s);
      
      if (status == EXIT_SUCCESS)
        break ;

      status = gsl_multiroot_test_residual (s->f, 0.0000001);
    }
  while (status == EXIT_SUCCESS && iter < 1000);

#ifdef DEBUG
  printf("x "); gsl_vector_fprintf (stdout, s->x, "%g"); printf("\n");
  printf("f "); gsl_vector_fprintf (stdout, s->f, "%g"); printf("\n");
#endif


  for (i = 0; i < n ; i++)
    {
      residual += fabs(s->f[i]);
    }

  gsl_multiroot_fdfsolver_free (s);
  free(J);
  free(x);

  gsl_test(status, "%s on %s (%g), %u iterations, residual = %.2g", T->name, desc, factor, iter, residual);

  return status;
}

