# -*- coding: utf-8 -*-

# https://mail.python.org/pipermail//cplusplus-sig/2011-September/016145.html

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

t = __import__(sys.argv[1])
argv = sys.argv[1:]
t.test(otb, argv)
