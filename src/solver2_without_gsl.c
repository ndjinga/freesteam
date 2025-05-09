/*
freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2009  John Pye

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#define FREESTEAM_BUILDING_LIB
#include "solver2.h"

#include "region1.h"
#include "region2.h"
#include "region3.h"
#include "region4.h"
#include "b23.h"

#include "derivs.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <multiroots.h>

/*------------------------------------------------------------------------------
  LOOKUP FOR APPROPRIATE PROPERTY EVALUATION FUNCTION
*/

typedef double PropertyFunction(double, double);

static PropertyFunction *solver2_region3_propfn(FREESTEAM_CHAR A){
	switch(A){
		case 'p': return &freesteam_region3_p_rhoT;
		case 'u': return &freesteam_region3_u_rhoT;
		case 's': return &freesteam_region3_s_rhoT;
		case 'h': return &freesteam_region3_h_rhoT;
		default: return 0;
	}
}

/* this function is a needed bit of a redundancy with solver2 in region 4 */
static double solver2_region4_p_Tx(double T, double x){
	(void)x;
	return freesteam_region4_psat_T(T);
}

static PropertyFunction *solver2_region4_propfn(FREESTEAM_CHAR A){
	switch(A){
		case 'p': return &solver2_region4_p_Tx;
		case 'u': return &freesteam_region4_u_Tx;
		case 's': return &freesteam_region4_s_Tx;
		case 'h': return &freesteam_region4_h_Tx;
		case 'v': return &freesteam_region4_v_Tx;
		default: return 0;
	}
}

/* completely unnecessary... */
static double solver2_region2_p_pT(double p, double T){
	(void)T;
	return p;
}

static PropertyFunction *solver2_region2_propfn(FREESTEAM_CHAR A){
	switch(A){
		case 'p': return &solver2_region2_p_pT;
		case 'u': return &freesteam_region2_u_pT;
		case 's': return &freesteam_region2_s_pT;
		case 'h': return &freesteam_region2_h_pT;
		case 'v': return &freesteam_region2_v_pT;
		default: return 0;
	}
}

/* completely unnecessary... */
static double solver2_region1_p_pT(double p, double T){
	(void)T;
	return p;
}

static PropertyFunction *solver2_region1_propfn(FREESTEAM_CHAR A){
	switch(A){
		case 'p': return &solver2_region1_p_pT;
		case 'u': return &freesteam_region1_u_pT;
		case 's': return &freesteam_region1_s_pT;
		case 'h': return &freesteam_region1_h_pT;
		case 'v': return &freesteam_region1_v_pT;
		default: return 0;
	}
}


typedef struct{
	FREESTEAM_CHAR A,B;
	PropertyFunction *Afn, *Bfn;
	double a,b;
} Solver2Data;

/*------------------------------------------------------------------------------
  REGION 3
*/

static int region3_f(const double *x, void *user_data, double *f){
#define D ((Solver2Data *)user_data)
	double rho = x[0];
	double T   = x[1];
	f[0]= (*(D->Afn))(rho,T) - (D->a);
	f[1]= (*(D->Bfn))(rho,T) - (D->b);
	return EXIT_SUCCESS;
#undef D
}

static int region3_df(const double *x, void *user_data, double *J){
#define D ((Solver2Data *)user_data)
	double rho = x[0];
	double T   = x[1];
	SteamState S = freesteam_region3_set_rhoT(rho,T);
	J[0]= -1./SQ(rho)*freesteam_region3_dAdvT(D->A,S);
	J[2]= freesteam_region3_dAdTv(D->A,S);
	J[1]= -1./SQ(rho)*freesteam_region3_dAdvT(D->B,S);
	J[3]= freesteam_region3_dAdTv(D->B,S);
	/*  Print matrix J
	fprintf(stderr,"\n region3_df : Print matrix J D->A= %d, D->B= %d: rho=%f, T=%f\n",D->A,D->B, rho, T);
	int n=2;
	for (size_t i = 0; i < n; i++)
	    for (size_t j = 0; j < n; j++)
	      fprintf(stderr,"%f, ", J[i+ n*j]);
	fprintf(stderr,"\n end matrix J\n "); 
	*/
	return EXIT_SUCCESS;
#undef D
}

static int region3_fdf(const double *x, void *user_data, double *f, double *J){
	return region3_f(x, user_data, f) || region3_df(x, user_data, J);
}

#if 0
static void region3_print_state(size_t iter, gsl_multiroot_fdfsolver *s){
	double rho = gsl_vector_get(s->x,0);
	double T = gsl_vector_get(s->x,1);
	fprintf(stderr,"iter = %lu: rho = %g, T = %g\n", iter,rho,T);
}
#endif

SteamState freesteam_solver2_region3(FREESTEAM_CHAR A, FREESTEAM_CHAR B, double atarget, double btarget, SteamState guess, int *retstatus){
	const gsl_multiroot_fdfsolver_type *T;
	gsl_multiroot_fdfsolver *s;
	int status;
	size_t iter = 0;
	size_t max_iter = 50;
	const size_t n = 2;

	Solver2Data D = {A,B,solver2_region3_propfn(A), solver2_region3_propfn(B), atarget,btarget};

	gsl_multiroot_function_fdf f = {&region3_f, &region3_df, &region3_fdf, n, &D};

	double *x = malloc (n*sizeof(double));
	x[0]= freesteam_rho(guess);
	x[1]= freesteam_T(guess);
	T = gsl_multiroot_fdfsolver_gnewton;
	s = gsl_multiroot_fdfsolver_alloc(T, n);
	gsl_multiroot_fdfsolver_set(s, &f, x,n);
	//region3_print_state(iter, s);

	do{
		iter++;
		status = gsl_multiroot_fdfsolver_iterate(s);
		//region3_print_state(iter, s);
		if(status){
			/* check if solver is stuck */
                        fprintf(stderr,"!!!Error Newton algorithm : Iteration %lu failed,%s (%s:%d): %s: \n",(long unsigned)iter, __func__,__FILE__,__LINE__,strerror(status));
			break;
		}
		status = gsl_multiroot_test_residual(s->f, 2e-6, n);
	} while(status == EXIT_FAILURE && iter < max_iter);

	if( iter == max_iter )
	    fprintf(stderr,"\n!!!Newton algorithm : Maximum iteration number reached : iter = %lu ,max_iter = %lu ,%s (%s:%d) \n",(long unsigned)iter,(long unsigned)max_iter, __func__,__FILE__,__LINE__);
	//else if(!status)
	//    fprintf(stderr,"\n Newton algorithm succeeded in %lu iterations.\n ", iter);
   	
	SteamState S = freesteam_region3_set_rhoT(s->x[0], s->x[1]);
	gsl_multiroot_fdfsolver_free(s);
	free(x);
	*retstatus = status;
	//freesteam_fprint(stderr,S);
	return S;
}

/*------------------------------------------------------------------------------
  REGION 4
*/

static int region4_f(const double *X, void *user_data, double *f){
#define D ((Solver2Data *)user_data)
	double T = X[0];
	double x = X[1];
	f[0]= (*(D->Afn))(T,x) - (D->a);
	f[1]= (*(D->Bfn))(T,x) - (D->b);
	return EXIT_SUCCESS;
#undef D
}

static int region4_df(const double *x, void *user_data, double *J){
#define D ((Solver2Data *)user_data)
	SteamState S = freesteam_region4_set_Tx(x[0],x[1]);
	J[0]= freesteam_region4_dAdTx(D->A,S);
	J[1]= freesteam_region4_dAdxT(D->A,S);
	J[2]= freesteam_region4_dAdTx(D->B,S);
	J[3]= freesteam_region4_dAdxT(D->B,S);
	return EXIT_SUCCESS;
#undef D
}

static int region4_fdf(const double *x, void *user_data, double *f, double *J){
	return region4_f(x, user_data, f) || region4_df(x, user_data, J);
}

#if 0
static void region4_print_state(size_t iter, gsl_multiroot_fdfsolver *s){
	double T = gsl_vector_get(s->x,0);
	double x = gsl_vector_get(s->x,1);
	fprintf(stderr,"iter = %lu: T = %g, x = %g\n", iter,T,x);
}
#endif

SteamState freesteam_solver2_region4(FREESTEAM_CHAR A, FREESTEAM_CHAR B, double atarget, double btarget, SteamState guess, int *retstatus){
	const gsl_multiroot_fdfsolver_type *T;
	gsl_multiroot_fdfsolver *s;
	int status;
	size_t iter = 0;
	size_t max_iter = 20;
	const size_t n = 2;

	Solver2Data D = {A,B,solver2_region4_propfn(A), solver2_region4_propfn(B), atarget,btarget};

	gsl_multiroot_function_fdf f = {&region4_f, &region4_df, &region4_fdf, n, &D};

	double *x = malloc (n*sizeof(double));
	assert(guess.region==4);
	x[0]= guess.R4.T;
	x[1]= guess.R4.x;
	T = gsl_multiroot_fdfsolver_gnewton;
	s = gsl_multiroot_fdfsolver_alloc(T, n);
	gsl_multiroot_fdfsolver_set(s, &f, x,n);
	//region4_print_state(iter, s);

	do{
		iter++;
		status = gsl_multiroot_fdfsolver_iterate(s);
		//region4_print_state(iter, s);
		if(status){
			/* check if solver is stuck */
                        fprintf(stderr,"!!!Newton algorithm : Iteration %lu failed,%s (%s:%d): %s: ",(long unsigned)iter, __func__,__FILE__,__LINE__,strerror(status));
			break;
		}
		status = gsl_multiroot_test_residual(s->f, 1e-7, n);
	} while(status == EXIT_SUCCESS && iter < max_iter);

	if( iter == max_iter )
	    fprintf(stderr,"!!!Newton algorithm : Maximum iteration number reached : iter = %lu ,max_iter = %lu ,%s (%s:%d) ",(long unsigned)iter,(long unsigned)max_iter, __func__,__FILE__,__LINE__);

	fprintf(stderr,"status = %s\n", strerror (status));
	SteamState S = freesteam_region4_set_Tx(s->x[0], s->x[1]);
	gsl_multiroot_fdfsolver_free(s);

	free(x);
	*retstatus = status;
	if(status)fprintf(stderr,"%s (%s:%d): %s: ",__func__,__FILE__,__LINE__,strerror(status));
	//freesteam_fprint(stderr,S);
	return S;
}

/*------------------------------------------------------------------------------
  REGION 2
*/

static int region2_f(const double *x, void *user_data, double *f){
#define D ((Solver2Data *)user_data)
	double p = x[0];
	double T = x[1];
	f[0]= (*(D->Afn))(p,T) - (D->a);
	f[1]= (*(D->Bfn))(p,T) - (D->b);
	return EXIT_SUCCESS;
#undef D
}

static int region2_df(const double *x, void *user_data, double *J){
#define D ((Solver2Data *)user_data)
	double p = x[0];
	double T = x[1];
	SteamState S = freesteam_region2_set_pT(p,T);
	J[0]= freesteam_region2_dAdpT(D->A,S);
	J[1]= freesteam_region2_dAdTp(D->A,S);
	J[2]= freesteam_region2_dAdpT(D->B,S);
	J[3]= freesteam_region2_dAdTp(D->B,S);
	return EXIT_SUCCESS;
#undef D
}

static int region2_fdf(const double *x, void *user_data, double *f, double *J){
	return region2_f(x, user_data, f) || region2_df(x, user_data, J);
}

static void region2_print_state(size_t iter, gsl_multiroot_fdfsolver *s){
	double p = s->x[0];
	double T = s->x[1];
	fprintf(stderr,"iter = %lu: p = %g, T = %g\n", (long unsigned)iter,p,T);
}

SteamState freesteam_solver2_region2(FREESTEAM_CHAR A, FREESTEAM_CHAR B, double atarget, double btarget, SteamState guess, int *retstatus){
	const gsl_multiroot_fdfsolver_type *T;
	gsl_multiroot_fdfsolver *s;
	int status;
	size_t iter = 0;
	size_t max_iter = 20;
	const size_t n = 2;

	fprintf(stderr,"region 2 solver...\n");
	Solver2Data D = {A,B,solver2_region2_propfn(A), solver2_region2_propfn(B), atarget,btarget};

	gsl_multiroot_function_fdf f = {&region2_f, &region2_df, &region2_fdf, n, &D};

	double *x = malloc (n*sizeof(double));
	x[0]= freesteam_rho(guess);
	x[1]= freesteam_T(guess);
	T = gsl_multiroot_fdfsolver_gnewton;
	s = gsl_multiroot_fdfsolver_alloc(T, n);
	gsl_multiroot_fdfsolver_set(s, &f, x,n);
	region2_print_state(iter, s);

	do{
		iter++;
		status = gsl_multiroot_fdfsolver_iterate(s);
		region2_print_state(iter, s);
		if(status){
			/* check if solver is stuck */
                        fprintf(stderr,"!!!Newton algorithm : Iteration %lu failed,%s (%s:%d): %s: ",(long unsigned)iter, __func__,__FILE__,__LINE__,strerror(status));
			break;
		}
		status = gsl_multiroot_test_residual(s->f, 1e-7, n);
	} while(status == EXIT_SUCCESS && iter < max_iter);

	if( iter == max_iter )
	    fprintf(stderr,"!!!Newton algorithm : Maximum iteration number reached : iter = %lu ,max_iter = %lu ,%s (%s:%d) ",(long unsigned)iter,(long unsigned)max_iter, __func__,__FILE__,__LINE__);

	SteamState S = freesteam_region2_set_pT(s->x[0], s->x[1]);
	gsl_multiroot_fdfsolver_free(s);

	free(x);
	*retstatus = status;
	if(status)fprintf(stderr,"%s (%s:%d): %s: ",__func__,__FILE__,__LINE__,strerror(status));
	//freesteam_fprint(stderr,S);
	return S;
}

/*------------------------------------------------------------------------------
  REGION 1
*/

static int region1_f(const double *x, void *user_data, double *f){
#define D ((Solver2Data *)user_data)
	double p = x[0];
	double T = x[1];
	f[0]= (*(D->Afn))(p,T) - (D->a);
	f[1]= (*(D->Bfn))(p,T) - (D->b);
	return EXIT_SUCCESS;
#undef D
}

static int region1_df(const double *x, void *user_data, double *J){
#define D ((Solver2Data *)user_data)
	double p = x[0];
	double T = x[1];
	SteamState S = freesteam_region1_set_pT(p,T);
	J[0]= freesteam_region1_dAdpT(D->A,S);
	J[1]= freesteam_region1_dAdTp(D->A,S);
	J[2]= freesteam_region1_dAdpT(D->B,S);
	J[3]= freesteam_region1_dAdTp(D->B,S);
	return EXIT_SUCCESS;
#undef D
}

static int region1_fdf(const double *x, void *user_data, double *f, double *J){
	return region1_f(x, user_data, f) || region1_df(x, user_data, J);
}

#if 0
static void region1_print_state(size_t iter, gsl_multiroot_fdfsolver *s){
	double p = gsl_vector_get(s->x,0);
	double T = gsl_vector_get(s->x,1);
	fprintf(stderr,"iter = %lu: p = %g, T = %g\n", iter,p,T);
}
#endif

SteamState freesteam_solver2_region1(FREESTEAM_CHAR A, FREESTEAM_CHAR B, double atarget, double btarget, SteamState guess, int *retstatus){
	const gsl_multiroot_fdfsolver_type *T;
	gsl_multiroot_fdfsolver *s;
	int status;
	size_t iter = 0;
	size_t max_iter = 20;
	const size_t n = 2;


	//fprintf(stderr,"region 1 solver...\n");
	Solver2Data D = {A,B,solver2_region1_propfn(A), solver2_region1_propfn(B), atarget,btarget};

	gsl_multiroot_function_fdf f = {&region1_f, &region1_df, &region1_fdf, n, &D};

	double *x = malloc (n*sizeof(double));
	x[0]= freesteam_rho(guess);
	x[1]= freesteam_T(guess);
	T = gsl_multiroot_fdfsolver_gnewton;
	s = gsl_multiroot_fdfsolver_alloc(T, n);
	gsl_multiroot_fdfsolver_set(s, &f, x,n);
	//region1_print_state(iter, s);

	do{
		iter++;
		status = gsl_multiroot_fdfsolver_iterate(s);
		//region1_print_state(iter, s);
		if(status){
			/* check if solver is stuck */
                        fprintf(stderr,"!!!Newton algorithm : Iteration %lu failed,%s (%s:%d): %s: ",(long unsigned)iter, __func__,__FILE__,__LINE__,strerror(status));
			break;
		}
		status = gsl_multiroot_test_residual(s->f, 1e-6,n);
	} while(status == EXIT_SUCCESS && iter < max_iter);

	if( iter == max_iter )
	    fprintf(stderr,"!!!Newton algorithm : Maximum iteration number reached : iter = %lu ,max_iter = %lu ,%s (%s:%d) ",(long unsigned)iter,(long unsigned)max_iter, __func__,__FILE__,__LINE__);

	SteamState S = freesteam_region1_set_pT(s->x[0], s->x[1]);
	gsl_multiroot_fdfsolver_free(s);

	free(x);
	*retstatus = status;
	if(status){
		fprintf(stderr,"%s (%s:%d): %s: ",__func__,__FILE__,__LINE__,strerror(status));
		freesteam_fprint(stderr,S);
	}
	return S;
}

