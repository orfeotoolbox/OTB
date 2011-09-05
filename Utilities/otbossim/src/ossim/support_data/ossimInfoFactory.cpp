//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Factory for info objects.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/support_data/ossimInfoFactory.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/support_data/ossimCcfInfo.h>
#include <ossim/support_data/ossimDemInfo.h>
#include <ossim/support_data/ossimDoqq.h>
#include <ossim/support_data/ossimDtedInfo.h>
#include <ossim/support_data/ossimJ2kInfo.h>
#include <ossim/support_data/ossimLasInfo.h>
#include <ossim/support_data/ossimNitfInfo.h>
#include <ossim/support_data/ossimTiffInfo.h>

#include <ossim/base/ossimFilename.h>

ossimInfoFactory* ossimInfoFactory::theInstance = 0;

ossimInfoFactory::~ossimInfoFactory()
{}

ossimInfoFactory* ossimInfoFactory::instance()
{
   if ( !theInstance )
   {
      theInstance = new ossimInfoFactory;
   }
   return theInstance;
}

ossimInfoBase* ossimInfoFactory::create(const ossimFilename& file) const
{
   ossimRefPtr<ossimInfoBase> result = 0;

   result = new ossimJ2kInfo();
   if ( result->open(file) )
   {
      return result.release();
   }

   result = new ossimNitfInfo();
   if ( result->open(file) )
   {
      return result.release();
   }

   result = new ossimTiffInfo();
   if ( result->open(file) )
   {
      return result.release();
   }

   result = new ossimDemInfo();
   if ( result->open(file) )
   {
      return result.release();
   }

   result = new ossimDtedInfo();
   if ( result->open(file) )
   {
      return result.release();
   }

   result = new ossimDtedInfo();
   if ( result->open(file) )
   {
      return result.release();
   }

   result = new ossimLasInfo();
   if ( result->open(file) )
   {
      return result.release();
   }

   result = new ossimDoqq();
   if ( result->open(file) )
   {
      return result.release();
   }   
   
   result = new ossimCcfInfo();
   if ( result->open(file) )
   {
      return result.release();
   }
   
   return 0;
}

ossimInfoFactory::ossimInfoFactory()
{}

ossimInfoFactory::ossimInfoFactory(const ossimInfoFactory& /* obj */ )
{}

const ossimInfoFactory& ossimInfoFactory::operator=(
   const ossimInfoFactory& /* rhs */)
{
   return *this;
}
