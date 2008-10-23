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
// $Id: ossimDemStats.h 7174 2005-01-11 18:10:38Z dburken $

#ifndef ossimDemStats_HEADER
#define ossimDemStats_HEADER

#include <iostream>

class ossimDemStats
{
public:

   ossimDemStats();
   ~ossimDemStats();

   friend std::ostream& operator<<(std::ostream& s,
                                   ossimDemStats const& stats);
   friend std::istream& operator>>(std::istream& s, ossimDemStats& stats);

   //Accessors

   /*!
    *  1 = available
    *  2 = unavailable
    *  The code defines the availability of statics
    */
   long getAvailCodeDE2() const;

   long getRSMEDE2x () const;

   long getRSMEDE2y () const;

   long getRSMEDE2z () const;

   /*!
    *  This code defines the sample size use
    *  to compute the accuracy, if 0 then the
    *  accuracy is estimated.
    */
   long getSampleSizeDE2() const;

   /*!
    *  1 = available
    *  2 = unavailable
    *  The code defines the availability of statics
    */
   long getAvailCodeDE5() const;

   long getRSMEDE5x () const;

   long getRSMEDE5y () const;

   long getRSMEDE5z () const;
	
   /*!
    *  This code defines the sample size use
    *  to compute the accuracy, if 0 then the
    *  accuracy is estimated.
    */
   long getSampleSizeDE5() const;

private:

   long _availCodeDE2;
   long _rsmeDE2x;
   long _rsmeDE2y;
   long _rsmeDE2z;
   long _sampleSizeDE2;
   long _availCodeDE5;
   long _rsmeDE5x;
   long _rsmeDE5y;
   long _rsmeDE5z;
   long _sampleSizeDE5;
};

#endif // #ifndef ossimDemStats_HEADER
