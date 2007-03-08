//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Oscar Kramer <okramer@imagelinks.com> (ossim port by D. Burken)
//
// Description:  
//
// Contains declaration of class ossimXmlNode.
// 
//*****************************************************************************
// $Id: ossimXmlNode.h 10421 2007-02-05 04:19:37Z gpotts $
#ifndef ossimXmlNode_HEADER
#define ossimXmlNode_HEADER

#include <vector>
using namespace std;

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimXmlAttribute.h>
#include <ossim/base/ossimString.h>


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
   bool getAttributeValue(ossimString& value, const ossimString& name)const;
   bool getChildTextValue(ossimString& value,
                          const ossimString& relPath)const;
   void addAttribute(ossimRefPtr<ossimXmlAttribute> attribute);
   void addAttribute(const ossimString& name, const ossimString& value);
   bool setAttribute(const ossimString& name,
                     const ossimString& value,
                     bool addIfNotPresentFlag=false);
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
