//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
//
// LICENSE: LGPL see top level directory for more details
//
// Author:  David Burken <dburken@imagelinks.com>
//
//********************************************************************
// $Id: ossimNitfImageBandV2_1.h,v 1.5 2004/11/11 14:23:33 gpotts Exp $

#ifndef ossimNitfImageBandV2_1_HEADER
#define ossimNitfImageBandV2_1_HEADER
#include <iostream>
#include <iterator>
#include <base/data_types/ossimString.h>

#include <support_data/nitf/ossimNitfImageBandV2_0.h>
#include <support_data/nitf/ossimNitfImageLutV2_1.h>

class ossimNitfImageBandV2_1 : public ossimNitfImageBandV2_0
{
public:
   ossimNitfImageBandV2_1();
   ~ossimNitfImageBandV2_1();
/*    virtual void parseStream(std::istream& in); */
/*    virtual void writeStream(std::ostream& out); */
/*    virtual void print(std::ostream& out)const; */
   virtual void setBandRepresentation(const ossimString& value);
};

#endif
