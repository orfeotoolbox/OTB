//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Oscar Kramer <okramer@imagelinks.com> (ossim port by D. Burken)
//
// Description:  
//
// Contains definition of class ossimXmlNode.
// 
//*****************************************************************************
// $Id: ossimXmlNode.cpp 20747 2012-04-18 15:24:12Z gpotts $

#include <iostream>
#include <stack>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimXmlAttribute.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>

static ossimTrace traceDebug("ossimXmlNode:debug");
static const ossimString XPATH_DELIM ("/");

RTTI_DEF2(ossimXmlNode, "ossimXmlNode", ossimObject, ossimErrorStatusInterface);

static std::istream& xmlskipws(std::istream& in)
{
   int c = in.peek();
   while( !in.fail() &&
         (( (c== ' ') || (c == '\t') || (c == '\n')|| (c == '\r') || (c<0x20) || (c>=0x7f) ))
         )
   {
      in.ignore(1);
      c = in.peek();
   }
   
   return in;
}

ossimXmlNode::ossimXmlNode(istream& xml_stream, ossimXmlNode* parent)
:
theParentNode (parent),
theCDataFlag(false)
{
   read(xml_stream);
}

ossimXmlNode::ossimXmlNode()
:theParentNode(0),
theCDataFlag(false)
{
}

ossimXmlNode::ossimXmlNode(const ossimXmlNode& src)
:theTag(src.theTag),
theParentNode(0),
theText(src.theText),
theCDataFlag(src.theCDataFlag)
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < src.theChildNodes.size();++idx)
   {
      theChildNodes.push_back((ossimXmlNode*)(src.theChildNodes[idx]->dup()));
   }
   for(idx = 0; idx < src.theAttributes.size();++idx)
   {
      theAttributes.push_back((ossimXmlAttribute*)(src.theAttributes[idx]->dup()));
   }
}

ossimXmlNode::~ossimXmlNode()
{
}

void ossimXmlNode::duplicateAttributes(ossimXmlNode::AttributeListType result)const
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx<theAttributes.size();++idx)
   {
      result.push_back((ossimXmlAttribute*)theAttributes[idx]->dup());
   }
	
}

void ossimXmlNode::duplicateChildren(ossimXmlNode::ChildListType& result)const
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx<theChildNodes.size();++idx)
   {
      result.push_back((ossimXmlNode*)theChildNodes[idx]->dup());
   }
}

void ossimXmlNode::setParent(ossimXmlNode* parent)
{
   theParentNode = parent;
}

void ossimXmlNode::skipCommentTag(std::istream& in)
{
   char c;
   while(!in.fail())
   {
      c = in.get();
      if(c == '-')
      {
         if(in.peek() == '-')
         {
            in.ignore();
            if(in.peek() == '>')
            {
               in.ignore();
               return;
            }
         }
      }
   }
}

bool ossimXmlNode::read(std::istream& in)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimXmlNode::read: entered ......" << std::endl;
   }
   char c;
   xmlskipws(in);
   if(in.fail())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
      }
      return false;
   }
   if(in.peek() == '<')
   {
      in.ignore(1);
   }
   if(in.fail())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
      }
      return false;
   }

   ossimString endTag;
   
   if(!readTag(in, theTag))
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
      }
      return false;
   }
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "theTag = " << theTag << "\n";
   }
   
   if((!in.fail())&&readEndTag(in, endTag))
   {
      if((endTag == "")||
         (endTag == theTag))
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
         }
         return true;
      }
      else
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
         }
         return false;
      }
   }
   // now parse attributes
   ossimRefPtr<ossimXmlAttribute> attribute = new ossimXmlAttribute;
   while(attribute->read(in))
   {
      theAttributes.push_back(new ossimXmlAttribute(*attribute));
   }
   // skip white space characters
   //
   xmlskipws(in);
   
   if(!in.fail()&&readEndTag(in, endTag))
   {
      if((endTag == "")||
         (endTag == theTag))
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
         }
         return true;
      }
      else
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
         }
         return false;
      }
   }
   c = in.peek();
   // make sure the attribute is closed
   //
   if(c != '>')
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
      }
      return false;
   }
   
   in.ignore(1);
   c = in.peek();
   
   // now do the text portion
   if(!readTextContent(in))
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
      }
      return false;
   }
   xmlskipws(in);
   c = in.peek();
   
   if(c != '<')
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
      }
      return false;
   }
   in.ignore(1);
   if(readEndTag(in, endTag))
   {
      if((endTag == "")||
         (endTag == theTag))
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
         }
         return true;
      }
      else
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
         }
         return false;
      }
   }
   c = in.peek();

   //---
   // now if it's not an endtag then it must be a tag starting the new child
   // node
   //---
   ossimRefPtr<ossimXmlNode> childNode;
   do
   {
      childNode = new ossimXmlNode;
      childNode->setParent(this);
      if(childNode->read(in))
      {
         theChildNodes.push_back(childNode);
      }
      else
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
         }
         return false;
      }
      xmlskipws(in);
      
      c = in.peek();
      if(c != '<')
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
         }
         return false;
      }
      in.ignore(1);
      if(readEndTag(in, endTag))
      {
         if((endTag == "")||
            (endTag == theTag))
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
            }
            return true;
         }
         else
         {
            setErrorStatus();
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "ossimXmlNode::read: leaving ......\n"<<__LINE__ << "\n";
            }
            return false;
         }
      }
   }while( !in.fail() );
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimXmlNode::read: leaving ......\n";
   }
   return true;
}


void ossimXmlNode::findChildNodes(const ossimString& xpath,
                                  ossimXmlNode::ChildListType& result)const
{
   //***
   // Scan for trivial result (no children owned):
   //***
   if (theChildNodes.size() == 0)
      return;
   
   //***
   // Make a copy to manipulate:
   //***
   ossimString rel_xpath (xpath);
   if (rel_xpath.empty())
      return;
   
   //---
   // First verify that this is not an absolute path:
   //---
   if (rel_xpath[static_cast<std::string::size_type>(0)] ==
       XPATH_DELIM[static_cast<std::string::size_type>(0)])
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "WARNING: ossimXmlNode::findChildNodes\n"
            << "Only relative XPaths can be searched from a node. "
             << "Returning null list...\n";
         }
      return;
   }
   
   //***
   // Read the desired tag from the relative xpath
   //***
   ossimString desired_tag (rel_xpath);
   if (rel_xpath.contains(XPATH_DELIM))
      desired_tag = rel_xpath.before(XPATH_DELIM);
   ossimString sub_xpath (rel_xpath.after(XPATH_DELIM));
   
   //***
   // Loop over all child nodes for match:
   //***
   ossimXmlNode::ChildListType::const_iterator child_iter = theChildNodes.begin();
   while(child_iter != theChildNodes.end())
   {
      if ((*child_iter)->getTag() == desired_tag)
      {
         if (sub_xpath.empty())
         {
            //***
            // This was the final target node, simply append to the result:
            //***
            result.push_back(*child_iter);
         }
         else
         {
            //***
            // This match identifies a possible tree to search given the
            // remaining xpath (sub_xpath). Query this child node to search
            // its tree for the remaining xpath:
            //***
            (*child_iter)->findChildNodes(sub_xpath, result);
         }
      }
      
      //***
      // Proceed to next child:
      //***
      child_iter++;
   }
}

const ossimRefPtr<ossimXmlNode> ossimXmlNode::findFirstNode(const ossimString& xpath)const
{
   if(theChildNodes.size() < 1) return 0;
   //
   // Make a copy to manipulate:
   //
   ossimString rel_xpath (xpath);
   if (rel_xpath.empty())
      return 0;
   
   //
   // First verify that this is not an absolute path:
   //
   if (rel_xpath[static_cast<std::string::size_type>(0)] ==
       XPATH_DELIM[static_cast<std::string::size_type>(0)])
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
         << "WARNING: ossimXmlNode::findChildNodes\n"
         << "Only relative XPaths can be searched from a node. "
         << "Returning null list...\n";

      }
      return 0;
   }
   
   //
   // Read the desired tag from the relative xpath
   //
   ossimString desired_tag (rel_xpath);
   if (rel_xpath.contains(XPATH_DELIM))
      desired_tag = rel_xpath.before(XPATH_DELIM);
   ossimString sub_xpath (rel_xpath.after(XPATH_DELIM));
   
   ossimRefPtr<ossimXmlNode> result = 0;
   
   //
   // Loop over all child nodes for match:
   //
   ossimXmlNode::ChildListType::const_iterator child_iter = theChildNodes.begin();
   while((child_iter != theChildNodes.end())&&
         (!result.valid()))
   {
      if ((*child_iter)->getTag() == desired_tag)
      {
         if (sub_xpath.empty())
         {
            return *child_iter;
         }
         else
         {
            //
            // This match identifies a possible tree to search given the
            // remaining xpath (sub_xpath). Query this child node to search
            // its tree for the remaining xpath:
            //
            result = (*child_iter)->findFirstNode(sub_xpath);
         }
      }
      
      //
      // Proceed to next child:
      //
      ++child_iter;
   }
   
   return result;
}

ossimRefPtr<ossimXmlNode> ossimXmlNode::findFirstNode(const ossimString& xpath)
{
   if(theChildNodes.size() < 1) return 0;
   //
   // Make a copy to manipulate:
   //
   ossimString rel_xpath (xpath);
   if (rel_xpath.empty())
      return 0;
   
   //
   // First verify that this is not an absolute path:
   //
   if (rel_xpath[static_cast<std::string::size_type>(0)] ==
       XPATH_DELIM[static_cast<std::string::size_type>(0)])
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
         << "WARNING: ossimXmlNode::findChildNodes\n"
         << "Only relative XPaths can be searched from a node. "
         << "Returning null list...\n";
      }
      return 0;
   }
   
   //
   // Read the desired tag from the relative xpath
   //
   ossimString desired_tag (rel_xpath);
   if (rel_xpath.contains(XPATH_DELIM))
      desired_tag = rel_xpath.before(XPATH_DELIM);
   ossimString sub_xpath (rel_xpath.after(XPATH_DELIM));
   
   ossimRefPtr<ossimXmlNode> result = 0;
   
   //
   // Loop over all child nodes for match:
   //
   ossimXmlNode::ChildListType::iterator child_iter = theChildNodes.begin();
   while((child_iter != theChildNodes.end())&&
         (!result.valid()))
   {
      if ((*child_iter)->getTag() == desired_tag)
      {
         if (sub_xpath.empty())
         {
            return *child_iter;
         }
         else
         {
            //
            // This match identifies a possible tree to search given the
            // remaining xpath (sub_xpath). Query this child node to search
            // its tree for the remaining xpath:
            //
            result = (*child_iter)->findFirstNode(sub_xpath);
         }
      }
      
      //
      // Proceed to next child:
      //
      ++child_iter;
   }
   
   return result;
}

ossimRefPtr<ossimXmlAttribute> ossimXmlNode::findAttribute(const ossimString& name)
{
   ossim_uint32 idx = 0;
   
   for(idx = 0; idx < theAttributes.size();++idx)
   {
      if(theAttributes[idx]->getName() == name)
      {
         return theAttributes[idx];
      }
   }
   
   return 0;
}

const ossimRefPtr<ossimXmlAttribute> ossimXmlNode::findAttribute(const ossimString& name)const
{
   ossim_uint32 idx = 0;
   
   for(idx = 0; idx < theAttributes.size();++idx)
   {
      if(theAttributes[idx]->getName() == name)
      {
         return theAttributes[idx];
      }
   }
   
   return 0;
}

void ossimXmlNode::setTag(const ossimString& tag)
{
   theTag = tag;
}

ossimString ossimXmlNode::getTag() const
{
   return theTag;
}

const ossimXmlNode* ossimXmlNode::getParentNode() const
{
   return theParentNode;
}

ossimXmlNode* ossimXmlNode::getParentNode()
{
   return theParentNode;
}

const ossimXmlNode::ChildListType& ossimXmlNode::getChildNodes() const
{
   return theChildNodes;
}

ossimXmlNode::ChildListType& ossimXmlNode::getChildNodes()
{
   return theChildNodes;
}

const ossimXmlNode::AttributeListType& ossimXmlNode::getAttributes() const
{
   return theAttributes;
}

bool ossimXmlNode::getAttributeValue(ossimString& value, const ossimString& name)const
{
   ossimRefPtr<ossimXmlAttribute> attribute = findAttribute(name);
   
   if(attribute.valid())
   {
      value = attribute->getValue();
   }
   
   return attribute.valid();
}

bool ossimXmlNode::getChildTextValue(ossimString& value,
                                     const ossimString& relPath)const
{
   ossimRefPtr<ossimXmlNode> node = findFirstNode(relPath);
   if(node.valid())
   {
      value = node->getText();
   }
   
   return node.valid();
}


void ossimXmlNode::setText(const ossimString& text)
{
   theText = text;
}

const ossimString& ossimXmlNode::getText() const
{
   return theText;
}

bool ossimXmlNode::cdataFlag()const
{
   return theCDataFlag;
}

void ossimXmlNode::setCDataFlag(bool value)
{
   theCDataFlag = value;
}

ostream& operator << (ostream& os, const ossimXmlNode& xml_node)
{
   return operator <<(os, &xml_node);
}

//**************************************************************************
//  FRIEND OPERATOR
//**************************************************************************
ostream& operator << (ostream& os, const ossimXmlNode* xml_node) 
{
   //
   // Determine the indentation level:
   //
   ossimString indent ("");
   const ossimXmlNode* parent = xml_node->theParentNode;
   while (parent)
   {
      indent += "   ";
      parent = parent->theParentNode;
   }
   
   //
   // Dump the tag opening:
   //
   os << "\n" << indent << "<" << xml_node->theTag;
   
   //
   // Dump any attributes:
   //
   if (xml_node->theAttributes.size())
   {
      ossimXmlNode::AttributeListType::const_iterator attr =
      xml_node->theAttributes.begin();
      while (attr != xml_node->theAttributes.end())
      {
         os << attr->get();
         attr++;
      }
   }
   
   if((xml_node->theChildNodes.size() == 0)&&
      (xml_node->theText == ""))
   {
      os << "/>";
   }
   else
   {
      os << ">";
      
      if(xml_node->cdataFlag())
      {
         os << "<![CDATA[" <<xml_node->theText <<  "]]>";
      }
      else
      {
         //
         // Dump any text:
         //
         os << xml_node->theText;
      }
      //
      // Dump any child nodes:
      //
      if (xml_node->theChildNodes.size())
      {
         ossimXmlNode::ChildListType::const_iterator nodes = xml_node->theChildNodes.begin();
         while (nodes != xml_node->theChildNodes.end())
         {
            os << (*nodes).get();
            nodes++;
         }
         os << "\n" << indent;
      }
      
      //
      // Dump the tag closing:
      // 
      os << "</" << xml_node->theTag << ">";
   }
   
   return os;
}

void ossimXmlNode::addAttribute(ossimRefPtr<ossimXmlAttribute> attribute)
{
   theAttributes.push_back(attribute.get());
}

void ossimXmlNode::addAttribute(const ossimString& name,
                                const ossimString& value)
{
   theAttributes.push_back(new ossimXmlAttribute(name, value));
}

bool  ossimXmlNode::setAttribute(const ossimString& name,
                                 const ossimString& value,
                                 bool addIfNotPresentFlag)
{
   bool result = false;
   ossimRefPtr<ossimXmlAttribute> attribute = findAttribute(name);
   if(attribute.valid())
   {
      attribute->setValue(value);
      result = true;
   }
   else
   {
      if(addIfNotPresentFlag)
      {
         addAttribute(name, value);
         result = true;
      }
   }
   
   return result;
}

ossimRefPtr<ossimXmlNode> ossimXmlNode::addNode(const ossimString& relPath,
                                                const ossimString& text)
{
   //
   // Make a copy to manipulate:
   //
   ossimString relXpath (relPath);
   if (relXpath.empty())
      return 0;
   
   //
   // First verify that this is not an absolute path:
   //
   if (relXpath[static_cast<std::string::size_type>(0)] ==
       XPATH_DELIM[static_cast<std::string::size_type>(0)])
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimXmlNode::findChildNodes\n"
         << "Only relative XPaths can be searched from a node. "
         << "Returning null list...\n";
         
      }
      return 0;
   }
   
   //
   // Read the desired tag from the relative xpath
   //
   ossimString desiredTag (relXpath);
   if (relXpath.contains(XPATH_DELIM))
   {
      desiredTag = relXpath.before(XPATH_DELIM);
   }
   ossimString subPath (relXpath.after(XPATH_DELIM));
   
   ossimRefPtr<ossimXmlNode> node = findFirstNode(desiredTag);
   
   if(!node.valid())
   {
      if(subPath.empty())
      {
         node = addChildNode(desiredTag, text);
      }
      else
      {
         node = addChildNode(desiredTag, "");
      }
   }
   if(!subPath.empty())
   {
      return node->addNode(subPath, text);
   }
   
   return node;
}

ossimRefPtr<ossimXmlNode> ossimXmlNode::addOrSetNode(const ossimString& relPath,
                                                     const ossimString& text)
{
   
   ossimRefPtr<ossimXmlNode> result = addNode(relPath, text);
   
   result->setText(text);
   
   return result;
}

void ossimXmlNode::addChildNode(ossimRefPtr<ossimXmlNode> node)
{
   if(node->theParentNode)
   {
      node->theParentNode->removeChild(node);
   }
   node->theParentNode = this;
   theChildNodes.push_back(node.get());
}

ossimRefPtr<ossimXmlNode> ossimXmlNode::addChildNode(const ossimString& tagName,
                                                     const ossimString& text)
{
   ossimRefPtr<ossimXmlNode> node = new ossimXmlNode();
   node->setParent(this);
   node->theTag = tagName;
   node->theText = text;
   theChildNodes.push_back(node);
   
   return node;
}

ossimRefPtr<ossimXmlNode> ossimXmlNode::removeChild(ossimRefPtr<ossimXmlNode> node)
{
   ossimXmlNode::ChildListType::iterator iter = theChildNodes.begin();
   while(iter != theChildNodes.end())
   {
      
      if(node == iter->get())
      {
         ossimRefPtr<ossimXmlNode> temp = *iter;
         
         theChildNodes.erase(iter);
         
         return temp;
      }
      ++iter;
   }
   
   return 0;
}

ossimRefPtr<ossimXmlNode> ossimXmlNode::removeChild(const ossimString& tag)
{
   ossimXmlNode::ChildListType::iterator iter = theChildNodes.begin();
   while(iter != theChildNodes.end())
   {
      if(tag == iter->get()->theTag)
      {
         ossimRefPtr<ossimXmlNode> temp = *iter;
         
         theChildNodes.erase(iter);
         
         return temp;
      }
      ++iter;
   }
   
   return 0;
}
void ossimXmlNode::addChildren(ossimXmlNode::ChildListType& children)
{
   ossim_uint32 idx;
   for(idx = 0; idx < children.size(); ++idx)
   {
      addChildNode(children[idx].get());
   }
}

void ossimXmlNode::setChildren(ossimXmlNode::ChildListType& children)
{
   clearChildren();
   addChildren(children);
}

void ossimXmlNode::addAttributes(ossimXmlNode::AttributeListType& children)
{
   ossim_uint32 idx;
   
   for(idx = 0; idx < children.size(); ++idx)
   {
      addAttribute(children[idx].get());
   }
}

void ossimXmlNode::setAttributes(ossimXmlNode::AttributeListType& children)
{
   clearAttributes();
   addAttributes(children);
}

void ossimXmlNode::clear()
{
   theChildNodes.clear();
   theAttributes.clear();
   theTag="";
   theText="";
   theCDataFlag=false;
}

void ossimXmlNode::clearChildren()
{
   theChildNodes.clear();
}

void ossimXmlNode::clearAttributes()
{
   theAttributes.clear();
}

void ossimXmlNode::toKwl(ossimKeywordlist& kwl,
                         const ossimString& prefix)const
{
   ossimString name = getTag();
   ossimString value = getText();
   
   ossimString copyPrefix = prefix;
   
   if(name != "")
   {
      copyPrefix += (name+".");
   }
   if(theChildNodes.size() < 1)
   {
      kwl.add(prefix+name,
              value,
              true);
   }

   ossimString attributePrefix = copyPrefix + "@";
   ossim_uint32 attributeIdx = 0;
   for(attributeIdx = 0; attributeIdx < theAttributes.size(); ++attributeIdx)
   {
      kwl.add(attributePrefix+theAttributes[attributeIdx]->getName(),
              theAttributes[attributeIdx]->getValue(), 
              true);
   }

   ossim_uint32 idx = 0;
   for(idx = 0; idx < theChildNodes.size();++idx)
   {
      theChildNodes[idx]->toKwl(kwl,
                                copyPrefix);
   }
   
}

bool ossimXmlNode::readTag(std::istream& in,
                           ossimString& tag)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimXmlNode::readTag: entered ......\n";
   }
   xmlskipws(in);
   
   tag = "";
   int c = in.peek();
   
   // bool validTag = false;
   //    while(!validTag)
   {
      while( (c != ' ')&&
            (c != '\n')&&
            (c != '\t')&&
            (c != '\r')&&
            (c != '<')&&
            (c != '>')&&
            (c != '/')&&
            (!in.fail()))
      {
         tag += (char)c;
         in.ignore(1);
         c = in.peek();
         if(tag == "!--") // ignore comment tags
         {
            tag = "--";
            break;
         }
      }
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimXmlNode::readTag: leaving ......\n";
   }
  
   return ((tag != "")&&(!in.fail()));
}

bool ossimXmlNode::readCDataContent(std::istream& in)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimXmlNode::readCDataContent: entered ..." << std::endl;
   }   
   
   // Ignore up to "]]>"
   
   bool result = false;

   char c;

   while(!in.fail())
   {
      c = in.get();
      if ( c != ']' )
      {
         theText += c;
      }
      else // at "]"
      {
         c = in.get();
         if( c == ']' ) // at "]]"
         {
            c = in.get();
            if( c == '>' )
            {
               //in >> xmlskipws;
               result = true;
               break;
            }
         }
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "theText: " << theText
         << "\nexit status: " << (result?"true":"false")
         << "\nossimXmlNode::readCDataContent: leaving ..." << std::endl;
   }
   
   return result;
}

#if 0
bool ossimXmlNode::readTextContent(std::istream& in)
{
   xmlskipws(in);
   
   theText = "";
   theCDataFlag = false;
   
   char buf[9];
   buf[8]='\0';
   
   std::streampos initialPos = in.tellg();
   
   in.read(buf,9);
   ossimString ostrBuf(buf);
   
   if(ostrBuf == "<![CDATA[")
   {
	   if(readCDataContent(in))
      {
         theCDataFlag = true;
         return true;
      }
	   else
	   {
         return false;
	   }
   }
   else if(ostrBuf.substr(0,4) == "<!--")
   {
	   in.seekg(initialPos);
	   char c = in.get();
	   // Strip comment
	   while(!in.fail()) // continue until we see a --> pattern
	   {
         c = in.get();
         if(c == '-')
         {
            c = in.get();
            if(c == '-')
            {
               c = in.get();
               if(c == '>')
               {
                  break;
               }
            }
         }
	   }
   }
   else if(ostrBuf.substr(0,1) ==  "<")
   {
	   in.seekg(initialPos);
   }
   else
   {
      in.seekg(initialPos);
	   char c = in.peek();
	   while(!in.fail() && c != '<')
	   {
         theText += (char)in.get();
         c = in.peek();
	   }
   }
   return !in.fail();
}
#endif
#if 0
bool ossimXmlNode::readTextContent(std::istream& in)
{
   xmlskipws(in);
   
   theText = "";
   theCDataFlag = false;
   char c = in.peek();
   
   do
   {
      if(c == '<')
      {
         in.ignore();
         
         // we will check for comments or CDATA
         if(in.peek()=='!')
         {
            char buf1[4];
            buf1[3] = '\0';
            in.read(buf1, 3);
            if(ossimString(buf1) == "!--")
            {
               // special text read
               theText += buf1;
               bool done = false;
               do
               {
                  if(in.peek() != '-')
                  {
                     in.ignore();
                  }
                  else
                  {
                     in.ignore();
                     if(in.peek() == '-')
                     {
                        in.ignore();
                        if(in.peek() == '>')
                        {
                           in.ignore();
                           done = true;
                           c = in.peek();
                        }
                     }
                  }
               }while(!done&&!in.fail());
            }
            else
            {
               
               char buf2[6];
               buf2[5] = '\0';
               in.read(buf2, 5);
               if(in.fail())
               {
                  return false;
               }
               if(ossimString(buf1)+ossimString(buf2) == "![CDATA[")
               {
                  if(readCDataContent(in))
                  {
                     theCDataFlag = true;
                     return true;
                  }
               }
            }
         }
         else
         {
            in.putback(c);
            return true;
         }
      }
      else
      {
         theText += (char)in.get();
         c = in.peek();
      }
   }while(!in.fail());
   
   return !in.fail();
}
#endif

bool ossimXmlNode::readTextContent(std::istream& in)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimXmlNode::readTextContent: entered ..." << std::endl;
   }
   
   //---
   // Parse the text string.  Do it with no peeks, ignores, or putbacks as
   // those seem to have issues on Windows (vs9).
   //---
   bool result = false;

   theText = "";
   theCDataFlag = false;
   
   xmlskipws(in);

   if ( !in.fail() )
   {
      std::streampos initialPos = in.tellg();

      char c = in.get();
      
      if ( c != '<' )
      {
         do // Get the text up to the next '<'.
         {
            theText += c;
            c = in.get();
         } while ( (c != '<') && !in.fail() );
         
         in.unget(); // Put '<' back.
         result = !in.fail();
      }
      else // At "<" see if comment
      {
         c = in.get();

         if ( c != '!' )
         {
            in.seekg(initialPos);
            result = !in.fail();
         }
         else // at "<!"
         {
            c = in.get();
            if ( c == '-' )
            {
               // Comment section: <!-- some comment -->
               c = in.get();
               if ( c == '-' ) // at "<!--"
               {
                  // Strip comment
                  while( !in.fail() ) // continue until we see a --> pattern
                  {
                     c = in.get();
                     if(c == '-')
                     {
                        c = in.get();
                        if(c == '-')
                        {
                           c = in.get();
                           if(c == '>')
                           {
                              result = !in.fail();
                              break;
                           }
                        }
                     }
                  }
               }
            }
            else if ( c == '[' ) // at "<!["
            {
               // CDATA section: <![CDATA[something-here]]>
               c = in.get();
               if ( c == 'C') // at "<![C:"
               {
                  c = in.get();
                  if ( c == 'D' )// at "<![CD"
                  {
                     c = in.get();
                     if ( c == 'A' ) // at "<![CDA"
                     {
                        c = in.get();
                        if ( c == 'T' ) // at "<![CDAT"
                        {
                           c = in.get();
                           if ( c == 'A' ) // at "<![CDATA"
                           {
                              c = in.get();
                              if ( c == '[' ) // at "<!CDATA["
                              {
                                 if (readCDataContent(in))
                                 {
                                    theCDataFlag = true;
                                    result = true;
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "theText: " << theText
         << "\ntheCDataFlag: " << (theCDataFlag?"true":"false")
         << "\nexit status: " << (result?"true":"false")
         << "\nossimXmlNode::readTextContent: leaving ..." << std::endl;
   }
   
   return result;
}

bool ossimXmlNode::readEndTag(std::istream& in,
                              ossimString& endTag)
{
   bool result = false;
   char c = in.peek();
   endTag = "";
   
   if(theTag == "--")// this is a comment tag
   {
      skipCommentTag(in);
      endTag = "--";
      return (!in.fail());
   }
   // check end tag
   //
   if(c == '/')
   {
      in.ignore();
      readTag(in, endTag);
      if(in.fail()) return false;
      xmlskipws(in);
      c = in.peek();
      result = true;
   }
   else
   {
      return false;
   }
   if(c != '>')
   {
      setErrorStatus();
      return false;
   }
   else
   {
      in.ignore(1);
   }
   if(in.fail()) result = false;
   
   return result;
}
