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
    TWASHSN,   // snow in shadow areas
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


    typedef VegetationSpectralRule<TInput, bool> VSRType;
    VSRType vsrf = VSRType();
    vsrf.SetTV1( this->m_TV1 );
    vsrf.SetTV2( this->m_TV2 );
    vsrf.SetSAT( this->m_SAT );

    bool vsr = vsrf( newPixel );

    bool hNDBSI = (lv[ LVType::ndbsi ] == LVType::High);
    bool hNIR   = (lv[ LVType::nir ] == LVType::High);

    // strong vegetation spectral category
    bool svsc = vsr and hNDVI and !(hMIR1 or hMIR2 or hNDBSI);

    if( svsc and hNIR)
      return static_cast<TOutput>(SVHNIR);
    if( svsc and !(hNIR))
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
    bool avsc = (vsr or shvsr) and mNDVI and !(hMIR1 or hMIR2 or hNDBSI or dbsr);

    if( avsc and hNIR)
      return static_cast<TOutput>(AVHNIR);
    if( avsc and !(hNIR))
      return static_cast<TOutput>(AVLNIR);

    typedef RangelandSpectralRule<TInput, bool> RSRType;
    RSRType rsrf = RSRType();
    rsrf.SetTV1( this->m_TV1 );
    rsrf.SetTV2( this->m_TV2 );
    rsrf.SetSAT( this->m_SAT );

    bool rsr = rsrf( newPixel );
    
    // weak vegetation spectral category
    bool wvsc = (vsr or rsr or shvsr) and lNDVI and lNDBSI and lMIR1 and lMIR2 and !(dbsr);

    if( wvsc and hNIR)
      return static_cast<TOutput>(WVHNIR);
    if( wvsc and !(hNIR))
      return static_cast<TOutput>(WVLNIR);

    bool mNDBSI = (lv[ LVType::ndbsi ] == LVType::Medium);
    // strong shrub rangeland spectral category
    bool ssrsc = rsr and hNDVI and mNDBSI;
    if( ssrsc and hNIR)
      return static_cast<TOutput>(SSRHNIR);
    if( ssrsc and !(hNIR))
      return static_cast<TOutput>(SSRLNIR);

    typedef WetlandSpectralRule<TInput, bool> WESRType;
    WESRType wesrf = WESRType();
    wesrf.SetTV1( this->m_TV1 );
    wesrf.SetTV2( this->m_TV2 );
    wesrf.SetSAT( this->m_SAT );

    bool wesr = wesrf( newPixel );
    
    // average shrub rangeland spectral category
    bool asrsc = rsr and mNDVI and mNDBSI and !(shvsr or wesr);
    if( asrsc and hNIR)
      return static_cast<TOutput>(ASRHNIR);
    if( asrsc and !(hNIR))
      return static_cast<TOutput>(ASRLNIR);


    // strong herbaceous rangeland spectral category
    bool shrsc = rsr and hNDVI and hNDBSI;
    if( shrsc )
      return static_cast<TOutput>(SHR);

    typedef BarrenLandOrBuiltUpOrCloudsSpectralRule<TInput, bool> BBCSRType;
    BBCSRType bbcsrf = BBCSRType();
    bbcsrf.SetTV1( this->m_TV1 );
    bbcsrf.SetTV2( this->m_TV2 );
    bbcsrf.SetSAT( this->m_SAT );

    bool bbcsr = bbcsrf( newPixel );
    // average herbaceous rangeland spectral category
    bool ahrsc = (rsr or bbcsr) and mNDVI and hNDBSI;
    if( ahrsc )
      return static_cast<TOutput>(AHR);

    // dark rangeland spectral category
    bool drsc = (vsr or rsr) and lNDVI and lMIR2 and !(hNIR or hMIR1 or lNDBSI);
    if( drsc )
      return static_cast<TOutput>(DR);

    // bright barren land or built up spectral category
    bool bbbsc = bbcsr and hNIR and hMIR2 and lNDVI and !(lNDBSI or lMIR1);

    bool lNDBBBI  = (lv[ LVType::ndbbbi ] == LVType::Low);
    
    bool bbbhtirsc = bbbsc and hTIR;

    if( bbbhtirsc and !(lNDBBBI) )
      return static_cast<TOutput>(BBBHTIRF);
    if( bbbhtirsc and lNDBBBI )
      return static_cast<TOutput>(BBBHTIRNF);

    bool bbbltirsc = bbbsc and !(hTIR);
    if( bbbltirsc and !(lNDBBBI) )
      return static_cast<TOutput>(BBBLTIRF);
    if( bbbltirsc and lNDBBBI )
      return static_cast<TOutput>(BBBLTIRNF);

    typedef FlatResponseBarrenLandOrBuiltUpSpectralRule<TInput, bool> FBBSRType;
    FBBSRType fbbsrf = FBBSRType();
    fbbsrf.SetTV1( this->m_TV1 );
    fbbsrf.SetTV2( this->m_TV2 );
    fbbsrf.SetSAT( this->m_SAT );

    bool fbbsr = fbbsrf( newPixel );
    // strong barren land or built up spectral category
    bool sbbsc = (bbcsr or fbbsr) and lNDVI and lNDBSI and !( hNIR or lMIR1);
    
    bool sbbhtirsc = sbbsc and hTIR;

    if( sbbhtirsc and (dbsr or fbbsr) )
      return static_cast<TOutput>(SBBHTIRF);
    if( sbbhtirsc and !(dbsr or fbbsr) )
      return static_cast<TOutput>(SBBHTIRNF);

    bool sbbltirsc = sbbsc and !(hTIR);
    if( sbbltirsc and (dbsr or fbbsr) )
      return static_cast<TOutput>(SBBLTIRF);
    if( sbbltirsc and !((dbsr or fbbsr)) )
      return static_cast<TOutput>(SBBLTIRNF);

    // average barren land or built up spectral category
    bool abbsc = (bbcsr or fbbsr) and lNDVI and mNDBSI and !( hNIR or lMIR1);
    
    bool abbhtirsc = abbsc and hTIR;

    if( abbhtirsc and !(lNDBBBI) )
      return static_cast<TOutput>(ABBHTIRF);
    if( abbhtirsc and lNDBBBI )
      return static_cast<TOutput>(ABBHTIRNF);

    bool abbltirsc = abbsc and !(hTIR);
    if( abbltirsc and !(lNDBBBI) )
      return static_cast<TOutput>(ABBLTIRF);
    if( abbltirsc and lNDBBBI )
      return static_cast<TOutput>(ABBLTIRNF);

    // dark barren land or built
    bool dbbsc = (bbcsr or fbbsr) and lNDVI and lMIR1 and !( hNIR or hMIR2 or lNDBSI);
    
    bool dbbhtirsc = dbbsc and hTIR;

    if( dbbhtirsc and (dbsr or fbbsr) )
      return static_cast<TOutput>(DBBHTIRF);
    if( dbbhtirsc and !(dbsr or fbbsr) )
      return static_cast<TOutput>(DBBHTIRNF);

    bool dbbltirsc = dbbsc and !(hTIR);
    if( dbbltirsc and (dbsr or fbbsr) )
      return static_cast<TOutput>(DBBLTIRF);
    if( dbbltirsc and !((dbsr or fbbsr)) )
      return static_cast<TOutput>(DBBLTIRNF);

    // weak rangeland spectral category
    bool wrsc = rsr and lNDVI and !(lNDBSI);
    if( wrsc )
      return static_cast<TOutput>(WR);

    // shadow area with vegetation spectral category
    bool shvsc = dbsr and shvsr and lBright and lVis and lNIR and lMIR1 and lMIR2 and !(hNDVI);
    if( shvsc )
      return static_cast<TOutput>(SHV);

    typedef ShadowWithBarrenLandSpectralRule<TInput, bool> SHBSRType;
    SHBSRType shbsrf = SHBSRType();
    shbsrf.SetTV1( this->m_TV1 );
    shbsrf.SetTV2( this->m_TV2 );
    shbsrf.SetSAT( this->m_SAT );

    bool shbsr = shbsrf( newPixel );
    // shadow with barren land spectral category
    bool shbsc = dbsr and shbsr and lBright and lVis and lNDVI and lNIR and lMIR1 and lMIR2;
    if( shbsc )
      return static_cast<TOutput>(SHB);

    typedef ShadowCloudOrSnowSpectralRule<TInput, bool> SHCLSNSRType;
    SHCLSNSRType shclsnsrf = SHCLSNSRType();
    shclsnsrf.SetTV1( this->m_TV1 );
    shclsnsrf.SetTV2( this->m_TV2 );
    shclsnsrf.SetSAT( this->m_SAT );

    bool shclsnsr = shclsnsrf( newPixel );
    // clouds in shadow areas spectral category
    bool shclsc = dbsr and shclsnsr and !(hNDSIVis or lNIR or lBright or lVis or hNDBSI or hTIR);
    if( shclsc )
      return static_cast<TOutput>(SHCL);

    bool hBright     = (lv[ LVType::bright ] == LVType::High);
    bool hVis        = (lv[ LVType::vis ] == LVType::High);
    // snow in shadow areas spectral category
    bool twashsnsc = dbsr and shclsnsr and hNDSIVis and lNIR and lMIR1 and lMIR2 and !(hBright or hVis or hNDBSI or hTIR);
    if( twashsnsc )    
      return static_cast<TOutput>(TWASHSN);

    // non forested wetland spectral category
    bool wesc = dbsr and wesr and lBright and lVis and lNIR and lMIR1 and lMIR2 and !(hNDVI or hNDBSI or lNDSIVis);
    if( wesc )
      return static_cast<TOutput>(WE);
    
    // turbid water spectral category
    bool twasc = dbsr and lNDVI and lMIR1 and lMIR2 and !(hBright or hVis or hNIR or lNDSIVis);
    if( twasc )
      return static_cast<TOutput>(TWA);
    
    return static_cast<TOutput>(SU);
    
  }
};


} // namespace LandsatTM
} // namespace Functor
} // namespace otb

#endif
