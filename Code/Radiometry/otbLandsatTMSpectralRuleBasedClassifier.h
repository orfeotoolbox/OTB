/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbLandsatTMSRBC_h
#define __otbLandsatTMSRBC_h

#include "otbLandsatTMIndices.h"

namespace otb
{

namespace Functor
{

namespace LandsatTM
{

/** \class SpectralRuleBasedClassifier
 *
 * Implementation of the SpectralRuleBasedClassifier for Landsat TM image
 *  land cover classification as described in table IV of Baraldi et
 *  al. 2006, "Automatic Spectral Rule-Based Preliminary Mapping of
 *  Calibrated Landsat TM and ETM+ Images", IEEE Trans. on Geoscience
 *  and Remote Sensing, vol 44, no 9.
 *
 *
 * \ingroup Functor
 * \ingroup Radiometry
 * \ingroup LandsatTMIndices
 */
template <class TInput, class TOutput>
class SpectralRuleBasedClassifier : public KernelSpectralRule<TInput, TOutput>
{
public:
  // Spectral categories
  enum SpectralCategory {
    NOCLASS, // if the pixel is not classified; just for error
             // checking; different from SU, which is a real rejection
             // class
    TKCL, // thick clouds
    TNCL, // thin clouds; possible confusions with cold light toned
          // (highly reflective) barren land and range land
    SN,   // snow
    ICSN, // ice or snow
    DPWASH, // deep clear water and shadow areas; possible confusions
            // with lava rocks
    SLWASH, // shallow clear water and shadow areas
    PBHNDVI, // pit bog with high NDVI
    PBMNDVI, // pit bog with medium NDVI
    PBLNDVI, // pit bog with low NDVI; possible confusions with
             // greenhouses
    SVHNIR, // strong vegetation with high NIR; includes broadleaved
            // decideous forests, vegetated cropland and pastures;
            // possible confusions with mixed forests
    SVLNIR, // strong vegetation with low NIR; includes evergreen
            // forests and mixed forests; possible confusions with
            // forest land in shadow areas
    AVHNIR, // average vegetation with high NIR; includes cropland and
            // pastures; possible confusions with evergreen forests
            // and mixed forests
    AVLNIR, // average vegetation with low NIR; includes evergreen
            // forests, especially coniferous; possible confusions
            // with forest land in shadow areas
    WVHNIR, // weak vegetation with high NIR; includes scarcely
            // vegetated areas
    WVLNIR, // weak vegetation with low NIR; includes forested wetland
    SSRHNIR, // strong shrub rangeland with high NIR; includes
             // herbaceous rangeland; possible confusions with shrub
             // and brush rangeland
    SSRLNIR, // strong shrub rangeland with low NIR; includes shrub
             // and brush rangeland; possible confusions with
             // herbaceous rangeland
    ASRHNIR, // average shrub rangeland with high NIR; includes
             // herbaceous rangeland; possible confusions with shrub
             // and brush rangeland
    ASRLNIR, // average shrub rangeland with low NIR; includes shrub
             // and brush rangeland; possible confusions with
             // herbaceous rangeland
    SHR,     // strong herbaceous rangeland; possible confusions with
             // vegetated cropland ans pastures
    AHR,     // average herbaceous rangeland; includes herbaceous
             // rangeland and barren land scarcely vegetated; possible
             // confusions with vegetated cropland and pastures
    DR,      // dark rangeland; includes mixed rangeland eventually in
             // shadow areas and non forested wetland; possible
             // confusions with mixed urban or built up
    BBBHTIRF, // bright barren land or built up with high TIR and flat
              // spectral response; includes urban or built up and
              // concrete roads
    BBBHTIRNF, // bright barren land or built up with high TIR and non flat
              // spectral response;
    BBBLTIRF, // bright barren land or built up with low TIR and flat
              // spectral response; includes urban or built up and
              // concrete roads
    BBBLTIRNF, // bright barren land or built up with low TIR and non flat
              // spectral response;
    SBBHTIRF, // strong barren land or built up with high TIR and flat
              // spectral response; includes urban or built up and
              // concrete roads
    SBBHTIRNF, // strong barren land or built up with high TIR with non flat
              // spectral response; includes ploughed fields, barren
              // land -- including bare exposed rocks -- and beaches
    SBBLTIRF, // strong barren land or built up with low TIR and flat
              // spectral response; includes urban or built up and
              // concrete roads
    SBBLTIRNF, // strong barren land or built up with low TIR with non flat
              // spectral response; includes ploughed fields, barren
              // land -- including bare exposed rocks -- and beaches
    ABBHTIRF, // average barren land or built up with high TIR and flat
              // spectral response; includes urban or built up and
              // concrete roads
    ABBHTIRNF, // average barren land or built up with high TIR with non flat
              // spectral response
    ABBLTIRF, // average barren land or built up with low TIR and flat
              // spectral response; includes urban or built up and
              // concrete roads
    ABBLTIRNF, // average barren land or built up with low TIR with non flat
              // spectral response
    DBBHTIRF, // dark barren land or built up with high TIR and flat
              // spectral response; includes urban or built up land,
              // in particular parking lots, concrete roads, asphalt
              // roads, grey-brown tile roofs, tan composite shingle
              // roofs; possible confusions with barren land on dark
              // mountain slopes including burned areas and bare
              // exposed rocks, especially dark-toned soil
    DBBHTIRNF, // dark barren land or built up with high TIR and non
               // flat spectral response; includes barren land, bare
               // exposed rocks, especially dark-toned soil
    DBBLTIRF, // dark barren land or built up with low TIR and flat
              // spectral response; includes urban or built up land,
              // in particular parking lots, concrete roads, asphalt
              // roads, grey-brown tile roofs, tan composite shingle
              // roofs; possible confusions with barren land on dark
              // mountain slopes including burned areas and bare
              // exposed rocks, especially dark-toned soil
    DBBLTIRNF, // dark barren land or built up with low TIR and non
               // flat spectral response; includes barren land, bare
               // exposed rocks, especially dark-toned soil
    WR,        // weak rangeland; includes herbaceous rangeland
    SHV,       // shadow area with vegetation; possible confusions
               // with non forested wetland
    SHB,       // shadow with barren land; includes also lava rock;
               // possible confusions with buil up areas, especially
               // asphalt roads
    SHCL,      // clouds in shadow areas
    SHSN,      // snow in shadow areas
    WE,        // non forested wetland; possible confusions with
               // shadow areas with vegetation
    TWA,       // turbid water; possible confusions with shadow areas
    SU        // shadow areas or unknown pixels
  };
  typedef typename TInput::ValueType PrecisionType;
  typedef bool OutputPixelType;
  
    /** Return the index name */
  virtual std::string GetName() const
  {
    return "LandsatTM SpectralRuleBasedClassifier";
  }
  
  SpectralRuleBasedClassifier() { }
  virtual ~SpectralRuleBasedClassifier() {}

  inline TOutput operator ()(const TInput& inputPixel)
  {
    TInput newPixel(this->PrepareValues( inputPixel ));

    SpectralCategory result = NOCLASS;

    return static_cast<TOutput>(result);
  }
};


} // namespace LandsatTM
} // namespace Functor
} // namespace otb

#endif
