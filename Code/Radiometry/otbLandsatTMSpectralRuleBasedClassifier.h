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
 * \brief Implementation of the SpectralRuleBasedClassifier for
 *  Landsat TM image land cover classification as described in table
 *  IV of Baraldi et al. 2006, "Automatic Spectral Rule-Based
 *  Preliminary Mapping of Calibrated Landsat TM and ETM+ Images",
 *  IEEE Trans. on Geoscience and Remote Sensing, vol 44, no 9. This
 *  classifier assumes that the input image is calibrated in
 *  reflectances and in temperature. The reflectances can be given in
 *  the 0-1 range (Normalized) or in 0-1000 (Thousands). Temperatures
 *  can be given in Kelvin, in Kelvin*100 or in Celsius. Appropriate
 *  accessors are available for setting these units.
 *
 *  The OTB development team acknowledges the contribution of Andrea
 *  Baraldi, who provided the correct set of threshold values for the
 *  published rule set implementation to be reproduced. Andrea Baraldi
 *  is the inventor of the Satellite Image Automatic Mapper TradeMark (SIAM)
 *  software product conceived, developed and supported solely by
 *  Baraldi Consultancy in Remote Sensing of Andrea Baraldi.
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
    NOCLASS, // 0 -if the pixel is not classified; just for error
             // checking; different from SU, which is a real rejection
             // class
    TKCL, // 1 -thick clouds
    TNCL, // 2 -thin clouds; possible confusions with cold light toned
          // (highly reflective) barren land and range land
    SN,   // 3 -snow
    ICSN, // 4 -ice or snow
    DPWASH, // 5 -deep clear water and shadow areas; possible confusions
            // with lava rocks
    SLWASH, // 6 -shallow clear water and shadow areas
    PBHNDVI, // 7 -pit bog with high NDVI
    PBMNDVI, // 8 -pit bog with medium NDVI
    PBLNDVI, // 9 -pit bog with low NDVI; possible confusions with
             // greenhouses
    SVHNIR, // 10 - strong vegetation with high NIR; includes broadleaved
            // decideous forests, vegetated cropland and pastures;
            // possible confusions with mixed forests
    SVLNIR, // 11 -strong vegetation with low NIR; includes evergreen
            // forests and mixed forests; possible confusions with
            // forest land in shadow areas
    AVHNIR, // 12 -average vegetation with high NIR; includes cropland and
            // pastures; possible confusions with evergreen forests
            // and mixed forests
    AVLNIR, // 13 -average vegetation with low NIR; includes evergreen
            // forests, especially coniferous; possible confusions
            // with forest land in shadow areas
    WVHNIR, // 14 -weak vegetation with high NIR; includes scarcely
            // vegetated areas
    WVLNIR, // 15 -weak vegetation with low NIR; includes forested wetland
    SSRHNIR, // 16 -strong shrub rangeland with high NIR; includes
             // herbaceous rangeland; possible confusions with shrub
             // and brush rangeland
    SSRLNIR, // 17 -strong shrub rangeland with low NIR; includes shrub
             // and brush rangeland; possible confusions with
             // herbaceous rangeland
    ASRHNIR, // 18 -average shrub rangeland with high NIR; includes
             // herbaceous rangeland; possible confusions with shrub
             // and brush rangeland
    ASRLNIR, // 19 -average shrub rangeland with low NIR; includes shrub
             // and brush rangeland; possible confusions with
             // herbaceous rangeland
    SHR,     // 20 - strong herbaceous rangeland; possible confusions with
             // vegetated cropland ans pastures
    AHR,     // 21 -average herbaceous rangeland; includes herbaceous
             // rangeland and barren land scarcely vegetated; possible
             // confusions with vegetated cropland and pastures
    DR,      // 22 -dark rangeland; includes mixed rangeland eventually in
             // shadow areas and non forested wetland; possible
             // confusions with mixed urban or built up
    BBBHTIRF, // 23 -bright barren land or built up with high TIR and flat
              // spectral response; includes urban or built up and
              // concrete roads
    BBBHTIRNF, // 24 -bright barren land or built up with high TIR and non flat
              // spectral response;
    BBBLTIRF, // 25 -bright barren land or built up with low TIR and flat
              // spectral response; includes urban or built up and
              // concrete roads
    BBBLTIRNF, // 26 -bright barren land or built up with low TIR and non flat
              // spectral response;
    SBBHTIRF, // 27 -strong barren land or built up with high TIR and flat
              // spectral response; includes urban or built up and
              // concrete roads
    SBBHTIRNF, // 28 -strong barren land or built up with high TIR with non flat
              // spectral response; includes ploughed fields, barren
              // land -- including bare exposed rocks -- and beaches
    SBBLTIRF, // 29 -strong barren land or built up with low TIR and flat
              // spectral response; includes urban or built up and
              // concrete roads
    SBBLTIRNF, // 30 -strong barren land or built up with low TIR with non flat
              // spectral response; includes ploughed fields, barren
              // land -- including bare exposed rocks -- and beaches
    ABBHTIRF, // 31 -average barren land or built up with high TIR and flat
              // spectral response; includes urban or built up and
              // concrete roads
    ABBHTIRNF, // 32 -average barren land or built up with high TIR with non flat
              // spectral response
    ABBLTIRF, // 33 -average barren land or built up with low TIR and flat
              // spectral response; includes urban or built up and
              // concrete roads
    ABBLTIRNF, // 34 -average barren land or built up with low TIR with non flat
              // spectral response
    DBBHTIRF, // 35 -dark barren land or built up with high TIR and flat
              // spectral response; includes urban or built up land,
              // in particular parking lots, concrete roads, asphalt
              // roads, grey-brown tile roofs, tan composite shingle
              // roofs; possible confusions with barren land on dark
              // mountain slopes including burned areas and bare
              // exposed rocks, especially dark-toned soil
    DBBHTIRNF, // 36 -dark barren land or built up with high TIR and non
               // flat spectral response; includes barren land, bare
               // exposed rocks, especially dark-toned soil
    DBBLTIRF, // 37 -dark barren land or built up with low TIR and flat
              // spectral response; includes urban or built up land,
              // in particular parking lots, concrete roads, asphalt
              // roads, grey-brown tile roofs, tan composite shingle
              // roofs; possible confusions with barren land on dark
              // mountain slopes including burned areas and bare
              // exposed rocks, especially dark-toned soil
    DBBLTIRNF, // 38 -dark barren land or built up with low TIR and non
               // flat spectral response; includes barren land, bare
               // exposed rocks, especially dark-toned soil
    WR,        // 39 -weak rangeland; includes herbaceous rangeland
    SHV,       // 40 -shadow area with vegetation; possible confusions
               // with non forested wetland
    SHB,       // 41 -shadow with barren land; includes also lava rock;
               // possible confusions with buil up areas, especially
               // asphalt roads
    SHCL,      // 42 -clouds in shadow areas
    TWASHSN,   // 43 -snow in shadow areas
    WE,        // 44 -non forested wetland; possible confusions with
               // shadow areas with vegetation
    TWA,       // 45 -turbid water; possible confusions with shadow areas
    SU        // 46 -shadow areas or unknown pixels
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
    bool clsc = (tkclsr || tnclsr) && !(lBright ||
                                         lVis || lNIR ||
                                         hNDSIVis ||
                                         lMIR1 ||
                                         lMIR2 || hTIR
                                         || hMIRTIR);

    // thick cloud spectral category
    if(clsc && lMIRTIR)
      return static_cast<TOutput>(TKCL);

    // thin cloud spectral category
    if(clsc && mMIRTIR)
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
    bool snicsc = (snicsr && lNDBSI && !(lBright ||
                     lVis || lNDSIVis || lNIR || hMIR1 || hMIR2 || hTIR ));

    // snow spectral category
    if(snicsc && hNDSIVis)
      return static_cast<TOutput>(SN);

    bool mNDSIVis     = (lv[ LVType::ndsivis ] == LVType::Medium);
    // ice or snow spectral category
    if(snicsc && mNDSIVis)
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
    bool washsc = washsr && lBright && lVis && lNDVI && lNIR && lMIR1 && lMIR2 && !(lTIR);


    // deep water or shadow spectral category
    if( washsc && hNDSIVis)
      return static_cast<TOutput>(DPWASH);

    // shallow water or shadow spectral category
    if( washsc && !(hNDSIVis))
      return static_cast<TOutput>(SLWASH);


    typedef PitbogOrGreenhouseSpectralRule<TInput, bool> PBGHSRType;
    PBGHSRType pbghsrf = PBGHSRType();
    pbghsrf.SetTV1( this->m_TV1 );
    pbghsrf.SetTV2( this->m_TV2 );
    pbghsrf.SetSAT( this->m_SAT );

    bool pbghsr = pbghsrf( newPixel );

    // Pit bog spectral category
    bool pbsc = pbghsr && lMIR1 && lMIR2 && lNDBSI && !(lNIR);

    bool mNDVI = (lv[ LVType::ndvi ] == LVType::Medium);
    bool hNDVI = (lv[ LVType::ndvi ] == LVType::High);
    // Pit bog categories for each ndvi
    if( pbsc && hNDVI)
      return static_cast<TOutput>(PBHNDVI);
    if( pbsc && mNDVI)
      return static_cast<TOutput>(PBMNDVI);
    if( pbsc && lNDVI)
      return static_cast<TOutput>(PBLNDVI);


    typedef VegetationSpectralRule<TInput, bool> VSRType;
    VSRType vsrf = VSRType();
    vsrf.SetTV1( this->m_TV1 );
    vsrf.SetTV2( this->m_TV2 );
    vsrf.SetSAT( this->m_SAT );

    bool vsr = vsrf( newPixel );

    bool hNDBSI = (lv[ LVType::ndbsi ] == LVType::High);
    bool hNIR   = (lv[ LVType::nir ] == LVType::High);

    // strong vegetation spectral category
    bool svsc = vsr && hNDVI && !(hMIR1 || hMIR2 || hNDBSI);

    if( svsc && hNIR)
      return static_cast<TOutput>(SVHNIR);
    if( svsc && !(hNIR))
      return static_cast<TOutput>(SVLNIR);

    typedef ShadowWithVegetationSpectralRule<TInput, bool> SHVSRType;
    SHVSRType shvsrf = SHVSRType();
    shvsrf.SetTV1( this->m_TV1 );
    shvsrf.SetTV2( this->m_TV2 );
    shvsrf.SetSAT( this->m_SAT );

    bool shvsr = shvsrf( newPixel );


    typedef DominantBlueSpectralRule<TInput, bool> DBSRType;
    DBSRType dbsrf = DBSRType();
    dbsrf.SetTV1( this->m_TV1 );
    dbsrf.SetTV2( this->m_TV2 );
    dbsrf.SetSAT( this->m_SAT );

    bool dbsr = dbsrf( newPixel );

    // average vegetation spectral category
    bool avsc = (vsr || shvsr) && mNDVI && !(hMIR1 || hMIR2 || hNDBSI || dbsr);

    if( avsc && hNIR)
      return static_cast<TOutput>(AVHNIR);
    if( avsc && !(hNIR))
      return static_cast<TOutput>(AVLNIR);

    typedef RangelandSpectralRule<TInput, bool> RSRType;
    RSRType rsrf = RSRType();
    rsrf.SetTV1( this->m_TV1 );
    rsrf.SetTV2( this->m_TV2 );
    rsrf.SetSAT( this->m_SAT );

    bool rsr = rsrf( newPixel );

    // weak vegetation spectral category
    bool wvsc = (vsr || rsr || shvsr) && lNDVI && lNDBSI && lMIR1 && lMIR2 && !(dbsr);

    if( wvsc && hNIR)
      return static_cast<TOutput>(WVHNIR);
    if( wvsc && !(hNIR))
      return static_cast<TOutput>(WVLNIR);

    bool mNDBSI = (lv[ LVType::ndbsi ] == LVType::Medium);
    // strong shrub rangeland spectral category
    bool ssrsc = rsr && hNDVI && mNDBSI;
    if( ssrsc && hNIR)
      return static_cast<TOutput>(SSRHNIR);
    if( ssrsc && !(hNIR))
      return static_cast<TOutput>(SSRLNIR);

    typedef WetlandSpectralRule<TInput, bool> WESRType;
    WESRType wesrf = WESRType();
    wesrf.SetTV1( this->m_TV1 );
    wesrf.SetTV2( this->m_TV2 );
    wesrf.SetSAT( this->m_SAT );

    bool wesr = wesrf( newPixel );

    // average shrub rangeland spectral category
    bool asrsc = rsr && mNDVI && mNDBSI && !(shvsr || wesr);
    if( asrsc && hNIR)
      return static_cast<TOutput>(ASRHNIR);
    if( asrsc && !(hNIR))
      return static_cast<TOutput>(ASRLNIR);


    // strong herbaceous rangeland spectral category
    bool shrsc = rsr && hNDVI && hNDBSI;
    if( shrsc )
      return static_cast<TOutput>(SHR);

    typedef BarrenLandOrBuiltUpOrCloudsSpectralRule<TInput, bool> BBCSRType;
    BBCSRType bbcsrf = BBCSRType();
    bbcsrf.SetTV1( this->m_TV1 );
    bbcsrf.SetTV2( this->m_TV2 );
    bbcsrf.SetSAT( this->m_SAT );

    bool bbcsr = bbcsrf( newPixel );
    // average herbaceous rangeland spectral category
    bool ahrsc = (rsr || bbcsr) && mNDVI && hNDBSI;
    if( ahrsc )
      return static_cast<TOutput>(AHR);

    // dark rangeland spectral category
    bool drsc = (vsr || rsr) && lNDVI && lMIR2 && !(hNIR || hMIR1 || lNDBSI);
    if( drsc )
      return static_cast<TOutput>(DR);

    // bright barren land or built up spectral category
    bool bbbsc = bbcsr && hNIR && lNDVI && hNDBSI && !(lMIR1 || lMIR2);

    bool lNDBBBI  = (lv[ LVType::ndbbbi ] == LVType::Low);

    bool bbbhtirsc = bbbsc && hTIR;

    if( bbbhtirsc && !(lNDBBBI) )
      return static_cast<TOutput>(BBBHTIRF);
    if( bbbhtirsc && lNDBBBI )
      return static_cast<TOutput>(BBBHTIRNF);

    bool bbbltirsc = bbbsc && !(hTIR);
    if( bbbltirsc && !(lNDBBBI) )
      return static_cast<TOutput>(BBBLTIRF);
    if( bbbltirsc && lNDBBBI )
      return static_cast<TOutput>(BBBLTIRNF);

    typedef FlatResponseBarrenLandOrBuiltUpSpectralRule<TInput, bool> FBBSRType;
    FBBSRType fbbsrf = FBBSRType();
    fbbsrf.SetTV1( this->m_TV1 );
    fbbsrf.SetTV2( this->m_TV2 );
    fbbsrf.SetSAT( this->m_SAT );

    bool fbbsr = fbbsrf( newPixel );
    // strong barren land or built up spectral category
    bool sbbsc = (bbcsr || fbbsr) && lNDVI && hNDBSI && !( hNIR || lMIR1);

    bool sbbhtirsc = sbbsc && hTIR;

    if( sbbhtirsc && (dbsr || fbbsr) )
      return static_cast<TOutput>(SBBHTIRF);
    if( sbbhtirsc && !(dbsr || fbbsr) )
      return static_cast<TOutput>(SBBHTIRNF);

    bool sbbltirsc = sbbsc && !(hTIR);
    if( sbbltirsc && (dbsr || fbbsr) )
      return static_cast<TOutput>(SBBLTIRF);
    if( sbbltirsc && !((dbsr || fbbsr)) )
      return static_cast<TOutput>(SBBLTIRNF);

    // average barren land or built up spectral category
    bool abbsc = (bbcsr || fbbsr) && lNDVI && mNDBSI && !(lMIR1);

    bool abbhtirsc = abbsc && hTIR;

    if( abbhtirsc && !(lNDBBBI) )
      return static_cast<TOutput>(ABBHTIRF);
    if( abbhtirsc && lNDBBBI )
      return static_cast<TOutput>(ABBHTIRNF);

    bool abbltirsc = abbsc && !(hTIR);
    if( abbltirsc && !(lNDBBBI) )
      return static_cast<TOutput>(ABBLTIRF);
    if( abbltirsc && lNDBBBI )
      return static_cast<TOutput>(ABBLTIRNF);

    // dark barren land or built
    bool dbbsc = (bbcsr || fbbsr) && lNDVI && lMIR1 && !( hNIR || hMIR2 || lNDBSI);

    bool dbbhtirsc = dbbsc && hTIR;

    if( dbbhtirsc && (dbsr || fbbsr) )
      return static_cast<TOutput>(DBBHTIRF);
    if( dbbhtirsc && !(dbsr || fbbsr) )
      return static_cast<TOutput>(DBBHTIRNF);

    bool dbbltirsc = dbbsc && !(hTIR);
    if( dbbltirsc && (dbsr || fbbsr) )
      return static_cast<TOutput>(DBBLTIRF);
    if( dbbltirsc && !((dbsr || fbbsr)) )
      return static_cast<TOutput>(DBBLTIRNF);

    // weak rangeland spectral category
    bool wrsc = rsr && lNDVI && !(lNDBSI);
    if( wrsc )
      return static_cast<TOutput>(WR);

    // shadow area with vegetation spectral category
    bool shvsc = dbsr && shvsr && lBright && lVis && lNIR && lMIR1 && lMIR2 && !(hNDVI);
    if( shvsc )
      return static_cast<TOutput>(SHV);

    typedef ShadowWithBarrenLandSpectralRule<TInput, bool> SHBSRType;
    SHBSRType shbsrf = SHBSRType();
    shbsrf.SetTV1( this->m_TV1 );
    shbsrf.SetTV2( this->m_TV2 );
    shbsrf.SetSAT( this->m_SAT );

    bool shbsr = shbsrf( newPixel );
    // shadow with barren land spectral category
    bool shbsc = dbsr && shbsr && lBright && lVis && lNDVI && lNIR && lMIR1 && lMIR2;
    if( shbsc )
      return static_cast<TOutput>(SHB);

    typedef ShadowCloudOrSnowSpectralRule<TInput, bool> SHCLSNSRType;
    SHCLSNSRType shclsnsrf = SHCLSNSRType();
    shclsnsrf.SetTV1( this->m_TV1 );
    shclsnsrf.SetTV2( this->m_TV2 );
    shclsnsrf.SetSAT( this->m_SAT );

    bool shclsnsr = shclsnsrf( newPixel );
    // clouds in shadow areas spectral category
    bool shclsc = dbsr && shclsnsr && !(hNDSIVis || lNIR || lBright || lVis || hNDBSI || hTIR);
    if( shclsc )
      return static_cast<TOutput>(SHCL);

    bool hBright     = (lv[ LVType::bright ] == LVType::High);
    bool hVis        = (lv[ LVType::vis ] == LVType::High);
    // turbid water or shadow snow spectral category
    bool twashsnsc = dbsr && shclsnsr && hNDSIVis && lNIR && lMIR1 && lMIR2 && !(hBright || hVis || hNDBSI || hTIR);
    if( twashsnsc )
      return static_cast<TOutput>(TWASHSN);

    // non forested wetland spectral category
    bool wesc = dbsr && wesr && lBright && lVis && lNIR && lMIR1 && lMIR2 && !(hNDVI || hNDBSI || lNDSIVis);
    if( wesc )
      return static_cast<TOutput>(WE);

    // turbid water spectral category
    bool twasc = dbsr && lNDVI && lMIR1 && lMIR2 && !(hBright || hVis || hNIR || lNDSIVis);
    if( twasc )
      return static_cast<TOutput>(TWA);

    return static_cast<TOutput>(SU);

  }
};


} // namespace LandsatTM
} // namespace Functor
} // namespace otb

#endif
