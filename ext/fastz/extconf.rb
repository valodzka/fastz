
require 'mkmf'

$defs << '-DSTD_INSPIRED' << '-DNOID' << '-DNDEBUG'
$objs = %w{asctime.o  localtime.o  fastz.o}

create_makefile('fastz_ext')
