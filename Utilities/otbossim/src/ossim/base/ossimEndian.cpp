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
// $Id: ossimEndian.cpp 11678 2007-09-07 12:06:19Z dburken $

#include <ossim/base/ossimEndian.h>

ossimEndian::ossimEndian()
{
   ossim_uint16  test;
   ossim_uint8  *testPtr=0;
   test = 0x0001;
   
   testPtr       = reinterpret_cast<unsigned char*>(&test);
   theSystemEndianType = testPtr[0] ? OSSIM_LITTLE_ENDIAN : OSSIM_BIG_ENDIAN;
}


