# -*- coding: utf-8 -*-

#
#  Example on the use of the Rescale
#

import os
import sys
if os.name == 'posix':
  orig_dlopen_flags = sys.getdlopenflags()
  try:
    import dl
  except ImportError:
    try:
      import DLFCN as dl
    except ImportError:
      dl = None
  if dl:
    sys.setdlopenflags(dl.RTLD_NOW|dl.RTLD_GLOBAL)
  # now load the module !
  import otbApplication as otb
  sys.setdlopenflags(orig_dlopen_flags)
  del dl
  del orig_dlopen_flags
else:
  import otbApplication as otb

app = otb.Registry.CreateApplication("Rescale")

app.SetParameterString("in", sys.argv[1])
app.SetParameterFloat("outmin", 1)
app.SetParameterFloat("outmax", 100)
app.SetParameterString("out", sys.argv[2] + ".tif")

app.ExecuteAndWriteOutput()
