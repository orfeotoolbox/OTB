# -*- coding: utf-8 -*-
#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

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
