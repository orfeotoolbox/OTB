//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: DTED Info object.
// 
//----------------------------------------------------------------------------
// $Id$

#include <iostream>
#include <ossim/support_data/ossimDtedInfo.h>
#include <ossim/support_data/ossimDtedVol.h>
#include <ossim/support_data/ossimDtedHdr.h>
#include <ossim/support_data/ossimDtedUhl.h>
#include <ossim/support_data/ossimDtedDsi.h>
#include <ossim/support_data/ossimDtedAcc.h>
#include <ossim/base/ossimErrorContext.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimRegExp.h>


ossimDtedInfo::ossimDtedInfo()
   : theFile()
{
}

ossimDtedInfo::~ossimDtedInfo()
{
}

bool ossimDtedInfo::open(const ossimFilename& file)
{
   bool result = false;

   // Test for extension, like dt0, dt1...
   ossimString ext = file.ext();
   ossimRegExp regExp("^[d|D][t|T][0-9]");
   
   if ( regExp.find( ext.c_str() ) )
   {
      ossimDtedVol vol(file, 0);
      ossimDtedHdr hdr(file, vol.stopOffset());
      ossimDtedUhl uhl(file, hdr.stopOffset());
      ossimDtedDsi dsi(file, uhl.stopOffset());
      ossimDtedAcc acc(file, dsi.stopOffset());
      
      //---
      // Check for errors.  Must have uhl, dsi and acc records.  vol and hdr
      // are for magnetic tape only; hence, may or may not be there.
      //---
      if ( (uhl.getErrorStatus() == ossimErrorCodes::OSSIM_OK) &&
           (dsi.getErrorStatus() == ossimErrorCodes::OSSIM_OK) &&
           (acc.getErrorStatus() == ossimErrorCodes::OSSIM_OK) )
      {
         theFile = file;
         result = true;
      }
      else
      {
         theFile.clear();
      }
   }

   return result;
}

std::ostream& ossimDtedInfo::print(std::ostream& out) const
{
   if ( theFile.size() )
   {
      std::string prefix = "dted.";
      
      ossimDtedVol vol(theFile, 0);
      ossimDtedHdr hdr(theFile, vol.stopOffset());
      ossimDtedUhl uhl(theFile, hdr.stopOffset());
      ossimDtedDsi dsi(theFile, uhl.stopOffset());
      ossimDtedAcc acc(theFile, dsi.stopOffset());
      if( vol.getErrorStatus() == ossimErrorCodes::OSSIM_OK )
      {
         vol.print(out, prefix);
      }
      if( hdr.getErrorStatus() == ossimErrorCodes::OSSIM_OK )
      {
         hdr.print(out, prefix);
      }
      if( uhl.getErrorStatus() == ossimErrorCodes::OSSIM_OK )
      {
         uhl.print(out, prefix);
      }
      if( dsi.getErrorStatus() == ossimErrorCodes::OSSIM_OK )
      {
         dsi.print(out, prefix);
      }
      if( acc.getErrorStatus() == ossimErrorCodes::OSSIM_OK )
      {
         acc.print(out, prefix);
      }
   }
   return out;
}

ossimRefPtr<ossimProperty> ossimDtedInfo::getProperty(
   const ossimString& name)const
{
   ossimRefPtr<ossimProperty> result = 0;

   //---
   // Look through dted records.
   // Must have uhl, dsi and acc records.  vol and hdr
   // are for magnetic tape only; hence, may or may not be there.
   //---
   ossimDtedVol vol(theFile, 0);
   if( vol.getErrorStatus() == ossimErrorCodes::OSSIM_OK )
   {
      if (name == "dted_vol_record")
      {
         ossimContainerProperty* box = new ossimContainerProperty();
         box->setName(name);

         std::vector<ossimString> list;
         vol.getPropertyNames(list);

         std::vector< ossimRefPtr<ossimProperty> > propList;

         std::vector<ossimString>::const_iterator i = list.begin();
         while (i != list.end())
         {
            ossimRefPtr<ossimProperty> prop = vol.getProperty( (*i) );
            if (prop.valid())
            {
               propList.push_back(prop);
            }
            ++i;
         }
         box->addChildren(propList);
         result = box;
      }
   }
   if (result.valid() == false)
   {
      ossimDtedHdr hdr(theFile, vol.stopOffset());
      if( hdr.getErrorStatus() == ossimErrorCodes::OSSIM_OK )
      {
         if (name == "dted_hdr_record")
         {
            ossimContainerProperty* box = new ossimContainerProperty();
            box->setName(name);
            
            std::vector<ossimString> list;
            hdr.getPropertyNames(list);
            
            std::vector< ossimRefPtr<ossimProperty> > propList;
            
            std::vector<ossimString>::const_iterator i = list.begin();
            while (i != list.end())
            {
               ossimRefPtr<ossimProperty> prop = hdr.getProperty( (*i) );
               if (prop.valid())
               {
                  propList.push_back(prop);
               }
               ++i;
            }
            box->addChildren(propList);
            result = box;
         }
      }
      if (result.valid() == false)
      {
         ossimDtedUhl uhl(theFile, hdr.stopOffset());
         if( uhl.getErrorStatus() == ossimErrorCodes::OSSIM_OK )
         {
            if (name == "dted_uhl_record")
            {
               ossimContainerProperty* box = new ossimContainerProperty();
               box->setName(name);
               
               std::vector<ossimString> list;
               uhl.getPropertyNames(list);
               
               std::vector< ossimRefPtr<ossimProperty> > propList;
               
               std::vector<ossimString>::const_iterator i = list.begin();
               while (i != list.end())
               {
                  ossimRefPtr<ossimProperty> prop = uhl.getProperty( (*i) );
                  if (prop.valid())
                  {
                     propList.push_back(prop); 
                  }
                  ++i;
               }
               box->addChildren(propList);
               result = box;
            }
         }
         if (result.valid() == false)
         {
            ossimDtedDsi dsi(theFile, uhl.stopOffset());
            if( dsi.getErrorStatus() == ossimErrorCodes::OSSIM_OK )
            {
               if (name == "dted_dsi_record")
               {
                  ossimContainerProperty* box =
                     new ossimContainerProperty();
                  box->setName(name);
                  
                  std::vector<ossimString> list;
                  dsi.getPropertyNames(list);
                  
                  std::vector< ossimRefPtr<ossimProperty> > propList;
                  
                  std::vector<ossimString>::const_iterator i = list.begin();
                  while (i != list.end())
                  {
                     ossimRefPtr<ossimProperty> prop =
                        dsi.getProperty( (*i) );
                     if (prop.valid())
                     {
                        propList.push_back(prop);
                     }
                     ++i;
                  }
                  box->addChildren(propList);
                  result = box;
               }
            }
            if (result.valid() == false)
            {
               ossimDtedAcc acc(theFile, dsi.stopOffset());
               if( acc.getErrorStatus() == ossimErrorCodes::OSSIM_OK )
               {
                  if (name == "dted_acc_record")
                  {
                     ossimContainerProperty* box =
                        new ossimContainerProperty();
                     box->setName(name);
                     
                     std::vector<ossimString> list;
                     acc.getPropertyNames(list);
                     
                     std::vector< ossimRefPtr<ossimProperty> > propList;
                     
                     ossimRefPtr<ossimProperty> prop = 0;
                     std::vector<ossimString>::const_iterator i =
                        list.begin();
                     while (i != list.end())
                     {
                        ossimRefPtr<ossimProperty> prop =
                           acc.getProperty( (*i) );
                        if (prop.valid())
                        {
                           propList.push_back(prop);
                        }
                        ++i;
                     }
                     box->addChildren(propList);
                     result = box;
                  }
               }
            }
         }
      }
   }

   return result;
}

void ossimDtedInfo::getPropertyNames(
   std::vector<ossimString>& propertyNames)const
{
   propertyNames.push_back(ossimString("dted_vol_record"));
   propertyNames.push_back(ossimString("dted_hdr_record"));
   propertyNames.push_back(ossimString("dted_uhl_record"));
   propertyNames.push_back(ossimString("dted_dsi_record"));
   propertyNames.push_back(ossimString("dted_acc_record"));
}


