
dir = File.expand_path(File.dirname(__FILE__))
require File.join(dir, '..', 'ext', 'fastz')
TimeZone.tz_db_path = File.join(dir, '..', 'data', 'date_time_zonespec.csv')
