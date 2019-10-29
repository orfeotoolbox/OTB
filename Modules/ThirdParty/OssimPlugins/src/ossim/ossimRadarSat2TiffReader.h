/*
 * Copyright (C) 2010-2016 by John Stastny
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

#ifndef ossimRadarSat2TiffReader_HEADER
#define ossimRadarSat2TiffReader_HEADER 1

#include <ossimPluginConstants.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/base/ossimFilename.h>

// Forward class declarations outside of namespace ossimplugins.
class ossimXmlDocument;

namespace ossimplugins
{
   class ossimRadarSat2TiffReader : public ossimTiffTileSource
   {
   public:
      
      /** default constructor */
      ossimRadarSat2TiffReader();
      
      /** virtual destructor */
      virtual ~ossimRadarSat2TiffReader();
      
      /** @return "ossim_radarsat2_tiff_reader" */
      virtual ossimString getShortName() const;
      
      /** @return "ossim RadarSAT-2 tiff reader" */
      virtual ossimString getLongName()  const;
      
      /**
       *  This open take the product xml file.
       *  
       *  @param file File to open.
       *
       *  @return true on success, false on error.
       */
      virtual bool open(const ossimFilename& file);
      
      /**
       * Method to save the state of an object to a keyword list.
       * Return true if ok or false on error.
       */
      virtual bool saveState(ossimKeywordlist& kwl,
                             const char* prefix=0)const;
      
      /**
       * Method to the load (recreate) the state of an object from a keyword
       * list.  Return true if ok or false on error.
       */
      virtual bool loadState(const ossimKeywordlist& kwl,
                             const char* prefix=0);
      
      /** Close method. */
      virtual void close();
      
      /**
       * Returns the image geometry object associated with this tile source or
       * NULL if non defined.  The geometry contains full-to-local image
       * transform as well as projection (image-to-world).
       */
      virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

      /**
       * @param Method to get geometry from the xml file or internal geotiff
       * tags.
       */
      virtual ossimRefPtr<ossimImageGeometry> getInternalImageGeometry() const;
      
   private:
      
      /**
       * @brief Method to check file to see if it's a RS2 product
       * xml file.
       * @param file The file to check.
       * @return true on success, false on error.
       */
      bool isRadarSat2ProductFile(const ossimFilename& file) const;
      
      /**
       * @brief Method to check file to see if it's a RS2 product
       * xml file.
       * @param xdoc XML document opened up to product.xml file.
       * @return true on success, false on error.
       */
      bool isRadarSat2ProductFile(const ossimXmlDocument* xdoc) const;
      
      
      ossimFilename theProductXmlFile;

      TYPE_DATA
   };

} // End: namespace ossimplugins

#endif /* #ifndef ossimRadarSat2TiffReader_HEADER */

