require 'benchmark'
require 'tzinfo'
require File.join(File.dirname(__FILE__), '..', 'lib', 'fastz')

TZ = 'America/New_York'

tz1 = TZInfo::Timezone.get(TZ)
tz2 = TimeZone[TZ]
time = Time.utc(2005,8,29,15,35,0)


puts '-> to utc'
n = 50000
Benchmark.bm do |x|
  x.report("Fastz ") {
    l2 = nil
    n.times { 
      l2 = tz2.utc_time_for(time)
    }
  }

  x.report("TZInfo") {
    l1 = nil
    n.times { 
      l1 = tz1.local_to_utc(time)
    }
  }
end

puts '-> to local'
Benchmark.bm do |x|
  x.report("Fastz ") {
    l2 = nil
    n.times { 
      l2 = tz2.local_time_for(time)
    }
  }

  x.report("TZInfo") {
    l1 = nil
    n.times { 
      l1 = tz1.utc_to_local(time)
    }
  }
end
