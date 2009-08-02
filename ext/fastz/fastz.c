
#include <xtime.h>
#include <ruby.h>
#include <st.h>

extern VALUE rb_cTime;

struct time_object {
  struct timeval tv;
  struct tm tm;
  int gmt;
  int tm_got;
};


static st_table *tz_cache = NULL;

static zoneinfo_t timezone_cached_get(const char *name)
{
  st_data_t key = (st_data_t)name, value = 0;
 
  //  if(!tz_cache) tz_cache = st_init_strcasetable();
  if(!tz_cache) tz_cache = st_init_strtable();
 
  st_lookup(tz_cache, key, &value);
 
  if (!value) {
    zoneinfo_t tz = loadzone(name);
    if (!tz)
      rb_raise(rb_eArgError, "time zone \"%s\" not found", name);
    st_insert(tz_cache, (st_data_t)name, (st_data_t)tz);
    value = (st_data_t)tz;
  }
 
  return (zoneinfo_t)value;
}

static VALUE rb_timezone_aref(VALUE klass, VALUE name) 
{
  zoneinfo_t tz = timezone_cached_get(StringValueCStr(name));
  return Data_Wrap_Struct(klass, 0, 0, tz);
}

static VALUE rb_timezone_utc_time_for(VALUE self, VALUE time)
{
  return Qnil;
}

static VALUE rb_timezone_local_time_for(VALUE self, VALUE time)
{
  struct time_object *tobj;
  time_t t;
  struct tm result;
  struct tm *tm_tmp;
  zoneinfo_t *zone;

  Data_Get_Struct(self, zoneinfo_t, zone);
  Data_Get_Struct(time, struct time_object, tobj);

  //time_modify(time);
  t = tobj->tv.tv_sec;
 
  tm_tmp = localtime_zr(&t, &result, *zone);

  if (!tm_tmp) {
    rb_raise(rb_eArgError, "localtime error");
  }

  tobj->tm = *tm_tmp;
  tobj->tm_got = 1;
  tobj->gmt = 0;

  return Qnil;
}

void Init_fastz_ext() {
  VALUE rb_cTimeZone = rb_define_class("TimeZone", rb_cObject);
  rb_define_singleton_method(rb_cTimeZone, "[]", rb_timezone_aref, 1);
  rb_define_method(rb_cTimeZone,"utc_time_for", rb_timezone_utc_time_for, 1);
  rb_define_method(rb_cTimeZone,"local_time_for", rb_timezone_local_time_for, 1);
}
