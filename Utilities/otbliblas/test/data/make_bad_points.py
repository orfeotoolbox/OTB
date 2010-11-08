from liblas import file
from liblas import header
from liblas import point

import datetime

h = header.Header()
h.date = datetime.datetime.now()
h.dataformat_id = 1

f = file.File('bad_points_1.1.las', mode='w', header=h)

p = point.Point()
p.flightline_edge = 3
p.return_number = 3
p.classification = 63
p.scan_angle = -127

#p.time = datetime.datetime(1969, 12, 31, 17, 59, 59, 2)

f.write(p)
f.close()
