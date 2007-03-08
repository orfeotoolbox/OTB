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
// $Id: ossimXmlNode.cpp 10425 2007-02-06 19:00:11Z gpotts $

#include <iostream>
using namespace std;

#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimXmlAttribute.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotifyContext.h>

static const ossimString XPATH_DELIM ("/");

RTTI_DEF2(ossimXmlNode, "ossimXmlNode", ossimObject, ossimErrorStatusInterface);

static std::istream& xmlskipws(std::istream& in)
{
   int c = in.peek();
   while(!in.fail()&&
         (c == ' ') ||
         (c == '\t') ||
         (c == '\n')||
	 (c == '\r'))
   {
      in.ignore(1);
      c = in.peek();
   }
   
   return in;
}


ossimXmlNode::ossimXmlNode(istream& xml_stream, ossimXmlNode* parent)
   :
   theParentNode (parent)
{
   read(xml_stream);
}

ossimXmlNode::ossimXmlNode()
   :theParentNode(0)
{
}

ossimXmlNode::~ossimXmlNode()
{
}

void ossimXmlNode::setParent(ossimXmlNode* parent)
{
   theParentNode = parent;
}

bool ossimXmlNode::read(std::istream& in)
{
   
   in >> xmlskipws;
   if(in.fail()) return false;
   if(in.peek() == '<')
   {
      in.ignore(1);
   }
   if(in.fail()) return false;
   streampos file_pos = in.tellg();
   ossimString endTag;

   
   if(!readTag(in, theTag)) return false;
   char c = in.peek();

   if((!in.fail())&&readEndTag(in, endTag))
   {
      if((endTag == "")||
         (endTag == theTag))
      {
         return true;
      }
      else
      {
         setErrorStatus();
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
   in >> xmlskipws;

   c = in.peek();

   if(!in.fail()&&readEndTag(in, endTag))
   {
      if((endTag == "")||
         (endTag == theTag))
      {
         return true;
      }
      else
      {
         setErrorStatus();
         return false;
      }
   }
   c = in.peek();
   // make sure the attribute is closed
   //
   if(c != '>')
   {
      setErrorStatus();
      return false;
   }

   in.ignore(1);
   c = in.peek();
   // now do the text portion
   //
   theText = "";

   readTextContent(in, theText);
   in >> xmlskipws;
   c = in.peek();

   c = in.peek();

   if(c != '<')
   {
      setErrorStatus();
      return false;
   }
   in.ignore(1);
   if(readEndTag(in, endTag))
   {
      if((endTag == "")||
         (endTag == theTag))
      {
         return true;
      }
      else
      {
         setErrorStatus();
         return false;
      }
   }
   c = in.peek();
   // now if it's not an endtag then it must be a tag starting the new child node
   //
   bool done = false;

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
         return false;
      }
      in >> xmlskipws;

      c = in.peek();
      if(c != '<')
      {
         setErrorStatus();
         return false;
      }
      in.ignore(1);
      if(readEndTag(in, endTag))
      {
         if((endTag == "")||
            (endTag == theTag))
         {
            return true;
         }
         else
         {
            setErrorStatus();
            return false;
         }
      }
   }while((!done)&&(!in.bad()));

   return true;
}


void ossimXmlNode::findChildNodes(const ossimString& xpath,
                                  vector<ossimRefPtr<ossimXmlNode> >& result)const
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
   
   //***
   // First verify that this is not an absolute path:
   //***
   if (rel_xpath[0] == XPATH_DELIM[0])
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimXmlNode::findChildNodes\n"
                                         << "Only relative XPaths can be searched from a node. "
                                         << "Returning null list...\n";
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
   vector<ossimRefPtr<ossimXmlNode> >::const_iterator child_iter = theChildNodes.begin();
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
   if (rel_xpath[0] == XPATH_DELIM[0])
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimXmlNode::findChildNodes\n"
                                         << "Only relative XPaths can be searched from a node. "
                                         << "Returning null list...\n";
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
   vector<ossimRefPtr<ossimXmlNode> >::const_iterator child_iter = theChildNodes.begin();
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
   if (rel_xpath[0] == XPATH_DELIM[0])
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimXmlNode::findChildNodes\n"
                                         << "Only relative XPaths can be searched from a node. "
                                         << "Returning null list...\n";
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
   vector<ossimRefPtr<ossimXmlNode> >::iterator child_iter = theChildNodes.begin();
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

const vector<ossimRefPtr<ossimXmlNode> >& ossimXmlNode::getChildNodes() const
{
   return theChildNodes;
}

vector<ossimRefPtr<ossimXmlNode> >& ossimXmlNode::getChildNodes()
{
   return theChildNodes;
}

const vector<ossimRefPtr<ossimXmlAttribute> >& ossimXmlNode::getAttributes() const
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

ossimString ossimXmlNode::getText() const
{
   return theText;
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
      vector<ossimRefPtr<ossimXmlAttribute> >::const_iterator attr =
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
      
      //
      // Dump any text:
      //
      os << xml_node->theText;
      
      //
      // Dump any child nodes:
      //
      if (xml_node->theChildNodes.size())
      {
         vector<ossimRefPtr<ossimXmlNode> >::const_iterator nodes = xml_node->theChildNodes.begin();
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
   theAttributes.push_back(attribute);
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
   if (relXpath[0] == XPATH_DELIM[0])
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING: ossimXmlNode::findChildNodes\n"
                                         << "Only relative XPaths can be searched from a node. "
                                         << "Returning null list...\n";
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
   theChildNodes.push_back(node);
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
   vector<ossimRefPtr<ossimXmlNode> >::iterator iter = theChildNodes.begin();
   while(iter != theChildNodes.end())
   {
      
      if(node.get() == iter->get())
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
   vector<ossimRefPtr<ossimXmlNode> >::iterator iter = theChildNodes.begin();
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

void ossimXmlNode::toKwl(ossimKeywordlist& kwl,
                         const ossimString& prefix)const
{
   ossimRefPtr<ossimXmlAttribute> nameAtt  = findAttribute("name");
   ossimRefPtr<ossimXmlAttribute> valueAtt = findAttribute("value");
   ossimRefPtr<ossimXmlAttribute> typeAtt  = findAttribute("type");

   ossimString name;
   ossimString value;
   ossimString type;
   if(nameAtt.valid())
   {
      name = nameAtt->getValue();
   }
   if(valueAtt.valid())
   {
      value = valueAtt->getValue();
   }
   else
   {
      value = theText;
   }
   
   if(typeAtt.valid())
   {
      type = typeAtt->getValue();
   }
   ossimString copyPrefix = prefix;

   if(name != "")
   {
      copyPrefix += (name+".");
   }
   if(type!= "")
   {
      kwl.add(copyPrefix,
              "type",
              typeAtt->getValue(),
              true);
   }
   
   if(theChildNodes.size() < 1)
   {
      kwl.add(prefix+name,
              value,
              true);
   }
   else
   {
      ossim_uint32 idx=0;

      for(idx = 0; idx < theChildNodes.size();++idx)
      {
         theChildNodes[idx]->toKwl(kwl,
                                   copyPrefix);
      }
   }
}

bool ossimXmlNode::readTag(std::istream& in,
                           ossimString& tag)
{
   in >> xmlskipws;

   tag = "";
   int c = in.peek();
   
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
   }

   return ((tag != "")&&(!in.fail()));
}

bool ossimXmlNode::readTextContent(std::istream& in,
                                   ossimString& text)
{
   in >> xmlskipws;

   text = "";
   char c = in.peek();
   bool done = false;

   do
   {
      if(c == '<')
      {
         done = true;
      }
      else
      {
         text += (char)in.get();
         c = in.peek();
      }
   }while((!in.bad())&&
          (!done));

   return (text != "");
}

bool ossimXmlNode::readEndTag(std::istream& in,
                              ossimString& endTag)
{
   bool result = false;
   char c = in.peek();
   endTag = "";

   // check end tag
   //
   if(c == '/')
   {
      in.ignore();
      readTag(in, endTag);
      if(in.fail()) return false;
      in >> xmlskipws;
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
