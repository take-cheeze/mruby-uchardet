#include "nsUniversalDetector.h"
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/variable.h>

#include <new>


namespace {
struct mrb_uchardet : public nsUniversalDetector {
  mrb_state* const M;
  mrb_value const self;

  mrb_uchardet(mrb_state *M, mrb_value self, uint32_t filter)
      : nsUniversalDetector(filter), M(M), self(self) {}

  void Report(char const * result) {
    mrb_iv_set(M, self, mrb_intern_lit(M, "result"), mrb_str_new_cstr(M, result));
  }
};

void free_uchardet(mrb_state *M, void *ptr) {
  mrb_assert(ptr);
  reinterpret_cast<mrb_uchardet*>(ptr)->~mrb_uchardet();
  mrb_free(M, ptr);
}

mrb_data_type type = { "uchardet", free_uchardet };

mrb_value init(mrb_state *M, mrb_value self) {
  mrb_int flags = 0;
  mrb_get_args(M, "|i", &flags);
  mrb_assert(mrb_type(self) == MRB_TT_DATA);
  DATA_PTR(self) = new(mrb_malloc(M, sizeof(mrb_uchardet))) mrb_uchardet(M, self, flags);
  DATA_TYPE(self) = &type;
  return self;
}

mrb_value handle(mrb_state *M, mrb_value self) {
  char *data; mrb_int len;
  mrb_get_args(M, "s", &data, &len);
  mrb_uchardet *det = reinterpret_cast<mrb_uchardet*>(DATA_PTR(self));
  if (det->HandleData(data, len) != NS_OK) {
    mrb_raise(M, mrb_class_get(M, "RuntimeError"), "failed handling data");
  }
  return self;
}

mrb_value end(mrb_state *M, mrb_value self) {
  return reinterpret_cast<mrb_uchardet*>(DATA_PTR(self))->DataEnd(), self;
}

mrb_value result(mrb_state *M, mrb_value self) {
  mrb_value ret = mrb_iv_get(M, self, mrb_intern_lit(M, "result"));
  if (mrb_nil_p(ret)) {
    mrb_raise(M, mrb_class_get(M, "RuntimeError"), "result not set");
  }
  return ret;
}
}

extern "C" void mrb_mruby_uchardet_gem_init(mrb_state *M) {
  RClass *cls = mrb_define_class(M, "UCharDet", M->object_class);
  MRB_SET_INSTANCE_TT(cls, MRB_TT_DATA);
  mrb_define_method(M, cls, "initialize", init, MRB_ARGS_ARG(0, 1));
  mrb_define_method(M, cls, "handle", handle, MRB_ARGS_REQ(1));
  mrb_define_method(M, cls, "end", end, MRB_ARGS_NONE());
  mrb_define_method(M, cls, "result", result, MRB_ARGS_NONE());

  mrb_define_const(M, cls, "FILTER_CHINISE_SIMPLIFIED", mrb_fixnum_value(NS_FILTER_CHINESE_SIMPLIFIED));
  mrb_define_const(M, cls, "FILTER_CHINISE_TRADITIONAL", mrb_fixnum_value(NS_FILTER_CHINESE_TRADITIONAL));
  mrb_define_const(M, cls, "FILTER_JAPANESE", mrb_fixnum_value(NS_FILTER_JAPANESE));
  mrb_define_const(M, cls, "FILTER_KOREAN", mrb_fixnum_value(NS_FILTER_KOREAN));
  mrb_define_const(M, cls, "FILTER_NON_CJK", mrb_fixnum_value(NS_FILTER_NON_CJK));
  mrb_define_const(M, cls, "FILTER_ALL", mrb_fixnum_value(NS_FILTER_ALL));
  mrb_define_const(M, cls, "FILTER_CHINESE", mrb_fixnum_value(NS_FILTER_CHINESE));
  mrb_define_const(M, cls, "FILTER_CJK", mrb_fixnum_value(NS_FILTER_CJK));
}

extern "C" void mrb_mruby_uchardet_gem_final(mrb_state*) {}
