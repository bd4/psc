
#ifndef MRC_DDC_H
#define MRC_DDC_H

#include <mrc_common.h>
#include <mrc_obj.h>
#include <mrc_fld.h>

#include <mpi.h>

struct mrc_ddc_funcs {
  void (*copy_to_buf)(int mb, int me, int p, int ilo[3], int ihi[3], void *buf, void *ctx);
  void (*copy_from_buf)(int mb, int me, int p, int ilo[3], int ihi[3], void *buf, void *ctx);
  void (*add_from_buf)(int mb, int me, int p, int ilo[3], int ihi[3], void *buf, void *ctx);
};

MRC_CLASS_DECLARE(mrc_ddc, struct mrc_ddc);

struct mrc_domain;

void mrc_ddc_set_funcs(struct mrc_ddc *ddc, struct mrc_ddc_funcs *funcs);
void mrc_ddc_set_domain(struct mrc_ddc *ddc, struct mrc_domain *domain);
struct mrc_domain *mrc_ddc_get_domain(struct mrc_ddc *ddc);
void mrc_ddc_setup(struct mrc_ddc *ddc);
void mrc_ddc_destroy(struct mrc_ddc *ddc);
void mrc_ddc_add_ghosts(struct mrc_ddc *ddc, int mb, int me, void *ctx);
void mrc_ddc_fill_ghosts(struct mrc_ddc *ddc, int mb, int me, void *ctx);
void mrc_ddc_get_nei_rank_patch(struct mrc_ddc *ddc, int p, int dir[3],
				int *nei_rank, int *nei_patch);

// AMR-specific functionality
// should probably be given a more generic interface,
// in particular _apply() could be put into fill_ghosts()
void mrc_ddc_amr_add_value(struct mrc_ddc *ddc,
			   int row_patch, int rowm, int row[3],
			   int col_patch, int colm, int col[3],
			   float val);
void mrc_ddc_amr_assemble(struct mrc_ddc *ddc);
void mrc_ddc_amr_apply(struct mrc_ddc *ddc, struct mrc_m3 *fld);


#define MRC_DDC_BUF3(buf,m, ix,iy,iz)		\
  (buf[(((m) * (ihi[2] - ilo[2]) +		\
	 iz - ilo[2]) * (ihi[1] - ilo[1]) +	\
	iy - ilo[1]) * (ihi[0] - ilo[0]) +	\
       ix - ilo[0]])

extern struct mrc_ddc_funcs mrc_ddc_funcs_f3;
extern struct mrc_ddc_funcs mrc_ddc_funcs_m3;

static inline int
mrc_ddc_dir2idx(int dir[3])
{
  return ((dir[2] + 1) * 3 + dir[1] + 1) * 3 + dir[0] + 1;
}

extern int _mrc_ddc_idx2dir[27][3];

static inline int *
mrc_ddc_idx2dir(int idx)
{
  return _mrc_ddc_idx2dir[idx];
}

#endif

