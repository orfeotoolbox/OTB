//**************************************************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Oscar Kramer
// 
// Description: Singleton class for logging all support data files opened during a session.
//
//**************************************************************************************************
// $Id$

#include <ossim/support_data/ossimSupportFilesList.h>
#include <ossim/base/ossimKeywordlist.h>

ossimSupportFilesList* ossimSupportFilesList::m_instance = 0;

//*************************************************************************************************
// Implements singleton pattern
//*************************************************************************************************
ossimSupportFilesList* ossimSupportFilesList::instance()
{
   if (!m_instance)
      m_instance = new ossimSupportFilesList;

   return m_instance;
}

//*************************************************************************************************
// Saves the list to a KWL
//*************************************************************************************************
void ossimSupportFilesList::save(ossimKeywordlist& kwl, const char* prefix) const
{
   ossimString baseName ("support_file");
   for (unsigned int i=0; i< (unsigned int) m_list.size(); i++)
   {
      ossimString key = baseName + ossimString::toString(i);
      kwl.add(prefix, key.chars(), m_list[i]);
   }
}
