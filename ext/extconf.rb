# $: << '/home/pavel/dev/rice/ruby/lib'
require 'mkmf-rice'

with_config("release") do
  $defs.push("-DNDEBUG")
  $defs.push("-DBOOST_DISABLE_ASSERTS")
end
create_makefile('fastz')
