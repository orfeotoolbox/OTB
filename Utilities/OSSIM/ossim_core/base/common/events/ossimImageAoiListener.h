//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimImageAoiListener.h,v 1.3 2002/11/25 18:49:10 gpotts Exp $
#ifndef ossimImageAoiListener_HEADER
#define ossimImageAoiListener_HEADER
#include "ossimListener.h"

class OSSIMDLLEXPORT ossimImageRectangleEvent;
class OSSIMDLLEXPORT ossimImagePolygonEvent;

class OSSIMDLLEXPORT ossimImageAoiListener : public ossimListener
{
public:
   ossimImageAoiListener():ossimListener(){}
   virtual ~ossimImageAoiListener(){}
   
   virtual void processEvent(ossimEvent& event);

   virtual void imageRectangleEvent(ossimImageRectangleEvent& event)
      {}
   virtual void imagePolygonEvent(ossimImagePolygonEvent& event)
      {}
};
#endif
