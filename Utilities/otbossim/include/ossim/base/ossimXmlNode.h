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
// $Id: ossimXmlNode.h 12527 2008-03-04 17:02:55Z gpotts $
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
   typedef std::vector<ossimRefPtr<ossimXmlNode> > ChildListType;
   typedef std::vector<ossimRefPtr<ossimXmlAttribute> > AttributeListType;
   
   ossimXmlNode(istream& xml_stream, ossimXmlNode* parent=0);
   ossimXmlNode();
   ossimXmlNode(const ossimXmlNode& src);
   virtual ossimObject* dup() const
   {
      return new ossimXmlNode(*this);
   }
	void duplicateAttributes(ossimXmlNode::AttributeListType result)const;
   void duplicateChildren(ossimXmlNode::ChildListType& result)const;
   bool read(std::istream& in);
   // Appends any matching nodes to the list supplied
   void findChildNodes(const ossimString& rel_xpath,
                       ossimXmlNode::ChildListType& nodelist)const;
   const ossimRefPtr<ossimXmlNode> findFirstNode(const ossimString& rel_xpath)const;
   ossimRefPtr<ossimXmlNode> findFirstNode(const ossimString& rel_xpath);

   ossimRefPtr<ossimXmlAttribute> findAttribute(const ossimString& name);
   const ossimRefPtr<ossimXmlAttribute> findAttribute(const ossimString& name)const;
   void setTag(const ossimString& tag);
   ossimString                      getTag()        const;
   const ossimXmlNode*              getParentNode() const;
   ossimXmlNode*              getParentNode();
   void setParent(ossimXmlNode* parent);
   const ossimXmlNode::ChildListType&      getChildNodes() const;
   ossimXmlNode::ChildListType&      getChildNodes();
   const  ossimXmlNode::AttributeListType& getAttributes() const;
   bool getAttributeValue(ossimString& value, const ossimString& name)const;
   ossimString getAttributeValue(const ossimString& name)const
   {
      ossimString value;
      getAttributeValue(value, name);
      return value;
   }
   bool getChildTextValue(ossimString& value,
                          const ossimString& relPath)const;
   ossimString getChildTextValue(const ossimString& relPath)const
   {
      ossimString value;
      getChildTextValue(value, relPath);
      return value;
   }
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
   void addChildren(ossimXmlNode::ChildListType& children);
   void setChildren(ossimXmlNode::ChildListType& children);
   void addAttributes(ossimXmlNode::AttributeListType& children);
   void setAttributes(ossimXmlNode::AttributeListType& children);
   void setText(const ossimString& text);
   const ossimString&                      getText()       const;
   bool cdataFlag()const;
   void setCDataFlag(bool value);
   OSSIMDLLEXPORT friend ostream& operator << (ostream& os, const ossimXmlNode* xml_node);
   OSSIMDLLEXPORT friend ostream& operator << (ostream& os, const ossimXmlNode& xml_node);

   ossimRefPtr<ossimXmlNode> removeChild(ossimRefPtr<ossimXmlNode> node);
   ossimRefPtr<ossimXmlNode> removeChild(const ossimString& tag);
   void clear();
   void clearChildren();
   void clearAttributes();
   
   void toKwl(ossimKeywordlist& kwl,
              const ossimString& prefix="")const;
   void fromKwl(const ossimKeywordlist& kwlToConvert);
  
protected:
   ~ossimXmlNode();
   bool readTag(std::istream& in,
                ossimString& tag);
   bool readTextContent(std::istream& in);
   bool readEndTag(std::istream& in,
                   ossimString& endTag);

   void skipCommentTag(std::istream& in);
   bool readCDataContent(std::istream& in);
   ossimString                 theTag;
   ossimXmlNode*         theParentNode;
   vector<ossimRefPtr<ossimXmlNode> >      theChildNodes;
   vector<ossimRefPtr<ossimXmlAttribute> >  theAttributes;
   ossimString                 theText;
   bool                        theCDataFlag;
/*    ossimString                 theCData; */

TYPE_DATA
};

#endif /* #ifndef ossimXmlNode_HEADER */
