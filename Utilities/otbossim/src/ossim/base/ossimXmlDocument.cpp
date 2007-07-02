//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.  All rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer (ossim port by D. Burken)
//
// Description:  
//
// Contains definition of class ossimXmlDocument. This class provides read-only
// parsing and accessing of an XML document file.
//*****************************************************************************
// $Id: ossimXmlDocument.cpp 10425 2007-02-06 19:00:11Z gpotts $

#include <stack>
#include <iostream>
#include <fstream>
using namespace std;

#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlAttribute.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRegExp.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimKeywordNames.h>

//
// Static trace for debugging
//
#include <ossim/base/ossimTrace.h>
static ossimTrace traceDebug("ossimXmlDocument:debug");

static std::istream& xmlskipws(std::istream& in)
{
   int c = in.peek();
   while((!in.fail())&&
         ((c == ' ') ||
          (c == '\t') ||
          (c == '\n')|
          (c == '\r')))
   {
      in.ignore(1);
      c = in.peek();
   }
   
   return in;
}

static const int BUFFER_MAX_LEN = 1000;
static const ossimString XPATH_DELIM ("/");

RTTI_DEF1(ossimXmlDocument, "ossimXmlDocument", ossimObject)
ossimXmlDocument::ossimXmlDocument(const ossimFilename& xmlFileName)
   :
   theRootNode  (0),
   theXmlHeader("<?xml version='1.0'?>")
{

   if(xmlFileName != "")
   {
      openFile(xmlFileName);
   }
}

ossimXmlDocument::~ossimXmlDocument()
{
}

bool ossimXmlDocument::write(const ossimFilename& file)
{
   std::ofstream out(file);

   if(out)
   {
      out << *this << std::endl;
   }
   else
   {
      return false;
   }

   return true;
}

bool ossimXmlDocument::openFile(const ossimFilename& filename)
{
   
   theFilename = filename;

   if(theFilename == "")
   {
      return false;
   }

   //
   // Open XML File:
   // Note: Opening text document binary to overcome an apparent windows bug.
   //
   ifstream xml_stream (filename.c_str(), ios::binary);
   if (!xml_stream)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG: ossimXmlDocument::ossimXmlDocument\n"
            << "encountered opening file <" << filename << "> for "
            << "reading. Aborting..." << endl;
      }
      return false;
   }

   return read(xml_stream);
}

bool ossimXmlDocument::read(std::istream& in)
{
//   char buffer[BUFFER_MAX_LEN];
//   streampos file_pos;
//   bool readingHeader = true;
   bool startTagCharacterFound = false;
   char c = in.peek();


   // initially we will do our own skipping to make sure we ar not binary
   //
   while(!in.bad()&&
         (c != '<')&&
         (c >= 0x20)&&(c <= 0x7e))
   {
      in.ignore(1);
      c = in.peek();
   }

   if (in.bad()||
       (c!='<'))
   {
      setErrorStatus();
      return false;
   }
   startTagCharacterFound = true;

   theXmlHeader = "";
   if(readHeader(in))
   {
      if(theXmlHeader=="")
      {
         if(startTagCharacterFound)
         {
            theXmlHeader = "<?xml version='1.0'?>";
         }
         else
         {
            setErrorStatus();
            return false;
         }
      }
   }
   if((!theXmlHeader.contains("xml version"))&&
      (!startTagCharacterFound))
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "FATAL: ossimXmlDocument::ossimXmlDocument"
            << "encountered parsing XML file <" << theFilename
            << ">. The file does not appear to be XML v1.0. \n"
            << "Header = \n" << theXmlHeader <<"\n"
            << endl;
      }
      setErrorStatus();
      return false;
   }
   theRootNode = new ossimXmlNode(in, 0);
   setErrorStatus(theRootNode->getErrorStatus());
   return (getErrorStatus()==ossimErrorCodes::OSSIM_OK);
}

void ossimXmlDocument::findNodes(const ossimString& arg_xpath,
                            vector<ossimRefPtr<ossimXmlNode> >& result) const
{
   //
   // First verify the root node exists:
   //
   if (!theRootNode.valid())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "WARNING: ossimXmlDocument::findNodes,\n"
            << "No root node has been instantiated. Returning null "
            << "node list..." << endl;
      }
      return;
   }

   //
   // Make a copy to manipulate:
   //
   ossimString xpath (arg_xpath);
   if (xpath.empty())
      return;
   
   //
   // Check if absolute path:
   //
   if (xpath[0] != XPATH_DELIM[0])
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "WARNING: ossimXmlDocument::findNodes\n"
            << "Only absolute XPaths are supported. Returning null "
            << "node list..." << endl;
      }
      return;
   }

   //
   // Check that root tag matches path root:
   //
   ossimString rel_xpath (xpath.after(XPATH_DELIM));
   ossimString root_tag (rel_xpath);
   if (root_tag.contains(XPATH_DELIM))
       root_tag = rel_xpath.before(XPATH_DELIM);
   
   if (root_tag != theRootNode->getTag())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "WARNING: ossimXmlDocument::findNodes\n"
            << "XPath's root node <"<<root_tag<<"> does not match the "
            << "stored root node's tag <" << theRootNode->getTag() << ">. "
            << "Returning null node list..." << endl;
      }
      return;
   }

   //
   // If the root node was the requested node, return it alone:
   //
   rel_xpath = rel_xpath.after(XPATH_DELIM);
   if (rel_xpath.empty())
   {
      result.push_back(theRootNode);
      return;
   }
   
   //
   // Pass the node request on to the root node with the relative path:
   //
   theRootNode->findChildNodes(rel_xpath, result);
}

ostream& operator << (ostream& os, const ossimXmlDocument& xml_doc) 
{
   os << xml_doc.theXmlHeader << endl;
   if (xml_doc.theRootNode.valid())
   {
      os << (xml_doc.theRootNode.get()) << endl;
   }
//    else
//       os << "-- no root node assigned -- " << endl;
   
   return os;
}

void ossimXmlDocument::initRoot(ossimRefPtr<ossimXmlNode> node)
{
   theRootNode = node;
}

ossimRefPtr<ossimXmlNode> ossimXmlDocument::getRoot()
{
   return theRootNode;
}

const ossimRefPtr<ossimXmlNode> ossimXmlDocument::getRoot()const
{
   return theRootNode;
}

ossimRefPtr<ossimXmlNode> ossimXmlDocument::removeRoot()
{
   ossimRefPtr<ossimXmlNode> root = theRootNode;

   theRootNode = 0;

   return root;
}


void ossimXmlDocument::fromKwl(const ossimKeywordlist& kwlToConvert)
{

   ossimKeywordlist kwl = kwlToConvert;
   theRootNode = 0;
   theRootNode = new ossimXmlNode;

   
   ossimString prefix = "";
   ossimKeywordlist::KeywordMap& map              = kwl.getMap();
   ossimKeywordlist::KeywordMap::iterator mapIter = map.begin();

   while(mapIter != map.end())
   {
      ossimString key = mapIter->first;
      key = key.substitute(".", "/", true);
      theRootNode->addNode(key, mapIter->second);
      ++mapIter;
   }

   // now collapse all keywordlist styles to the XML style
   //
   std::stack<ossimRefPtr<ossimXmlNode> > tempStack;
   tempStack.push(theRootNode);
   while(!tempStack.empty())
   {
      
      ossimRefPtr<ossimXmlNode> node = tempStack.top();
      tempStack.pop();
      vector<ossimRefPtr<ossimXmlNode> >& childNodes = node->getChildNodes();
      ossim_uint32 idx = 0;
      for(idx = 0; idx < childNodes.size(); ++idx)
      {
         tempStack.push(childNodes[idx]);
      }
      if(node->getChildNodes().size() > 0)
      {
         if(node->getTag()!="")
         {
            node->addAttribute("name", node->getTag());
         }
         node->setTag("object");
      }
      
      if(node->getTag() == "type")
      {
         if(node->getParentNode())
         {
            node->getParentNode()->removeChild("type");
            node->getParentNode()->addAttribute("type", node->getText());
         }
      }
      else if(node->getChildNodes().size() < 1)
      {
         if(node->getTag()!="")
         {
            node->addAttribute("name", node->getTag());
         }
         if(!node->getText().contains("\n"))
         {
            if(node->getText()!="")
            {
               node->addAttribute("value", node->getText());
               node->setText("");
            }
         }
         node->setTag("property");
      }
   }
}

void ossimXmlDocument::toKwl(ossimKeywordlist& kwl ,
                             const ossimString& prefix)const
{
   
//    const std::vector<ossimRefPtr<ossimXmlNode> >& children = theRootNode->getChildNodes();
   
//    ossim_uint32 idx = 0;
   
//    for(idx = 0; idx < children.size(); ++idx)
//    {
//       children[idx]->toKwl(kwl, prefix);
//    }
   theRootNode->toKwl(kwl, prefix);
}

bool ossimXmlDocument::readHeader(std::istream& in)
{
   char c;
   in>>xmlskipws;

   while(in.peek() == '<')
   {
      in.ignore(1);
      c = in.peek();
      if((c == '?')||
         (c == '!'))
      {
         theXmlHeader += "<";
         theXmlHeader += (char)in.get();
         
         while((in.peek() != '>')&&
               (!in.bad()))
         {
            theXmlHeader += (char)in.get();
         }
         if(!in.bad())
         {
            theXmlHeader += (char)in.get();
         }
         in>>xmlskipws;
      }
   }

   return (!in.bad());
}
