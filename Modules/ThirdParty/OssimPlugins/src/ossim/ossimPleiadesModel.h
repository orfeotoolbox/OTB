/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
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

#ifndef ossimPleiadesModel_HEADER
#define ossimPleiadesModel_HEADER

#include <ossimPluginConstants.h>

#include <ossim/projection/ossimRpcModel.h>
#include "ossimPleiadesDimapSupportData.h"

#include <ossim/base/ossimFilename.h>



namespace ossimplugins
{
   class ossimPleiadesDimapSupportData;

   class OSSIM_PLUGINS_DLL ossimPleiadesModel : public ossimRpcModel
   {
   public:
      /*!
       * CONSTRUCTORS:
       */
      /** @brief default constructor */
      ossimPleiadesModel();

      /** @brief copy constructor */
      ossimPleiadesModel(const ossimPleiadesModel& rhs);

      /** @brief Destructor */
      virtual ~ossimPleiadesModel();

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

      void setSupportData(ossimPleiadesDimapSupportData* supportData)
      {
         theSupportData = supportData;
      }
      ossimPleiadesDimapSupportData* getSupportData()
      {
         return theSupportData.get();
      }
      const ossimPleiadesDimapSupportData* getSupportData()const
      {
         return theSupportData.get();
      }

   protected:

      bool parseMetaData(const ossimFilename& file);

      bool parseRpcData (const ossimFilename& file);

      // In the future
      //bool parseJP2File (const ossimFilename& file);

      void finishConstruction();

      ossimRefPtr<ossimPleiadesDimapSupportData> theSupportData;

      ossimFilename _productXmlFile;


      TYPE_DATA
         };
}
#endif /* #ifndef ossimPleiadesModel_HEADER */

