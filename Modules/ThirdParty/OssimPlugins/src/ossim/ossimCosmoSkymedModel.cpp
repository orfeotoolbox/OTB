//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include "ossimCosmoSkymedModel.h"

namespace ossimplugins
{

RTTI_DEF1(ossimCosmoSkymedModel, "ossimCosmoSkymedModel", ossimSarModel);

ossimCosmoSkymedModel::ossimCosmoSkymedModel()
{
}

ossimCosmoSkymedModel::~ossimCosmoSkymedModel()
{
}


ossimObject*
ossimCosmoSkymedModel
::dup() const
{
 return new ossimCosmoSkymedModel(*this);
}

//*************************************************************************************************
// Print
//*************************************************************************************************
std::ostream&
ossimCosmoSkymedModel
::print(std::ostream& out) const
{
   // Capture stream flags since we are going to mess with them.
   std::ios_base::fmtflags f = out.flags();

   out << "\nDump of ossimSentinel1Model at address " << (hex) << this
       << (dec)
       << "\n------------------------------------------------"
       << "\n  theImageID            = " << theImageID
       << "\n  theImageSize          = " << theImageSize

       << "\n------------------------------------------------"
       << "\n  " << endl;

   // Set the flags back.
   out.flags(f);
   return ossimSarModel::print(out);

}

bool
ossimCosmoSkymedModel
::updateMetadata(const ossimKeywordlist& kwl)
{
   return true;
}

bool
ossimCosmoSkymedModel
::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   ossimSarModel::saveState(kwl, prefix);
   return true;
}

bool
ossimCosmoSkymedModel
::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   theSensorID = kwl.find("Metadata.Mission_ID");
   theImageID = kwl.find("Metadata.Programmed_Image_ID");

   ossimSarModel::loadState(kwl, prefix);
   return true;
}

}
