//*******************************************************************
// Copyright (C) 2000 Intelligence Data Systems. 
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRpcBTag.h 22013 2012-12-19 17:37:20Z dburken $
#ifndef ossimNitfRpcBTag_HEADER
#define ossimNitfRpcBTag_HEADER 1

#include <ossim/support_data/ossimNitfRpcBase.h>

/**
 * The layout of RPC00B is the same as RPC00A.  The difference is how the
 * coefficients are used; hence, all the code is in the ossimNitfRpcBase class.
 */
class OSSIM_DLL ossimNitfRpcBTag : public ossimNitfRpcBase
{
public:
   ossimNitfRpcBTag();

TYPE_DATA   
};

#endif
