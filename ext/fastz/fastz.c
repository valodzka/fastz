
#include <xtime.h>
#include <ruby.h>

static VALUE rb_timezone_aref(VALUE klass, VALUE name) 
{
  return Qnil;
}

static VALUE rb_timezone_utc_time_for(VALUE self, VALUE time)
{
  return Qnil;
}

static VALUE rb_timezone_local_time_for(VALUE self, VALUE time)
{
  return Qnil;
}

static void Init_fastz() {
  VALUE rb_cTimeZone = rb_define_class("TimeZone", rb_cObject);
  rb_define_singleton_method(rb_cTimeZone, "[]", rb_timezone_aref, 1);
  rb_define_method(rb_cTimeZone,"utc_time_for", rb_timezone_utc_time_for, 1);
  rb_define_method(rb_cTimeZone,"local_time_for", rb_timezone_local_time_for, 1);
}
