//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimGeoAnnotationObject.cpp,v 1.3 2005/12/07 19:05:11 dburken Exp $

#include <imaging/annotation/ossimGeoAnnotationObject.h>

RTTI_DEF1(ossimGeoAnnotationObject,
          "ossimGeoAnnotationObject",
          ossimAnnotationObject)

ossimGeoAnnotationObject::ossimGeoAnnotationObject(unsigned char r,
                                                   unsigned char g,
                                                   unsigned char b,
                                                   long thickness)
   :ossimAnnotationObject(r, g, b, thickness)
{
}

ossimGeoAnnotationObject::ossimGeoAnnotationObject(
   const ossimGeoAnnotationObject& rhs)
   :
   ossimAnnotationObject(rhs)
{
}

ossimGeoAnnotationObject::~ossimGeoAnnotationObject()
{
}

bool ossimGeoAnnotationObject::saveState(ossimKeywordlist& kwl,
                                         const char* prefix) const
{
   return ossimAnnotationObject::saveState(kwl, prefix);
}

bool ossimGeoAnnotationObject::loadState(const ossimKeywordlist& kwl,
                                         const char* prefix)
{
   return ossimAnnotationObject::loadState(kwl, prefix);
}
