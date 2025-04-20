/* multiroots/test_funcs.h
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


typedef void (*initpt_function) (double * x);

extern gsl_multiroot_function_fdf rosenbrock;
void rosenbrock_initpt (double * x);
int rosenbrock_f (const double * x, void *params, double * f);
int rosenbrock_df (const double * x, void *params, double * df);
int rosenbrock_fdf (const double * x, void *params, double * f, double * df);

extern gsl_multiroot_function_fdf roth;
void roth_initpt (double * x);
int roth_f (const double * x, void *params, double * f);
int roth_df (const double * x, void *params, double * df);
int roth_fdf (const double * x, void *params, double * f, double * df);

extern gsl_multiroot_function_fdf brownscal;
void brownscal_initpt (double * x);
int brownscal_f (const double * x, void *params, double * f);
int brownscal_df (const double * x, void *params, double * df);
int brownscal_fdf (const double * x, void *params, double * f, double * df);

extern gsl_multiroot_function_fdf powellscal;
void powellscal_initpt (double * x);
int powellscal_f (const double * x, void *params, double * f);
int powellscal_df (const double * x, void *params, double * df);
int powellscal_fdf (const double * x, void *params, double * f, double * df);

extern gsl_multiroot_function_fdf powellsing;
void powellsing_initpt (double * x);
int powellsing_f (const double * x, void *params, double * f);
int powellsing_df (const double * x, void *params, double * df);
int powellsing_fdf (const double * x, void *params, double * f, double * df);

extern gsl_multiroot_function_fdf wood;
void wood_initpt (double * x);
int wood_f (const double * x, void *params, double * f);
int wood_df (const double * x, void *params, double * df);
int wood_fdf (const double * x, void *params, double * f, double * df);

extern gsl_multiroot_function_fdf helical;
void helical_initpt (double * x);
int helical_f (const double * x, void *params, double * f);
int helical_df (const double * x, void *params, double * df);
int helical_fdf (const double * x, void *params, double * f, double * df);

extern gsl_multiroot_function_fdf dbv;
void dbv_initpt (double * x);
int dbv_f (const double * x, void *params, double * f);
int dbv_df (const double * x, void *params, double * df);
int dbv_fdf (const double * x, void *params, double * f, double * df);

extern gsl_multiroot_function_fdf trig;
void trig_initpt (double * x);
int trig_f (const double * x, void *params, double * f);
int trig_df (const double * x, void *params, double * df);
int trig_fdf (const double * x, void *params, double * f, double * df);

