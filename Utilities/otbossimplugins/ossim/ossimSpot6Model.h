//*******************************************************************
// Copyright (C) 2014  Centre National Etudes Spatiales
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// Author : Christophe Palmann (christophe.palmann@c-s.fr)
//
// Description:
//
// Contains declaration of class ossimSpot6Model.
//
//*****************************************************************************
// FIXME $Id: ossimSpot5Model.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimSpot6Model_HEADER
#define ossimSpot6Model_HEADER

#include <ossimPluginConstants.h>

#include <ossim/projection/ossimRpcModel.h>
#include "ossimSpot6DimapSupportData.h"

#include <ossim/base/ossimFilename.h>

#include <iostream>


namespace ossimplugins
{
   class ossimSpot6DimapSupportData;

   class OSSIM_PLUGINS_DLL ossimSpot6Model : public ossimRpcModel
   {
   public:
      /*!
       * CONSTRUCTORS:
       */
      /** @brief default constructor */
      ossimSpot6Model();

      /** @brief copy constructor */
      ossimSpot6Model(const ossimSpot6Model& rhs);

      /** @brief Destructor */
      virtual ~ossimSpot6Model();

      bool open(const ossimFilename& file);

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

      void setSupportData(ossimSpot6DimapSupportData* supportData)
      {
         theSupportData = supportData;
      }
      ossimSpot6DimapSupportData* getSupportData()
      {
         return theSupportData.get();
      }
      const ossimSpot6DimapSupportData* getSupportData()const
      {
         return theSupportData.get();
      }

   protected:

      bool parseMetaData(const ossimFilename& file);

      bool parseRpcData (const ossimFilename& file);

      // In the future
      //bool parseJP2File (const ossimFilename& file);

      void finishConstruction();

      ossimRefPtr<ossimSpot6DimapSupportData> theSupportData;

      ossimFilename _productXmlFile;


      TYPE_DATA
         };
}
#endif /* #ifndef ossimSpot6Model_HEADER */

