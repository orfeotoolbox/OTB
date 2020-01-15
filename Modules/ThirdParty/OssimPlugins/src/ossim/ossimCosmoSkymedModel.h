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


#ifndef ossimCosmoSkymedModel_HEADER
#define ossimCosmoSkymedModel_HEADER

#include <iosfwd>
#include <string>
#include <boost/config.hpp>

#include "ossimPluginConstants.h" // OSSIM_PLUGINS_DLL

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimRefPtr.h>

#include "ossim/ossimSarSensorModel.h"

#include <ossim/support_data/ossimSupportFilesList.h>

//#include "ossimGeometricSarSensorModel.h"

class OSSIMDLLEXPORT ossimXmlNode;
class OSSIMDLLEXPORT ossimString;

namespace ossimplugins
{

   class OSSIM_PLUGINS_DLL ossimCosmoSkymedModel : public ossimSarSensorModel
   {
   public:

      /**
       * CONSTRUCTORS.
       * @{
       */
      /** @brief Default constructor */
      ossimCosmoSkymedModel();

#if ! (defined(BOOST_NO_DEFAULTED_FUNCTIONS) || defined(BOOST_NO_CXX1_DEFAULTED_FUNCTIONS))
      /** @brief Copy constructor */
      ossimCosmoSkymedModel(ossimCosmoSkymedModel const& rhs) = default;

      /** @brief Move constructor */
      ossimCosmoSkymedModel(ossimCosmoSkymedModel && rhs) = default;

      /** @brief Destructor */
      virtual ~ossimCosmoSkymedModel() = default;
#endif
      //@}

      bool open(const ossimFilename& file);

      /*!
       * Returns pointer to a new instance, copy of this.
       */
      ossimObject* dup() const override;

      /*!
       * Extends base-class implementation. Dumps contents of object to ostream.
       */
      std::ostream& print(std::ostream& out) const override;

      /*!
       * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
       * KWL files. Returns true if successful.
       */
      virtual bool saveState(ossimKeywordlist& kwl,
            const char* prefix=NULL) const override;

      virtual bool loadState(ossimKeywordlist const& kwl,
            const char* prefix=NULL) override;

      bool initImageSize(ossimIpt& imageSize) const;

      virtual void imagingRay(ossimDpt const& image_point, ossimEcefRay& image_ray) const override;

      bool isSLC() const { return  theSLC; }

      void clearFields();
     
   protected:
#if defined(__clang__)
#pragma clang diagnostic push
#if defined(__apple_build_version__)
/* Apple's 3.6.0 based clang doesn't support -Winconsistent-missing-override */
#if __apple_build_version__ >= 7000053
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif
#elif __clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 7)
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#endif
      TYPE_DATA
#pragma clang diagnostic pop
#else
      TYPE_DATA
#endif

   private:
      bool readSBI(std::string fileSBI);

      ossimKeywordlist   theProductKwl;
      bool               theSLC;
      bool               theTOPSAR;
   }; //end class ossimCosmoSkymedModel

}  //end namespace ossimplugins


#endif /* #ifndef ossimCosmoSkymedModel_HEADER */
