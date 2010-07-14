//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
//         Orginally written by Jamie Moyers (jmoyers@geeks.com)
//         Adapted from the package KDEM.
// Description: This class provides some statistics for DEMs.
//
//********************************************************************
// $Id: ossimDemStats.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <stdlib.h>

#include <ossim/support_data/ossimDemStats.h>
#include <ossim/support_data/ossimDemUtil.h>
using namespace std;

ossimDemStats::ossimDemStats()
   : _availCodeDE2(0),
     _rsmeDE2x(0),
     _rsmeDE2y(0),
     _rsmeDE2z(0),
     _sampleSizeDE2(0),
     _availCodeDE5(0),
     _rsmeDE5x(0),
     _rsmeDE5y(0),
     _rsmeDE5z(0),
     _sampleSizeDE5(0)

{
}

ossimDemStats::~ossimDemStats()
{
}

long
ossimDemStats::getAvailCodeDE2() const
{
   return _availCodeDE2;
}

long
ossimDemStats::getRSMEDE2x() const
{
   return _rsmeDE2x;
}

long
ossimDemStats::getRSMEDE2y() const
{
   return _rsmeDE2y;
}

long
ossimDemStats::getRSMEDE2z() const
{
   return _rsmeDE2z;
}

long
ossimDemStats::getSampleSizeDE2() const
{
   return _sampleSizeDE2;
}

long
ossimDemStats::getAvailCodeDE5() const
{
   return _availCodeDE5;
}

long
ossimDemStats::getRSMEDE5x() const
{
   return _rsmeDE5x;
}

long
ossimDemStats::getRSMEDE5y() const
{
   return _rsmeDE5y;
}

long
ossimDemStats::getRSMEDE5z() const
{
   return _rsmeDE5z;
}

long
ossimDemStats::getSampleSizeDE5() const
{
   return _sampleSizeDE5;
}

ostream&
operator<<(ostream& s,  ossimDemStats const& /* stats */)
{
   return s;
}

istream&
operator>>(istream& s, ossimDemStats& stats)
{
   char bufstr[1024];

   ossimDemUtil::getRecord(s, bufstr);

   stats._availCodeDE2 = ossimDemUtil::getLong(bufstr, 0, 6);
   stats._rsmeDE2x = ossimDemUtil::getLong(bufstr, 6, 6);
   stats._rsmeDE2y = ossimDemUtil::getLong(bufstr, 12, 6);
   stats._rsmeDE2z = ossimDemUtil::getLong(bufstr, 18, 6);
   stats._sampleSizeDE2 = ossimDemUtil::getLong(bufstr, 24, 6);
   stats._availCodeDE5 = ossimDemUtil::getLong(bufstr, 30, 6);
   stats._rsmeDE5x = ossimDemUtil::getLong(bufstr, 36, 6);
   stats._rsmeDE5y = ossimDemUtil::getLong(bufstr, 42, 6);
   stats._rsmeDE5z = ossimDemUtil::getLong(bufstr, 48, 6);
   stats._sampleSizeDE5 = ossimDemUtil::getLong(bufstr, 54, 6);

   return s;
}
