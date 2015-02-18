//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Common code for this plugin.
//
//----------------------------------------------------------------------------
// $Id$

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
