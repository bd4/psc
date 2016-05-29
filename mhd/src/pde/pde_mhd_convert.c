
#include "ggcm_mhd_private.h"

#define TINY_NUMBER 1.0e-20 // FIXME

// ----------------------------------------------------------------------
// mhd_prim_from_fc

static void _mrc_unused
mhd_prim_from_fc(struct ggcm_mhd *mhd, fld1d_state_t W, fld1d_state_t U,
		 int ldim, int l, int r)
{
  mrc_fld_data_t gamma_minus_1 = mhd->par.gamm - 1.;

  for (int i = -l; i < ldim + r; i++) {
    mrc_fld_data_t *w = &F1S(W, 0, i), *u = &F1S(U, 0, i);

    mrc_fld_data_t rri = 1. / u[RR];
    w[RR] = u[RR];
    w[VX] = u[RVX] * rri;
    w[VY] = u[RVY] * rri;
    w[VZ] = u[RVZ] * rri;
    w[PP] = gamma_minus_1 * (u[EE] 
			     - .5 * (sqr(u[RVX]) + sqr(u[RVY]) + sqr(u[RVZ])) * rri
			     - .5 * (sqr(u[BX]) + sqr(u[BY]) + sqr(u[BZ])));
    w[PP] = fmax(w[PP], TINY_NUMBER);
    w[BX] = u[BX];
    w[BY] = u[BY];
    w[BZ] = u[BZ];
  }
}

// ----------------------------------------------------------------------
// mhd_fc_from_prim

static void _mrc_unused
mhd_fc_from_prim(struct ggcm_mhd *mhd, struct mrc_fld *U_cc, struct mrc_fld *W_cc,
		    int ldim, int l, int r)
{
  mrc_fld_data_t gamma_minus_1 = mhd->par.gamm - 1.;

  for (int i = -l; i < ldim + r; i++) {
    mrc_fld_data_t *U = &F1(U_cc, 0, i), *W = &F1(W_cc, 0, i);

    mrc_fld_data_t rr = W[RR];
    U[RR ] = rr;
    U[RVX] = rr * W[VX];
    U[RVY] = rr * W[VY];
    U[RVZ] = rr * W[VZ];
    U[EE ] = 
      W[PP] / gamma_minus_1 +
      + .5 * (sqr(W[VX]) + sqr(W[VY]) + sqr(W[VZ])) * rr
      + .5 * (sqr(W[BX]) + sqr(W[BY]) + sqr(W[BZ]));
    U[BX ] = W[BX];
    U[BY ] = W[BY];
    U[BZ ] = W[BZ];
  }
}

// ----------------------------------------------------------------------
// mhd_prim_from_sc

static void _mrc_unused
mhd_prim_from_sc(struct ggcm_mhd *mhd, fld1d_state_t W, fld1d_state_t U,
		 int ldim, int l, int r)
{
  mrc_fld_data_t gamma_minus_1 = mhd->par.gamm - 1.;

  for (int i = -l; i < ldim + r; i++) {
    mrc_fld_data_t *w = &F1S(W, 0, i), *u = &F1S(U, 0, i);

    mrc_fld_data_t rri = 1. / u[RR];
    w[RR] = u[RR];
    w[VX] = rri * u[RVX];
    w[VY] = rri * u[RVY];
    w[VZ] = rri * u[RVZ];
    mrc_fld_data_t rvv = (sqr(u[RVX]) + sqr(u[RVY]) + sqr(u[RVZ])) * rri;
    w[PP] = gamma_minus_1 * (u[UU] - .5 * rvv);
    w[PP] = fmax(w[PP], TINY_NUMBER);
  }
}

// ----------------------------------------------------------------------
// mhd_sc_from_prim

static void _mrc_unused
mhd_sc_from_prim(struct ggcm_mhd *mhd, fld1d_state_t U, fld1d_state_t W,
		 int ldim, int l, int r)
{
  mrc_fld_data_t gamma_minus_1 = mhd->par.gamm - 1.;

  for (int i = -l; i < ldim + r; i++) {
    mrc_fld_data_t *u = &F1S(U, 0, i), *w = &F1S(W, 0, i);

    mrc_fld_data_t rr = w[RR];
    u[RR ] = rr;
    u[RVX] = rr * w[VX];
    u[RVY] = rr * w[VY];
    u[RVZ] = rr * w[VZ];
    u[UU ] = 
      w[PP] / gamma_minus_1 +
      + .5 * (sqr(w[VX]) + sqr(w[VY]) + sqr(w[VZ])) * rr;
  }
}

