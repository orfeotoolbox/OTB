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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkImage.h"
#include <iostream>

#include "otbMapProjections.h"

int otbMapProjectionsNew( int argc, char* argv[] )
{
  try 
    {        
        otb::AlbersProjection::Pointer lAlbersProjection = otb::AlbersProjection::New();
        otb::AzimEquDistProjection::Pointer lAzimEquDistProjection = otb::AzimEquDistProjection::New();
        otb::BngProjection::Pointer lBngProjection = otb::BngProjection::New();
        otb::BonneProjection::Pointer lBonneProjection = otb::BonneProjection::New();
        otb::CadrgProjection::Pointer lCadrgProjection = otb::CadrgProjection::New();
        otb::CassiniProjection::Pointer lCassiniProjection = otb::CassiniProjection::New();
        otb::CylEquAreaProjection::Pointer lCylEquAreaProjection = otb::CylEquAreaProjection::New();
        otb::Eckert4Projection::Pointer lEckert4Projection = otb::Eckert4Projection::New();
        otb::Eckert6Projection::Pointer lEckert6Projection = otb::Eckert6Projection::New();
        otb::GnomonicProjection::Pointer lGnomonicProjection = otb::GnomonicProjection::New();
        otb::LambertConformalConicProjection::Pointer lLambertConformalConicProjection = otb::LambertConformalConicProjection::New();
        otb::LlxyProjection::Pointer lLlxyProjection = otb::LlxyProjection::New();
        otb::EquDistCylProjection::Pointer lEquDistCylProjection = otb::EquDistCylProjection::New();
        otb::MercatorProjection::Pointer lMercatorProjection = otb::MercatorProjection::New();
        otb::MillerProjection::Pointer lMillerProjection = otb::MillerProjection::New();
        otb::MollweidProjection::Pointer lMollweidProjection = otb::MollweidProjection::New();
        otb::NewZealandMapGridProjection::Pointer lNewZealandMapGridProjection = otb::NewZealandMapGridProjection::New();
        otb::ObliqueMercatorProjection::Pointer lObliqueMercatorProjection = otb::ObliqueMercatorProjection::New();
        otb::OrthoGraphicProjection::Pointer lOrthoGraphicProjection = otb::OrthoGraphicProjection::New();
        otb::PolarStereoProjection::Pointer lPolarStereoProjection = otb::PolarStereoProjection::New();
        otb::PolyconicProjection::Pointer lPolyconicProjection = otb::PolyconicProjection::New();
        otb::SinusoidalProjection::Pointer lSinusoidalProjection = otb::SinusoidalProjection::New();
        otb::SpaceObliqueMercatorProjection::Pointer lSpaceObliqueMercatorProjection = otb::SpaceObliqueMercatorProjection::New();
        otb::StereographicProjection::Pointer lStereographicProjection = otb::StereographicProjection::New();
        otb::TransCylEquAreaProjection::Pointer lTransCylEquAreaProjection = otb::TransCylEquAreaProjection::New();
        otb::TransMercatorProjection::Pointer lTransMercatorProjection = otb::TransMercatorProjection::New();
        otb::UpsProjection::Pointer lUpsProjection = otb::UpsProjection::New();
        otb::UtmProjection::Pointer lUtmProjection = otb::UtmProjection::New();
        otb::VanDerGrintenProjection::Pointer lVanDerGrintenProjection = otb::VanDerGrintenProjection::New();


    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  // Software Guide : EndCodeSnippet

//#endif
  return EXIT_SUCCESS;
}


