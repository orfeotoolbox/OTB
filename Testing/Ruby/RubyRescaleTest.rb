#
#  Example on the use of the Rescale 
#
require 'otbapplication'

if ARGV.length < 2 then
    puts( "Usage: RubyRescaleTest input outputbasename" )
    exit( 1 )
end

tag = ARGV[0];

r = Otbapplication::Registry
applist = r.get_available_applications()
puts( applist )
