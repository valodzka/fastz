
require 'mkmf'

$defs << '-DSTD_INSPIRED'
$objs = %w{asctime.o  localtime.o  fastz.o}

create_makefile('fatz')
