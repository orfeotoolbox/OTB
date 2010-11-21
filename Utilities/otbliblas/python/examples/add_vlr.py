from liblas import file as lasfile
from liblas import vlr
from liblas import header as lasheader

f = lasfile.File('test/data/srs_utm17.las',None,'rb')
h = f.header

v = vlr.VLR()

text =  open('schemas/las.xml','rb').read()

import ctypes

data = ctypes.create_string_buffer(text)

v.userid='hobu'
v.recordid = 12345
v.data = data

h.add_vlr(v)

f2 = lasfile.File('junk.las',header=h,mode='w')
for p in f:
    f2.write(p)
f2.close()
