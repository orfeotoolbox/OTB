//----------------------------------------------------------------------------
// Copyright (C) 2005 David Burken, all rights reserved.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: David Burken
//
// Description:
//
// Contains class declaration of ossimDtedElevationImageSource.
//
// $Id: ossimDtedElevationImageSource.h,v 1.4 2005/02/11 15:07:31 dburken Exp $
//----------------------------------------------------------------------------
#ifndef ossimDtedElevationImageSource_HEADER
#define ossimDtedElevationImageSource_HEADER

#include <base/common/ossimConstants.h>
#include <base/common/ossimRefPtr.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimGrect.h>


// Forward class declarations.
class ossimImageData;
class ossimImageChain;
class ossimDtedTileSource;
class ossimConnectableObject;
class ossimMapProjection;
class ossimIrect;


/**
 * Class to return large area blocks of data as an ossimImageData object from
 * multiple dted cells.  Source dted cells must be of the same resolution and
 * in the standard dted directory stucture.
 *
 * Example directory structure:
 *
 *                      dted_level2_directory
 *                        /              \  
 *                       /                \
 *                    w099                w100
 *                   /    \              /    \
 *                  /      \            /      \
 *               n32.dt2  n33.dt2   n32.dt2  n33.dt2
 */
class OSSIMDLLEXPORT ossimDtedElevationImageSource
{
public:

   enum ResamplerType
   {
      NEAREST_NEIGHBOR = 0,
      BILINEAR         = 1
   };
   
   /**
    * Default constructor
    */
   ossimDtedElevationImageSource();

   /**
    * Destructor
    */
   virtual ~ossimDtedElevationImageSource();

   /**
    * @param rect Requested rectangle.
    *
    * @param lines Number of lines.  This will determine the
    * post spacing in the latitude direction.
    *
    * @param samples Number of samples.  This will determine the post spacing
    * in the longitude direction.
    */
   ossimRefPtr<ossimImageData> getBlock(const ossimGrect& rect,
                                        ossim_uint32 lines,
                                        ossim_uint32 samples) const;
   
   /**
    * @param directory Directory to search for dted cells.
    *
    * @note Directory should be in a standard dted format.
    *
    * @note All cells in the directory should be of same resolution.
    */
   void setDirectory(const ossimFilename& directory);

   /**
    * @param directory Will be initialized to the current dted directory.
    */
   void getDirectory(ossimFilename& directory) const;

    /**
    * @param ext Should be the extension of cells in the directory like
    * "dt1", "dt2".
    *
    * @note All cells in the directory should have same extension.
    */
   void setCellExtension(const ossimString& ext);

   /**
    * @param ext Will be initialized to the current cell extension.
    */
   void getCellExtension(ossimString& ext) const;

   /**
    * @param type Enumerated resampler type.
    */
   void setResamplerType(ossimDtedElevationImageSource::ResamplerType type);

   /**
    * @return Resampler type.
    */
   ResamplerType getResamplerType() const;
   

private:
   
   /**
    * Finds all cells in "theDirectory" for rectangle passed in.
    *
    * @param rect Rectangle of requested elevation block.
    *
    * @param vf Vector of ossimFilenames to initialize with cells that are
    * present and overlap rect.
    *
    * @note vf will be cleared upon entring this method, so if vf.empty() is
    * true than no cells were found.
    */
   void findCells(const ossimGrect& rect,
                  vector<ossimFilename>& vf) const;

   /**
    * Main resample method.
    *
    * @param rect Rectangle of requested elevation block.
    *
    */
   void resampleCell(const ossimGrect& rect,
                     const ossimFilename& dtedFile,
                     ossimImageData& id,
                     const ossimDpt& postSpacing) const;

   /**
    * Specialized resample method.  Uses ossimDtedHandler and its
    * getHeightAboveMSL method to fill rect.
    */
   void resampleCellBilinear(const ossimGrect& rect,
                             const ossimFilename& dtedFile,
                             ossimImageData& id,
                             const ossimDpt& postSpacing) const;

   /**
    * Specialized resample method.  Uses ossimDtedTileSource to fill a patch,
    * then resamples to post spacing using nearest neighbor.
    */
   void resampleCellNearestNeighbor(const ossimGrect& rect,
                                    const ossimFilename& dtedFile,
                                    ossimImageData& id,
                                    const ossimDpt& requestedPostSpacing)const;

   /**
    * @param rect Ground rectangle.
    *
    * @param lines Number of desired lines in latitude direction.
    *
    * @param samples Number of desired samples in the longitude direction.
    *
    * @param result
    * result.y = (rect.ul().latd - rect.ll().latd) / lines
    * result.x = (rect.lr().lond - rect.ll().lond) / samples
    */
   void getPostSpacing(const ossimGrect& rect,
                       ossim_uint32 lines,
                       ossim_uint32 samples,
                       ossimDpt& result) const;

   /**
    * Aligns clip rect to requested rect with respect to post spacing.
    */
   void snap(const ossimGrect& rect,
             const ossimDpt& postSpacing,
             ossimGrect& clipRect) const;

   /**
    * @return The index for gpt into id's buf.  If point does not intersect
    * the rectangle an "OSSIM_UINT_NAN" is returned.
    */
   ossim_uint32 computeIndex(const ossimImageData& id,
                             const ossimGrect& idRect,
                             const ossimGpt& gpt,
                             const ossimDpt& postSpacing) const;

   /**
    * @return Returns the rLevel that is equal to, or greater than the
    * requestedPostSpacing.
    */
   ossim_uint32 computeRLevel(ossim_uint32 numberOfRLevels,
                              const ossimDpt& requestedPostSpacing,
                              const ossimDpt& cellPostSpacing) const;

   ossimConnectableObject* createRawChain(const ossimFilename& file) const;

   ossimImageChain* createMosaic(
      const std::vector<ossimFilename>& inputs) const;
   
   ossimImageChain* createMosaic(
      const std::vector<ossimConnectableObject*>& inputs) const;

   ossimMapProjection* getView(ossimConnectableObject* container) const;

   void computeImageRect(const ossimMapProjection* view,
                         const ossimGrect& grect,
                         ossimIrect& irect) const;

   ossimFilename theDirectory;
   ossimString   theCellExtension;
   ResamplerType theResamplerType;
   
};

#endif /* End of #ifndef ossimDtedElevationImageSource_HEADER */

