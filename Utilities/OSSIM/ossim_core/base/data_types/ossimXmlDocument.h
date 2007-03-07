//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.  All rights reserved.
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
// Author:  Oscar Kramer <okramer@imagelinks.com> (ossim port by D. Burken)
//
// Description:  
//
// Contains declaration of class ossimXmlDocument. This class provides read-only
// parsing and accessing of an XML document file.
// 
//*****************************************************************************
// $Id: ossimXmlDocument.h,v 1.4 2005/06/17 13:35:57 gpotts Exp $
#ifndef ossimXmlDocument_HEADER
#define ossimXmlDocument_HEADER

#include <vector>
#include <base/common/ossimObject.h>
#include <base/common/ossimErrorStatusInterface.h>
#include <base/common/ossimRefPtr.h>
#include "ossimFilename.h"

class ossimXmlNode;

class OSSIMDLLEXPORT ossimXmlDocument : public ossimObject,
                                     public ossimErrorStatusInterface
{
public:
   ossimXmlDocument(const ossimFilename& xmlFileName="");
   ~ossimXmlDocument();

   bool write(const ossimFilename& file);
   bool openFile(const ossimFilename& filename);
   bool read(std::istream& in);
   /**
    * Appends any matching nodes to the list supplied (should be empty):
    */
   void findNodes(const ossimString& xpath,
                  std::vector<ossimRefPtr<ossimXmlNode> >& nodelist) const;
   
   OSSIMDLLEXPORT friend std::ostream& operator << (std::ostream& os, const ossimXmlDocument& xml_doc);

   void initRoot(ossimRefPtr<ossimXmlNode> node);

   ossimRefPtr<ossimXmlNode> getRoot();
   const ossimRefPtr<ossimXmlNode> getRoot()const;
   ossimRefPtr<ossimXmlNode> removeRoot();
   void fromKwl(const ossimKeywordlist& kwl);
   void toKwl(ossimKeywordlist& kwl, const ossimString& prefix = "")const;
   
private:
   ossimRefPtr<ossimXmlNode>  theRootNode;
   ossimString                theXmlHeader;
   ossimFilename              theFilename;

   bool readHeader(std::istream& in);
TYPE_DATA
};

#endif /* #ifndef ossimXmlDocument_HEADER */
