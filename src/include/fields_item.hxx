
#pragma once

#include "psc_output_fields_item_private.h"

#include "particles.hxx"
#include "fields3d.hxx"
#include "bnd.hxx"

#include <mrc_profile.h>

// ======================================================================
// FieldsItemBase

struct FieldsItemBase
{
  virtual void run(PscMfieldsBase mflds_base, PscMparticlesBase mprts_base,
		   PscMfieldsBase mres_base) = 0;
};

// ======================================================================
// PscFieldsItem

template<typename S>
struct PscFieldsItem
{
  using sub_t = S;
  
  static_assert(std::is_convertible<sub_t*, FieldsItemBase*>::value,
  		"sub classes used in PscFieldsItemParticles must derive from FieldsItemBase");
  
  explicit PscFieldsItem(psc_output_fields_item *item)
    : item_(item)
  {}

  void operator()(PscMfieldsBase mflds, PscMparticlesBase mprts, PscMfieldsBase mres)
  {
    struct psc_output_fields_item_ops *ops = psc_output_fields_item_ops(item_);
    assert(ops->run_all);
    
    ops->run_all(item_, mflds.mflds(), mprts.mprts(), mres.mflds());
    if (ops->flags & POFI_ADD_GHOSTS) {
      assert(item_->bnd);
      auto bnd = PscBndBase(item_->bnd);
      bnd.add_ghosts(mres, 0, mres.n_fields());
    }
  }

  sub_t* sub() { return mrc_to_subobj(item_, sub_t); }
  sub_t* operator->() { return sub(); }

private:
  psc_output_fields_item* item_;
};

using PscFieldsItemBase = PscFieldsItem<FieldsItemBase>;

// ======================================================================
// PscFieldsItemWrapper

template<typename FieldsItem>
class PscFieldsItemWrapper
{
public:
  const static size_t size = sizeof(FieldsItem);
  
  static void setup(psc_output_fields_item* _item)
  {
    PscFieldsItem<FieldsItem> item(_item);
    new(item.sub()) FieldsItem();
  }

  static void destroy(psc_output_fields_item* _item)
  {
    PscFieldsItem<FieldsItem> item(_item);
    item->~FieldsItem();
  }

  static void run(struct psc_output_fields_item *_item, struct psc_mfields *mflds_base,
		  struct psc_mparticles *mprts_base, struct psc_mfields *mres_base)
  {
    PscFieldsItem<FieldsItem> item(_item);
    item->run(mflds_base, PscMparticlesBase{mprts_base}, mres_base);
  }
};

// ======================================================================
// FieldsItemOps

template<typename Item_t>
struct FieldsItemOps : psc_output_fields_item_ops {
  using Wrapper_t = PscFieldsItemWrapper<Item_t>;
  FieldsItemOps() {
    name      = Item_t::name();
    size      = Wrapper_t::size;
    setup     = Wrapper_t::setup;
    destroy   = Wrapper_t::destroy;
    nr_comp   = Item_t::n_comps;
    fld_names = Item_t::fld_names();
    run_all   = Wrapper_t::run;
    flags     = Item_t::flags;
  }
};

