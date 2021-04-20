
#include <psc_fields_cuda.h>
#include <fields_item.hxx>

#include "cuda_iface.h"

void cuda_mfields_calc_dive_yz(MfieldsStateCuda& mflds, MfieldsCuda& mf);
void cuda_mfields_calc_dive_xyz(MfieldsStateCuda& mflds, MfieldsCuda& mf);

template <>
struct Item_dive<MfieldsStateCuda>
{
  using Mfields = MfieldsCuda;
  using MfieldsState = MfieldsStateCuda;
  constexpr static const char* name = "dive";
  constexpr static int n_comps = 1;
  static std::vector<std::string> fld_names() { return {"dive"}; } // FIXME

  Item_dive(MfieldsState& mflds)
    : mres_{mflds.grid(), n_comps, mflds.grid().ibn}
  {
    if (mflds.grid().isInvar(0)) {
      cuda_mfields_calc_dive_yz(mflds, mres_);
    } else {
      cuda_mfields_calc_dive_xyz(mflds, mres_);
    }
  }

  auto gt() { return view_interior(mres_.gt(), mres_.ibn()); }

private:
  Mfields mres_;
};
