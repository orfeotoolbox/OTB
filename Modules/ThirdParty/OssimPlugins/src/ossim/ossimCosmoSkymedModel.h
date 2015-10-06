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

#ifndef ossimCosmoSkymedModel_H
#define ossimCosmoSkymedModel_H


#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimSarModel.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>

#include <iostream>
#include <list>
//#include "itkMetaDataObject.h"

namespace ossimplugins
{
class ossimCosmoSkymedModel : public ossimSarModel
{
public:
  /**
   * @brief Constructor
   */
  ossimCosmoSkymedModel();

  /**
   * @brief Destructor
   */
  ~ossimCosmoSkymedModel();

   bool updateMetadata(const ossimKeywordlist& kwl);

   /*!
    * Returns pointer to a new instance, copy of this.
    * Not implemented yet!  Returns NULL...
    */
   virtual ossimObject* dup() const;

   /*!
    * Extends base-class implementation. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;

   /*!
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL) const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);

protected:

   TYPE_DATA

   //private:

};
}

#endif
