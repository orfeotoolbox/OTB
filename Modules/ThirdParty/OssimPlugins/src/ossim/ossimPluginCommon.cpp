/*
 * Copyright (C) 2010-2016 by David Burken
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <cstdlib>

#include <ossimPluginCommon.h>
#include <ossim/base/ossimDate.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>
#include <otb/CivilDateTime.h>
#include <ossim/base/ossimTrace.h>
static ossimTrace traceDebug("ossimPluginCommon:debug");
namespace ossimplugins
{


bool ossim::iso8601TimeStringToCivilDate(const std::string& dateString,
                                         CivilDateTime& outputDate)
{
   bool result = true;

   ossimLocalTm otm;
   if ( otm.setIso8601(dateString) )
   {
      outputDate.set_year( otm.getYear() );
      outputDate.set_month( otm.getMonth() );
      outputDate.set_day( otm.getDay() );

      int second = otm.tm_hour * 3600 + otm.tm_min * 60 + otm.tm_sec;
      outputDate.set_second( second );
      outputDate.set_decimal( otm.getFractionalSecond() );

   }
   else
   {
      result = false;
   }

   return result;
}


bool ossim::getPath(const ossimString& path,
                    const ossimXmlDocument* xdoc,
                    ossimString& s)
{
   bool result = false;
   if (xdoc)
   {
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      xdoc->findNodes(path, xnodes);
      if ( xnodes.size() == 1 ) // Error if more than one.
      {
         if ( xnodes[0].valid() )
         {
            s = xnodes[0]->getText();
            result = true;
         }
         else
         {
            if(traceDebug())
            {
               
               ossimNotify(ossimNotifyLevel_WARN)
               << "ossim::getPath ERROR:\n"
               << "Node not found: " << path
               << std::endl;
            }
         }
      }
      else if ( xnodes.size() == 0 )
      {
         if(traceDebug())
         {
           ossimNotify(ossimNotifyLevel_WARN)
               << "ossim::getPath ERROR:\n"
               << "Node not found: " << path
            << std::endl;
         }
      }
      else
      {
         if(traceDebug())
         {

            ossimNotify(ossimNotifyLevel_WARN)
               << "ossim::getPath ERROR:\n"
               << "Multiple nodes found: " << path
               << std::endl;
         }
      }
   }
   if (!result)
   {
      s.clear();
   }
   return result;
}

bool ossim::getPath(const ossimString& path,
                    const ossimXmlDocument* xdoc,
                    std::vector<ossimString>& v)
{
   bool result = false;
   if (xdoc)
   {
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      xdoc->findNodes(path, xnodes);
      if ( xnodes.size() )
      {
         std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator i =
            xnodes.begin();
         while ( i != xnodes.end() )
         {
            v.push_back( (*i)->getText() );
            ++i;
         }
         result = true;
      }
      else
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
            << "ossim::getPath ERROR:\n"
            << "Nodes not found: " << path
            << std::endl;
         }
      }
   }
   if (!result)
   {
      v.clear();
   }
   return result;
}

bool ossim::findFirstNode(const ossimString& path,
                          ossimRefPtr<ossimXmlNode> node,
                          ossimString& s)
{
   bool result = false;
   if ( node.valid() )
   {
      ossimRefPtr<ossimXmlNode> n = node->findFirstNode(path);
      if ( n.valid() )
      {
         s = n->getText();
         if ( s.size() )
         {
            result = true;
         }
         else
         {
            if(!traceDebug())
            {
               ossimNotify(ossimNotifyLevel_WARN)
               << "ossim::findFirstNode ERROR:\n"
               << "Node empty: " << path
               << std::endl;
            }
         }
      }
      else
      {
         if(!traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossim::findFirstNode ERROR:\n"
               << "Node not found: " << path
               << std::endl;
         }
      }
   }
   return result;
}
}
