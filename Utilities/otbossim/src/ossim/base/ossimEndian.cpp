//******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This file contains the implementation of the
//              endian byte swap routines.
//
//***********************************
// $Id: ossimEndian.cpp 22197 2013-03-12 02:00:55Z dburken $

#include <ossim/base/ossimEndian.h>

ossimEndian::ossimEndian()
{
   ossim_uint16  test;
   ossim_uint8  *testPtr=0;
   test = 0x0001;
   
   testPtr       = reinterpret_cast<unsigned char*>(&test);
   theSystemEndianType = testPtr[0] ? OSSIM_LITTLE_ENDIAN : OSSIM_BIG_ENDIAN;
}
