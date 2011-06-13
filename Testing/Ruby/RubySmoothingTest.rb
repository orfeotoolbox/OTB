#
#  Example on the use of the Smoothing 
#
require 'otbapplication'

if ARGV.length < 2 then
    puts( "Usage: RubySmoothingTest input outputbasename" )
    exit( 1 )
end

tag = ARGV[0];

r = Otbapplication::Registry
applist = r.get_available_applications()
puts( applist )