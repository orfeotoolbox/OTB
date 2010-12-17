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
    // We normalize the pixel just once, so that the indices do not
    // need to do it
    TInput newPixel(this->PrepareValues( inputPixel ));


    // Get the linguistic variables
    typedef LinguisticVariables<TInput> LVType;
    LVType lvf = LVType();
    lvf.SetSAT( this->m_SAT );
    typename LVType::OutputPixelType lv = lvf( newPixel );

    
    SpectralCategory result = NOCLASS;

    typedef ThickCloudsSpectralRule<TInput, bool> TKCLSRType;
    TKCLSRType tkclsrf = TKCLSRType();
    tkclsrf.SetTV1( this->m_TV1 );
    tkclsrf.SetTV2( this->m_TV2 );
    tkclsrf.SetSAT( this->m_SAT );

    bool tkclsr = tkclsrf( newPixel );

    typedef ThinCloudsSpectralRule<TInput, bool> TNCLSRType;
    TNCLSRType tnclsrf = TNCLSRType();
    tnclsrf.SetTV1( this->m_TV1 );
    tnclsrf.SetTV2( this->m_TV2 );
    tnclsrf.SetSAT( this->m_SAT );

    bool tnclsr = tnclsrf( newPixel );

    bool lBright     = (lv[ LVType::bright ] == LVType::Low);
    bool lVis        = (lv[ LVType::vis ] == LVType::Low);
    bool lNIR        = (lv[ LVType::nir ] == LVType::Low);
    bool hNDSIVis    = (lv[ LVType::ndsivis ] == LVType::High);
    bool lMIR1       = (lv[ LVType::mir1 ] == LVType::Low);
    bool lMIR2       = (lv[ LVType::mir2 ] == LVType::Low);
    bool hTIR        = (lv[ LVType::tir ] == LVType::High);
    bool hMIRTIR     = (lv[ LVType::mirtir ] == LVType::High);
    bool mMIRTIR     = (lv[ LVType::mirtir ] == LVType::Medium);
    bool lMIRTIR     = (lv[ LVType::mirtir ] == LVType::Low);

    
    // cloud spectral category
    bool clsc = (tkclsr or tnclsr) and !(lBright or
                                         lVis or lNIR or
                                         hNDSIVis or
                                         lMIR1 or
                                         lMIR2 or hTIR
                                         or hMIRTIR);
    
    // thick cloud spectral category
    if(clsc and lMIRTIR)
      return static_cast<TOutput>(TKCL);

    // thin cloud spectral category
    if(clsc and mMIRTIR)
      return static_cast<TOutput>(TNCL);


    typedef SnowOrIceSpectralRule<TInput, bool> SNICSRType;
    SNICSRType snicsrf = SNICSRType();
    snicsrf.SetTV1( this->m_TV1 );
    snicsrf.SetTV2( this->m_TV2 );
    snicsrf.SetSAT( this->m_SAT );

    bool snicsr = snicsrf( newPixel );

    bool lNDBSI     = (lv[ LVType::ndbsi ] == LVType::Low);
    bool lNDSIVis   = (lv[ LVType::ndsivis ] == LVType::Low);
    bool hMIR1      = (lv[ LVType::mir1 ] == LVType::High);
    bool hMIR2      = (lv[ LVType::mir2 ] == LVType::High);
    
    // snow or ice spectral category
    bool snicsc = (snicsr and lNDBSI and !(lBright or
                     lVis or lNDSIVis or lNIR or hMIR1 or hMIR2 or hTIR));

    // snow spectral category
    if(snicsc and hNDSIVis)
      return static_cast<TOutput>(SN);

    bool mNDSIVis     = (lv[ LVType::ndsivis ] == LVType::Medium);
    // ice or snow spectral category
    if(snicsc and mNDSIVis)
      return static_cast<TOutput>(ICSN);


    typedef WaterOrShadowSpectralRule<TInput, bool> WASHSRType;
    WASHSRType washsrf = WASHSRType();
    washsrf.SetTV1( this->m_TV1 );
    washsrf.SetTV2( this->m_TV2 );
    washsrf.SetSAT( this->m_SAT );

    bool washsr = washsrf( newPixel );
    
    
    bool lNDVI = (lv[ LVType::ndvi ] == LVType::Low);
    bool lTIR  = (lv[ LVType::tir ] == LVType::Low);
    // water or shadow spectral category
    bool washsc = washsr and lBright and lVis and lNDVI and lNIR and lMIR1 and lMIR2 and !(lTIR);

    return static_cast<TOutput>(washsr * 10);
    
    // deep water or shadow spectral category
    if( washsc and hNDSIVis)
      return static_cast<TOutput>(DPWASH);

    // shallow water or shadow spectral category
    if( washsc and !(hNDSIVis))
      return static_cast<TOutput>(SLWASH);


    typedef PitbogOrGreenhouseSpectralRule<TInput, bool> PBGHSRType;
    PBGHSRType pbghsrf = PBGHSRType();
    pbghsrf.SetTV1( this->m_TV1 );
    pbghsrf.SetTV2( this->m_TV2 );
    pbghsrf.SetSAT( this->m_SAT );

    bool pbghsr = pbghsrf( newPixel );
    
    // Pit bog spectral category
    bool pbsc = pbghsr and lMIR1 and lMIR2 and lNDBSI and !(lNIR);

    bool mNDVI = (lv[ LVType::ndvi ] == LVType::Medium);
    bool hNDVI = (lv[ LVType::ndvi ] == LVType::High);
    // Pit bog categories for each ndvi
    if( pbsc and hNDVI)
      return static_cast<TOutput>(PBHNDVI);
    if( pbsc and mNDVI)
      return static_cast<TOutput>(PBMNDVI);
    if( pbsc and lNDVI)
      return static_cast<TOutput>(PBLNDVI);
    
    
    
    return static_cast<TOutput>(result);
    
  }
};


} // namespace LandsatTM
} // namespace Functor
} // namespace otb

#endif
