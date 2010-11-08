#!/usr/bin/env python

from liblas import file
from liblas import header
from liblas import point
from liblas import color
from liblas import srs
from liblas import guid

import datetime

p = point.Point()
p.flightline_edge = 0
p.return_number = 1
p.classification = 0
p.scan_angle = -13
p.x = 470692.447538
p.y = 4602888.904642
p.z = 16.0
c = color.Color()

c.red=255
c.green=12
c.blue=234
p.color = c
p.time = datetime.datetime(2008,3,19)
p.classification = 2
p.return_number = 2

s = srs.SRS()
s.proj4 = '+proj=utm +zone=15 +ellps=NAD83 +datum=NAD83 +units=m +no_defs '

g = guid.GUID(key='8388f1b8-aa1b-4108-bca3-6bc68e7b062e')
number_of_points = 1

def write_file(version, format):
    h = header.Header()
    h.guid = g
    h.date = datetime.datetime.now()
    h.dataformat_id = format
    h.major_version = 1
    h.minor_version = version
    h.min = [p.x, p.y, p.z]
    h.max = [p.x, p.y, p.z]
    h.point_return_count = [0L, number_of_points, 0L, 0L, 0L, 0L, 0L, 0L]
    h.srs = s
    h.date = p.time
    
    f = file.File('1.%d_%d.las'%(version,format), mode='w', header=h)
    for i in xrange(number_of_points):
        f.write(p)
    f.close()

write_file(0,0)
write_file(0,1)
write_file(1,0)
write_file(1,1)
write_file(2,0)
write_file(2,1)
write_file(2,2)
write_file(2,3)