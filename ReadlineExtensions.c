#include "ruby.h"
#include <readline/readline.h>
#include <readline/history.h>

VALUE Readline = Qnil;
VALUE ReadlineExtensions = Qnil;
static ID quoting_detection_proc;

void Init_readlineextensions();
int readline_char_is_quoted_p(char *text, int index);
VALUE readlineextensions_s_set_quoting_detection_proc(VALUE self, VALUE proc);
VALUE readlineextensions_s_quoting_detection_proc(VALUE self);

void Init_readlineextensions() {
    quoting_detection_proc = rb_intern("quoting_detection_proc");

    Readline = rb_const_get(rb_cObject, rb_intern("Readline"));
    ReadlineExtensions = rb_define_module("ReadlineExtensions");
    rb_define_singleton_method(ReadlineExtensions, "quoting_detection_proc=",
            readlineextensions_s_set_quoting_detection_proc, 1);
    rb_define_singleton_method(ReadlineExtensions, "quoting_detection_proc",
            readlineextensions_s_quoting_detection_proc, 0);

    rl_char_is_quoted_p = &readline_char_is_quoted_p;
}

VALUE readlineextensions_s_set_quoting_detection_proc(VALUE self, VALUE proc) {
    if (!NIL_P(proc) && !rb_respond_to(proc, rb_intern("call"))) {
        rb_raise(rb_eArgError, "argument must respond to `call'");
    }
    return rb_ivar_set(ReadlineExtensions, quoting_detection_proc, proc);
}

VALUE readlineextensions_s_quoting_detection_proc(VALUE self) {
    return rb_ivar_get(ReadlineExtensions, quoting_detection_proc);
}

int readline_char_is_quoted_p(char *text, int index) {
    VALUE proc, result;

    proc = rb_attr_get(ReadlineExtensions, quoting_detection_proc);
    if (NIL_P(proc)) {
        return 0;
    }

    result = rb_funcall(proc, rb_intern("call"), 2,
            rb_locale_str_new_cstr(text), INT2FIX(index));
    return result ? 1 : 0;
}
