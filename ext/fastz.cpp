
#include "rice/Object.hpp"
#include "rice/Class.hpp"
#include "rice/Array.hpp"
#include "rice/String.hpp"
#include "rice/Data_Type.hpp"
#include "rice/Data_Object.hpp"
#include "rice/Symbol.hpp"
#include "rice/Identifier.hpp"
#include "rice/Constructor.hpp"

#include "boost/date_time/local_time/local_time.hpp"
#include "boost/date_time/local_time/tz_database.hpp"

using namespace Rice;
using std::string;
using std::vector;
using std::exception;
using boost::local_time::local_date_time;
using boost::local_time::tz_database;
using boost::local_time::time_zone_ptr;
using boost::posix_time::ptime;
using boost::posix_time::time_duration;
using boost::gregorian::date;

static const Identifier TO_A("to_a"), TV_USEC("tv_usec"), UTC("utc");

template <>
Object to_ruby<ptime>(ptime const & pt) {
  const	time_duration& td = pt.time_of_day();
  const date& d = pt.date();  
  Object cTime(rb_cTime);

  long 
	year	 = d.year(),
	month	 = d.month(),
	day		 = d.day(),
	hours	 = td.hours(),
	minutes	 = td.minutes(),
	seconds	 = td.seconds(),
	microsec = td.total_microseconds() - (hours * 3600 + minutes * 60 + seconds) * 1000000;
  
  return cTime.call(UTC, year, month, day, hours, minutes, seconds, microsec);
}

template<>
ptime from_ruby<ptime>(Object time)
{  
  Array t = Array(time.call(TO_A));
  Object usec = time.call(TV_USEC);  
  float ajust = time_duration::traits_type::ticks_per_second / 1000000.0;

  return ptime(date(from_ruby<long>(t[5]), 
					from_ruby<long>(t[4]), 
					from_ruby<long>(t[3])), 
			   time_duration(from_ruby<long>(t[2]), 
							 from_ruby<long>(t[1]), 
							 from_ruby<long>(t[0]),
							 from_ruby<long>(usec)*ajust));
}
  

class TimeZoneNotFound : public exception {
public:
  virtual const char* what() const throw () {
	return "Time zone not found";
  } 
};

class TimeZone {
  static tz_database tz_db;
  static string tz_db_path;
  static bool is_tz_loaded;

  time_zone_ptr self;
public:  

  static void ensure_loaded() {
	if (!is_tz_loaded) {
	  tz_db.load_from_file(tz_db_path);
	  is_tz_loaded = true;
	}
  }

  static void set_tz_db_path(Object tz_class, const char* tz_db_path_) {
	tz_db_path = tz_db_path_;
  }

  static Data_Object<TimeZone> get(Object tz_class, const char* tz_name) {
	ensure_loaded();
	time_zone_ptr tz = tz_db.time_zone_from_region(tz_name);
	if (tz.get() == 0)
	  throw TimeZoneNotFound();
	return new TimeZone(tz);
  }  

  static Array regions(Object tz) {      
	ensure_loaded();
	vector<string> regs = tz_db.region_list();
	return Array(regs.begin(), regs.end());
  }

  TimeZone(time_zone_ptr tz_ptr) : self(tz_ptr) {}
  
  const string dst_zone_abbrev() { return self->dst_zone_abbrev(); }
  const string std_zone_abbrev() { return self->std_zone_abbrev(); }
  const string dst_zone_name()   { return self->dst_zone_name(); }
  const string std_zone_name()   { return self->std_zone_name(); }  
  bool has_dst() { return self->has_dst(); }

  Object local_time(Object utc_time) {	
	local_date_time ldt(from_ruby<ptime>(utc_time), self);
	return to_ruby(ldt.local_time());
  }

  Object utc_time(Object local_time) {
	ptime local_ptime = from_ruby<ptime>(local_time);
	local_date_time ldt(local_ptime.date(), 
						local_ptime.time_of_day(), 
						self, 
						local_date_time::EXCEPTION_ON_ERROR);
	return to_ruby(ldt.utc_time());	
  }
};

bool TimeZone::is_tz_loaded = false;
tz_database TimeZone::tz_db;
string TimeZone::tz_db_path;

extern "C" 
void Init_fastz() {
  RUBY_TRY
	{
	  Data_Type<TimeZone> rb_cTimeZone = define_class<TimeZone>("TimeZone")
		.define_singleton_method("tz_db_path=", &TimeZone::set_tz_db_path)
		.define_singleton_method("[]", &TimeZone::get)
		.define_singleton_method("regions", &TimeZone::regions)
		.define_method("dst_abbrev", &TimeZone::dst_zone_abbrev)
		.define_method("std_abbrev", &TimeZone::std_zone_abbrev)
		.define_method("dst_name", &TimeZone::dst_zone_name)
		.define_method("std_name", &TimeZone::std_zone_name)
		.define_method("has_dst?", &TimeZone::has_dst)
		.define_method("local_time_for", &TimeZone::local_time)
	    .define_method("utc_time_for", &TimeZone::utc_time);
	}
  RUBY_CATCH
}

