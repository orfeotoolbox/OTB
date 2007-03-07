/*
      poly_Boolean: a polygon clip library
      Copyright (C) 1997  Alexey Nikitin, Michael Leonov
      leonov@propro.iis.nsk.su

      This library is free software; you can redistribute it and/or
      modify it under the terms of the GNU Library General Public
      License as published by the Free Software Foundation; either
      version 2 of the License, or (at your option) any later version.

      This library is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
      Library General Public License for more details.

      You should have received a copy of the GNU Library General Public
      License along with this library; if not, write to the Free
      Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

      polyio.h
      (C) 1997 Michael Leonov

*/
#ifndef _POLYIO_H
#define _POLYIO_H

#include "polyarea.h"

#ifdef __cplusplus
extern "C" {
#endif

int LoadPOLYAREA( POLYAREA **PA, char * fname);
int SavePOLYAREA( POLYAREA * PA, char * fname);

#ifdef __cplusplus
}
#endif

#endif
