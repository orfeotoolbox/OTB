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
// Contains declaration of class ossimXmlNode.
// 
//*****************************************************************************
// $Id: ossimXmlNode.h,v 1.9 2005/06/17 13:35:57 gpotts Exp $
#ifndef ossimXmlNode_HEADER
#define ossimXmlNode_HEADER

#include <vector>
using namespace std;

#include "base/common/ossimObject.h"
#include "base/common/ossimRefPtr.h"
#include "base/common/ossimErrorStatusInterface.h"
#include "ossimXmlAttribute.h"
#include "ossimString.h"


class OSSIMDLLEXPORT ossimXmlNode : public ossimObject,
                                    public ossimErrorStatusInterface
{
public:
   ossimXmlNode(istream& xml_stream, ossimXmlNode* parent=0);
   ossimXmlNode();

   bool read(std::istream& in);
   // Appends any matching nodes to the list supplied
   void findChildNodes(const ossimString& rel_xpath,
                       vector<ossimRefPtr<ossimXmlNode> >& nodelist)const;
   const ossimRefPtr<ossimXmlNode> findFirstNode(const ossimString& rel_xpath)const;
   ossimRefPtr<ossimXmlNode> findFirstNode(const ossimString& rel_xpath);

   ossimRefPtr<ossimXmlAttribute> findAttribute(const ossimString& name);
   const ossimRefPtr<ossimXmlAttribute> findAttribute(const ossimString& name)const;
   void setTag(const ossimString& tag);
   ossimString                      getTag()        const;
   const ossimXmlNode*              getParentNode() const;
   ossimXmlNode*              getParentNode();
   void setParent(ossimXmlNode* parent);
   const vector<ossimRefPtr<ossimXmlNode> >&      getChildNodes() const;
   vector<ossimRefPtr<ossimXmlNode> >&      getChildNodes();
   const vector<ossimRefPtr<ossimXmlAttribute> >& getAttributes() const;
   void addAttribute(ossimRefPtr<ossimXmlAttribute> attribute);
   void addAttribute(const ossimString& name, const ossimString& value);
   ossimRefPtr<ossimXmlNode> addNode(const ossimString& relPath,
                                     const ossimString& text = "");
   ossimRefPtr<ossimXmlNode> addOrSetNode(const ossimString& relPath,
                                          const ossimString& text = "");
   void addChildNode(ossimRefPtr<ossimXmlNode> node);
   ossimRefPtr<ossimXmlNode> addChildNode(const ossimString& tagName,
                                          const ossimString& text="");
   void setText(const ossimString& text);
   ossimString                      getText()       const;

   OSSIMDLLEXPORT friend ostream& operator << (ostream& os, const ossimXmlNode* xml_node);
   OSSIMDLLEXPORT friend ostream& operator << (ostream& os, const ossimXmlNode& xml_node);

   ossimRefPtr<ossimXmlNode> removeChild(ossimRefPtr<ossimXmlNode> node);
   ossimRefPtr<ossimXmlNode> removeChild(const ossimString& tag);

   void toKwl(ossimKeywordlist& kwl,
              const ossimString& prefix="")const;
   void fromKwl(const ossimKeywordlist& kwlToConvert);
  
protected:
   ~ossimXmlNode();
   ossimString                 theTag;
   ossimXmlNode*         theParentNode;
   vector<ossimRefPtr<ossimXmlNode> >      theChildNodes;
   vector<ossimRefPtr<ossimXmlAttribute> >  theAttributes;
   ossimString                 theText;

   bool readTag(std::istream& in,
                ossimString& tag);
   bool readTextContent(std::istream& in,
                        ossimString& text);
   bool readEndTag(std::istream& in,
                   ossimString& endTag);
TYPE_DATA
};

#endif /* #ifndef ossimXmlNode_HEADER */
