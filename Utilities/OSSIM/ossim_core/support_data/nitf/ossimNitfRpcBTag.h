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
// $Id: ossimNitfRpcBTag.h,v 1.4 2005/09/28 18:29:29 dburken Exp $
#ifndef ossimNitfRpcBTag_HEADER
#define ossimNitfRpcBTag_HEADER

#include <support_data/nitf/ossimNitfRpcBase.h>

/**
 * The layout of RPC00B is the same as RPC00A.  The difference is how the
 * coefficients are used; hence, all the code is in the ossimNitfRpcBase class.
 */
class OSSIM_DLL ossimNitfRpcBTag : public ossimNitfRpcBase
{
public:
   ossimNitfRpcBTag();

   /** @return "RPC00B" as an ossimString. */
   virtual ossimString getRegisterTagName() const;

TYPE_DATA   
};

#endif
