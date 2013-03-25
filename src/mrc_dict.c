
#include <mrc_dict.h>
#include <mrc_list.h>
#include <mrc_params.h>
#include <mrc_io.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mrc_dict {
  struct mrc_obj obj;
};

static void
_mrc_dict_view(struct mrc_dict *dict)
{
  MPI_Comm comm = mrc_dict_comm(dict);

  mpi_printf(comm, "\n");
  mpi_printf(comm, "%-20s| %s\n", "parameter", "value");
  mpi_printf(comm, "--------------------+---------------------------------------- %s\n",
	     mrc_dict_name(dict));

  struct mrc_dict_entry *p;
  __list_for_each_entry(p, &dict->obj.dict_list, entry, struct mrc_dict_entry) {
    switch (p->type) {
    case PT_INT:
    case PT_SELECT:
      mpi_printf(comm, "%-20s| %d\n", p->name, p->val.u_int);
      break;
    case PT_BOOL:
      mpi_printf(comm, "%-20s| %s\n", p->name, p->val.u_bool ? "yes" : "no");
      break;
    case PT_FLOAT:
      mpi_printf(comm, "%-20s| %g\n", p->name, p->val.u_float);
      break;
    case PT_DOUBLE:
      mpi_printf(comm, "%-20s| %g\n", p->name, p->val.u_double);
      break;
    case PT_STRING:
      mpi_printf(comm, "%-20s| %s\n", p->name, p->val.u_string);
      break;
    }
  }
}

static void
_mrc_dict_write(struct mrc_dict *dict, struct mrc_io *io)
{
  const char *path = mrc_io_obj_path(io, dict);
  struct mrc_dict_entry *p;
  __list_for_each_entry(p, &dict->obj.dict_list, entry, struct mrc_dict_entry) {
    mrc_io_write_attr(io, path, p->type, p->name, &p->val);
  }
}

void
mrc_dict_add(struct mrc_dict *dict, int type, const char *name,
	     union param_u *pv)
{
  struct mrc_dict_entry *p = calloc(1, sizeof(*p));
  p->type = type;
  p->name = strdup(name);
  if (p->type == PT_STRING) {
    p->val.u_string = strdup(pv->u_string);
  } else {
    p->val = *pv;
  }
  list_add_tail(&p->entry, &dict->obj.dict_list);
}

void
mrc_dict_add_int(struct mrc_dict *dict, const char *name, int val)
{
  union param_u uval;
  uval.u_int = val;
  mrc_dict_add(dict, PT_INT, name, &uval);
}

void
mrc_dict_add_bool(struct mrc_dict *dict, const char *name, bool val)
{
  union param_u uval;
  uval.u_bool = val;
  mrc_dict_add(dict, PT_BOOL, name, &uval);
}

void
mrc_dict_add_float(struct mrc_dict *dict, const char *name, float val)
{
  union param_u uval;
  uval.u_float = val;
  mrc_dict_add(dict, PT_FLOAT, name, &uval);
}

void
mrc_dict_add_double(struct mrc_dict *dict, const char *name, double val)
{
  union param_u uval;
  uval.u_double = val;
  mrc_dict_add(dict, PT_DOUBLE, name, &uval);
}

void
mrc_dict_add_string(struct mrc_dict *dict, const char *name, const char *val)
{
  union param_u uval;
  uval.u_string = val;
  mrc_dict_add(dict, PT_STRING, name, &uval);
}

struct mrc_class_mrc_dict mrc_class_mrc_dict = {
  .name         = "mrc_dict",
  .size         = sizeof(struct mrc_dict),
  .view         = _mrc_dict_view,
  .write        = _mrc_dict_write,
};

