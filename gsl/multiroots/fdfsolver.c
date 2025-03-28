/* multiroots/fdfsolver.c
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

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <multiroots.h>

gsl_multiroot_fdfsolver *
gsl_multiroot_fdfsolver_alloc (const gsl_multiroot_fdfsolver_type * T, 
                                       size_t n)
{
  int status;

  gsl_multiroot_fdfsolver * s;

  s = (gsl_multiroot_fdfsolver *) malloc (sizeof (gsl_multiroot_fdfsolver));

  if (s == 0)
    {
      perror ("failed to allocate space for multiroot solver struct");
    }

  s->x = calloc (n,sizeof(double));//gsl_vector_calloc

  if (s->x == 0) 
    {
      free (s);
      perror ("failed to allocate space for x");
    }

  s->f = calloc (n,sizeof(double));//gsl_vector_calloc

  if (s->f == 0) 
    {
      free (s->x);
      free (s);
      perror ("failed to allocate space for f");
    }

  s->J = calloc (n,n*sizeof(double));//gsl_matrix_calloc

  if (s->J == 0) 
    {
      free (s->x);
      free (s->f);
      free (s);
      perror ("failed to allocate space for g");
    }

  s->dx = calloc (n,sizeof(double));//gsl_vector_calloc

  if (s->dx == 0) 
    {
      free (s->J);
      free (s->x);
      free (s->f);
      free (s);
      perror ("failed to allocate space for dx");
    }

  s->state = malloc (T->size);

  if (s->state == 0)
    {
      free (s->dx);
      free (s->x);
      free (s->f);
      free (s->J);
      free (s);         /* exception in constructor, avoid memory leak */
      
      perror ("failed to allocate space for multiroot solver state");
    }

  s->type = T ;

  status = (s->type->alloc)(s->state, n);

  if (status != EXIT_SUCCESS)
    {
      free (s->state);
      free (s->dx);
      free (s->x);
      free (s->f);
      free (s->J);
      free (s);         /* exception in constructor, avoid memory leak */
      
      perror ("failed to set solver");
    }
  
  s->fdf = NULL;
  
  return s;
}

int
gsl_multiroot_fdfsolver_set (gsl_multiroot_fdfsolver * s, 
                             gsl_multiroot_function_fdf * f, 
                             const double * x)
{
  if (s->x->size != f->n)
    {
      perror ("function incompatible with solver size");
    }
  
  if (x->size != f->n) 
    {
      perror ("vector length not compatible with function");
    }  
    
  s->fdf = f;
  memcpy(s->x,x,s->x->size);
  
  return (s->type->set) (s->state, s->fdf, s->x, s->f, s->J, s->dx);
}

int
gsl_multiroot_fdfsolver_iterate (gsl_multiroot_fdfsolver * s)
{
  return (s->type->iterate) (s->state, s->fdf, s->x, s->f, s->J, s->dx);
}

void
gsl_multiroot_fdfsolver_free (gsl_multiroot_fdfsolver * s)
{
  RETURN_IF_NULL (s);
  (s->type->free) (s->state);
  free (s->state);
  free (s->dx);
  free (s->x);
  free (s->f);
  free (s->J);
  free (s);
}

const char *
gsl_multiroot_fdfsolver_name (const gsl_multiroot_fdfsolver * s)
{
  return s->type->name;
}

double *
gsl_multiroot_fdfsolver_root (const gsl_multiroot_fdfsolver * s)
{
  return s->x;
}

double *
gsl_multiroot_fdfsolver_dx (const gsl_multiroot_fdfsolver * s)
{
  return s->dx;
}

double *
gsl_multiroot_fdfsolver_f (const gsl_multiroot_fdfsolver * s)
{
  return s->f;
}
