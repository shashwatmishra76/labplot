/***************************************************************************
    File                 : nsl_sf_basic.h
    Project              : LabPlot
    Description          : NSL special basic functions
    --------------------------------------------------------------------
    Copyright            : (C) 2017 by Stefan Gerlach (stefan.gerlach@uni.kn)

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/

#ifndef NSL_SF_BASIC_H
#define NSL_SF_BASIC_H

#include <stdlib.h>

/* stdlib.h */
double nsl_sf_rand() { return rand(); }
double nsl_sf_random() { return random(); }
double nsl_sf_drand() { return random()/(double)RAND_MAX; }

/* math.h */
double nsl_sf_sgn(double x) {
#ifndef _WIN32
	return copysign(1.0, x);
#else
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	else
		return 0;
#endif
}
double nsl_sf_theta(double x) {
	if (x >= 0)
		return 1;
	else
		return 0;
}

/* missing trig. functions */
double nsl_sf_sec(double x) { return 1./cos(x); }
double nsl_sf_csc(double x) { return 1./sin(x); }
double nsl_sf_cot(double x) { return 1./tan(x); }
double nsl_sf_asec(double x) { return acos(1./x); }
double nsl_sf_acsc(double x) { return asin(1./x); }
double nsl_sf_acot(double x) {
	if (x > 0)
		return atan(1./x);
	else
		return atan(1./x) + M_PI;
}
double nsl_sf_sech(double x) { return 1./cosh(x); }
double nsl_sf_csch(double x) { return 1./sinh(x); }
double nsl_sf_coth(double x) { return 1./tanh(x); }
double nsl_sf_asech(double x) { return gsl_acosh(1./x); }
double nsl_sf_acsch(double x) { return gsl_asinh(1./x); }
double nsl_sf_acoth(double x) { return gsl_atanh(1./x); }

/* wrapper for GSL functions with integer parameters */
#define MODE GSL_PREC_DOUBLE
/* mathematical functions */
double nsl_sf_ldexp(double x, double expo) { return gsl_ldexp(x, round(expo)); }
double nsl_sf_powint(double x, double n) { return gsl_sf_pow_int(x, round(n)); }
/* Airy functions */
double nsl_sf_airy_Ai(double x) { return gsl_sf_airy_Ai(x, MODE); }
double nsl_sf_airy_Bi(double x) { return gsl_sf_airy_Bi(x, MODE); }
double nsl_sf_airy_Ais(double x) { return gsl_sf_airy_Ai_scaled(x, MODE); }
double nsl_sf_airy_Bis(double x) { return gsl_sf_airy_Bi_scaled(x, MODE); }
double nsl_sf_airy_Aid(double x) { return gsl_sf_airy_Ai_deriv(x, MODE); }
double nsl_sf_airy_Bid(double x) { return gsl_sf_airy_Bi_deriv(x, MODE); }
double nsl_sf_airy_Aids(double x) { return gsl_sf_airy_Ai_deriv_scaled(x, MODE); }
double nsl_sf_airy_Bids(double x) { return gsl_sf_airy_Bi_deriv_scaled(x, MODE); }
double nsl_sf_airy_0_Ai(double s) { return gsl_sf_airy_zero_Ai(round(s)); }
double nsl_sf_airy_0_Bi(double s) { return gsl_sf_airy_zero_Bi(round(s)); }
double nsl_sf_airy_0_Aid(double s) { return gsl_sf_airy_zero_Ai_deriv(round(s)); }
double nsl_sf_airy_0_Bid(double s) { return gsl_sf_airy_zero_Bi_deriv(round(s)); }
/* Bessel functions */
double nsl_sf_bessel_Jn(double n, double x) { return gsl_sf_bessel_Jn(round(n), x); }
double nsl_sf_bessel_Yn(double n, double x) { return gsl_sf_bessel_Yn(round(n), x); }
double nsl_sf_bessel_In(double n, double x) { return gsl_sf_bessel_In(round(n), x); }
double nsl_sf_bessel_Ins(double n, double x) { return gsl_sf_bessel_In_scaled(round(n), x); }
double nsl_sf_bessel_Kn(double n, double x) { return gsl_sf_bessel_Kn(round(n), x); }
double nsl_sf_bessel_Kns(double n, double x) { return gsl_sf_bessel_Kn_scaled(round(n), x); }
double nsl_sf_bessel_jl(double l, double x) { return gsl_sf_bessel_jl(round(l), x); }
double nsl_sf_bessel_yl(double l, double x) { return gsl_sf_bessel_yl(round(l), x); }
double nsl_sf_bessel_ils(double l, double x) { return gsl_sf_bessel_il_scaled(round(l), x); }
double nsl_sf_bessel_kls(double l, double x) { return gsl_sf_bessel_kl_scaled(round(l), x); }
double nsl_sf_bessel_0_J0(double s) { return gsl_sf_bessel_zero_J0(round(s)); }
double nsl_sf_bessel_0_J1(double s) { return gsl_sf_bessel_zero_J1(round(s)); }
double nsl_sf_bessel_0_Jnu(double nu, double s) { return gsl_sf_bessel_zero_Jnu(nu, round(s)); }

double nsl_sf_hydrogenicR(double n, double l, double z, double r) { return gsl_sf_hydrogenicR(round(n), round(l), z, r); }
/* elliptic integrals */
double nsl_sf_ellint_Kc(double x) { return gsl_sf_ellint_Kcomp(x, MODE); }
double nsl_sf_ellint_Ec(double x) { return gsl_sf_ellint_Ecomp(x, MODE); }
double nsl_sf_ellint_Pc(double x, double n) { return gsl_sf_ellint_Pcomp(x, n, MODE); }
double nsl_sf_ellint_F(double phi, double k) { return gsl_sf_ellint_F(phi, k, MODE); }
double nsl_sf_ellint_E(double phi, double k) { return gsl_sf_ellint_E(phi, k, MODE); }
double nsl_sf_ellint_P(double phi, double k, double n) { return gsl_sf_ellint_P(phi, k, n, MODE); }
double nsl_sf_ellint_D(double phi, double k) {
#if GSL_MAJOR_VERSION >= 2
	return gsl_sf_ellint_D(phi,k,MODE);
#else
	return gsl_sf_ellint_D(phi,k,0.0,MODE);
#endif
}
double nsl_sf_ellint_RC(double x, double y) { return gsl_sf_ellint_RC(x, y, MODE); }
double nsl_sf_ellint_RD(double x, double y, double z) { return gsl_sf_ellint_RD(x, y, z, MODE); }
double nsl_sf_ellint_RF(double x, double y, double z) { return gsl_sf_ellint_RF(x, y, z, MODE); }
double nsl_sf_ellint_RJ(double x, double y, double z, double p) { return gsl_sf_ellint_RJ(x, y, z, p, MODE); }

double nsl_sf_exprel_n(double n, double x) { return gsl_sf_exprel_n(round(n), x); }
double nsl_sf_fermi_dirac_int(double j, double x) { return gsl_sf_fermi_dirac_int(round(j), x); }
/* gamma */
double nsl_sf_fact(double n) { return gsl_sf_fact(round(n)); }
double nsl_sf_doublefact(double n) { return gsl_sf_doublefact(round(n)); }
double nsl_sf_lnfact(double n) { return gsl_sf_lnfact(round(n)); }
double nsl_sf_lndoublefact(double n) { return gsl_sf_lndoublefact(round(n)); }
double nsl_sf_choose(double n, double m) { return gsl_sf_choose(round(n), round(m)); }
double nsl_sf_lnchoose(double n, double m) { return gsl_sf_lnchoose(round(n), round(m)); }
double nsl_sf_taylorcoeff(double n, double x) { return gsl_sf_taylorcoeff(round(n), x); }

double nsl_sf_gegenpoly_n(double n, double l, double x) { return gsl_sf_gegenpoly_n(round(n), l, x); }
double nsl_sf_hyperg_1F1i(double m, double n, double x) { return gsl_sf_hyperg_1F1_int(round(m), round(n), x); }
double nsl_sf_hyperg_Ui(double m, double n, double x) { return gsl_sf_hyperg_U_int(round(m), round(n), x); }
double nsl_sf_laguerre_n(double n, double a, double x) { return gsl_sf_laguerre_n(round(n), a, x); }

double nsl_sf_legendre_Pl(double l, double x) { return gsl_sf_legendre_Pl(round(l), x); }
double nsl_sf_legendre_Ql(double l, double x) { return gsl_sf_legendre_Ql(round(l), x); }
double nsl_sf_legendre_Plm(double l, double m, double x) { return gsl_sf_legendre_Plm(round(l), round(m), x); }
double nsl_sf_legendre_sphPlm(double l, double m, double x) { return gsl_sf_legendre_sphPlm(round(l), round(m), x); }
double nsl_sf_conicalP_sphreg(double l, double L, double x) { return gsl_sf_conicalP_sph_reg(round(l), L, x); }
double nsl_sf_conicalP_cylreg(double m, double l, double x) { return gsl_sf_conicalP_sph_reg(round(m), l, x); }
double nsl_sf_legendre_H3d(double l,  double L, double e) { return gsl_sf_legendre_H3d(round(l), L, e); }

double nsl_sf_psiint(double n) { return gsl_sf_psi_int(round(n)); }
double nsl_sf_psi1int(double n) { return gsl_sf_psi_1_int(round(n)); }
double nsl_sf_psin(double n,  double x) { return gsl_sf_psi_n(round(n), x); }

double nsl_sf_zetaint(double n) { return gsl_sf_zeta_int(round(n)); }
double nsl_sf_zetam1int(double n) { return gsl_sf_zetam1_int(round(n)); }
double nsl_sf_etaint(double n) { return gsl_sf_eta_int(round(n)); }

/* random number distributions */
double nsl_sf_poisson(double k, double m) { return gsl_ran_poisson_pdf(round(k), m); }
double nsl_sf_bernoulli(double k, double p) { return gsl_ran_bernoulli_pdf(round(k), p); }
double nsl_sf_binomial(double k, double p, double n) { return gsl_ran_binomial_pdf(round(k), p, round(n)); }
double nsl_sf_negative_binomial(double k, double p, double n) { return gsl_ran_negative_binomial_pdf(round(k), p, n); }
double nsl_sf_pascal(double k, double p, double n) { return gsl_ran_pascal_pdf(round(k), p, round(n)); }
double nsl_sf_geometric(double k, double p) { return gsl_ran_geometric_pdf(round(k), p); }
double nsl_sf_hypergeometric(double k, double n1, double n2, double t) {
	return gsl_ran_hypergeometric_pdf(round(k), round(n1), round(n2), round(t));
}
double nsl_sf_logarithmic(double k, double p) { return gsl_ran_logarithmic_pdf(round(k), p); }

#endif /* NSL_SF_BASIC_H */
