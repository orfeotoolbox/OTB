//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Mingjie Su
//
// Description: Rpf Info object.
// 
//----------------------------------------------------------------------------
// $Id: ossimRpfInfo.cpp 1237 2010-08-05 19:50:27Z ming.su $

//ossim includes
#include <ossim/base/ossimTrace.h>
#include <ossim/support_data/ossimRpfInfo.h>
#include <ossim/support_data/ossimInfoFactory.h>

// Static trace for debugging
static ossimTrace traceDebug("ossimRpfInfo:debug");
static ossimTrace traceDump("ossimRpfInfo:dump"); // This will dump offsets.

ossimRpfInfo::ossimRpfInfo()
   : ossimInfoBase(),
     theFile(),
     m_infoFile()
{
}

ossimRpfInfo::~ossimRpfInfo()
{
}

bool ossimRpfInfo::open(const ossimFilename& file)
{
   theFile = file;
   if (isOpen())
   {
     std::ifstream in((theFile).c_str() );

     std::string line;
     int index = 0;
     while(in.good())
     {
       // Read in a line.
       std::getline(in, line);
       ossimString tmpStr = ossimString(line);
       if (index > 0)
       {
         if (!tmpStr.empty())
         {
           std::vector<ossimString> tmpVector = tmpStr.split("|");
           if (tmpVector.size() > 0)
           {
             m_infoFile = tmpVector[0];
             break;
           }
         }
       }
       index++;
     }
     in.close();

     return true;
   }
   return false;
}

bool ossimRpfInfo::isOpen()
{
  ossimString ext = theFile.ext().downcase();

  if(ext == "rpf")
  {
    return true;
  }
  else
  {
    return false;
  }
}

std::ostream& ossimRpfInfo::print(std::ostream& out) const
{
  ossimInfoBase* info = ossimInfoFactory::instance()->create(m_infoFile);
  if (info)
  {
    info->print(out);
  }

  return out;
}