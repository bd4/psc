
#pragma once

#include <math.h>

#include <psc/moment.hxx>

// ======================================================================
// n

template <typename MF, typename D>
struct Moment_n_1st_nc
{
  using Mfields = MF;
  using dim_t = D;
  using real_t = typename Mfields::real_t;
  using moment_type =
    psc::moment::moment_n<psc::deposit::code::Deposit1stNc, dim_t>;

  template <typename Mparticles>
  static void run(Mfields& mflds, Mparticles& mprts)
  {
    moment_type{}(mflds.storage(), mflds.ib(), mprts);
  }
};

// ======================================================================
// rho

template <typename MF, typename D>
struct Moment_rho_1st_nc : ItemMomentCRTP<Moment_rho_1st_nc<MF, D>, MF>
{
  using Base = ItemMomentCRTP<Moment_rho_1st_nc<MF, D>, MF>;
  using Mfields = MF;
  using dim_t = D;
  using real_t = typename Mfields::real_t;
  using moment_type =
    psc::moment::moment_rho<psc::deposit::code::Deposit1stNc, dim_t>;

  template <typename Mparticles>
  explicit Moment_rho_1st_nc(const Mparticles& mprts) : Base{mprts.grid()}
  {
    Base::mres_gt_.view() = 0.f;
    moment_type{}(Base::mres_gt_, Base::mres_ib_, mprts);
    Base::bnd_.add_ghosts(mprts.grid(), Base::mres_gt_, Base::mres_ib_);
  }

  auto& operator()() { return Base::mres_gt_; }

  auto storage() = delete;
};
