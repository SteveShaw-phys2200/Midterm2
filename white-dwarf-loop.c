/*
 * The program solves the following system of two 
 * first order differential equations, which describe 
 * the radial mass distribution of white dwarf star
 * 
 * The step-size of the integrator is automatically 
 * adjusted by the controller to maintain the 
 * requested accuracy
 */

#include <stdio.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

int dwarf_eqs (double r, const double y[], double f[], void *params);

int main (void) 
{
	size_t neqs = 2;          /* number of equations */
	double eps_abs = 1.e-8, 
	eps_rel = 0.;           /* desired precision */
	double stepsize = 1e-6;   /* initial integration step */
	int status;
	double rho_c = 10.;       /* the central density */
	double r = 0., r1 = 1200.; /* start at center r=0 */
	int np = 100;				
	double rho_min = .08;
	double rho_max = 1000000;
	double step = exp(log((rho_max - rho_min)) /(np -1));

	for (int i = 0; i < np; i++)
	{
		if (i==0)
		{
			rho_c = rho_min * step;
		}
		r = 0;
		rho_c = rho_min * pow(step, i);
		/* 
	 	* Initial conditions 
	 	*/
		double y[2] = { 0, rho_c };   /* for res1 */
		/*
	 	* Explicit embedded Runge-Kutta-Fehlberg (4,5) method. 
	 	* This method is a good general-purpose integrator.
	 	*/
		gsl_odeiv2_step    *s = gsl_odeiv2_step_alloc 
						(gsl_odeiv2_step_rkf45, neqs);
		gsl_odeiv2_control *c = gsl_odeiv2_control_y_new (eps_abs,			    						eps_rel);
		gsl_odeiv2_evolve  *e = gsl_odeiv2_evolve_alloc (neqs);
    
		gsl_odeiv2_system sys = {dwarf_eqs, NULL, neqs, &rho_c};

		/*
		 * Evolution loop 
		 */
		while ( (r < r1) && (y[1] > 0) )
		{
			status = gsl_odeiv2_evolve_apply (e, c, s, &sys, &r, 
					r1, &stepsize, y);
	
			if (status != GSL_SUCCESS) {
				printf ("Troubles: % .5e  % .5e  % .5e\n", r, y[0], y[1]);
				break;
			}
		}
		printf ("% .5e  % .5e  % .5e\n", r, y[0], y[1]);
		gsl_odeiv2_evolve_free (e);
		gsl_odeiv2_control_free (c);
		gsl_odeiv2_step_free (s);
	}
	return 0;
}
