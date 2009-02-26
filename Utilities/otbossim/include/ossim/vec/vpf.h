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
// $Id: vpf.h 13586 2008-09-17 01:35:25Z gpotts $
#ifndef VPF_HEADER
#define VPF_HEADER

#include <ossim/ossimConfig.h>

#if __OSSIM_CARBON__
#define MAXINT INT_MAX
#endif

extern "C"
{
#include <ossim/vpfutil/vpfapi.h>
#include <ossim/vpfutil/vpfview.h>
#include <ossim/vpfutil/vpfselec.h>
#include <ossim/vpfutil/vpftable.h>
#include <ossim/vpfutil/vpfprim.h>
#include <ossim/vpfutil/vpfmisc.h>
#include <ossim/vpfutil/vpfdisp.h>
}

#endif
