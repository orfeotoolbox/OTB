//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// Description: This class give the capability to access tiles from an
//              vpf file.
//
//********************************************************************
// $Id: vpf.h,v 1.4 2004/11/10 17:44:32 dburken Exp $
#ifndef VPF_HEADER
#define VPF_HEADER

#include <ossimConfig.h>

#if __OSSIM_CARBON__
#define MAXINT INT_MAX
#endif

extern "C"
{
#include "vpf_util/vpfview.h"   
#include "vpf_util/vpfselec.h"
#include "vpf_util/vpftable.h"
#include "vpf_util/vpfprim.h"
#include "vpf_util/vpfmisc.h"
#include "vpf_util/vpfdisp.h"
}

#endif
