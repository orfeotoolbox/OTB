//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Oscar Kramer
// 
//********************************************************************
// $Id$
#ifndef ossimSupportFilesList_HEADER
#define ossimSupportFilesList_HEADER

#include <vector>
#include <ossim/base/ossimFilename.h>

class ossimKeywordlist;

//*************************************************************************************************
//! Singleton class for logging all support data files opened during a session.
//*************************************************************************************************
class OSSIMDLLEXPORT ossimSupportFilesList
{
public:
   static ossimSupportFilesList* instance();

   //! Add support data filename to the list:
   void add(const ossimFilename& f) { m_list.push_back(f); }

   //! Output list to the kwl.
   void save(ossimKeywordlist& kwl, const char* prefix) const;
   
   //! Clears the list to ready for new accumulation:
   void clear() { m_list.clear(); }

private:
   ossimSupportFilesList()  { }
   ~ossimSupportFilesList() { m_instance=0; }

   std::vector<ossimFilename>       m_list;
   static ossimSupportFilesList*    m_instance;
};

#endif
