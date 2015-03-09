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


#include <iostream>
#include <fstream>

#include "itkMacro.h"
#include "otbImage.h"
#include "otbMapProjections.h"

int otbMapProjectionsNew(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  otb::AlbersInverseProjection::Pointer      lAlbersProjection = otb::AlbersInverseProjection::New();
  otb::AlbersForwardProjection::Pointer      lAlbersProjection2 = otb::AlbersForwardProjection::New();
  otb::AzimEquDistInverseProjection::Pointer lAzimEquDistProjection = otb::AzimEquDistInverseProjection::New();
  otb::AzimEquDistForwardProjection::Pointer lAzimEquDistProjection2 = otb::AzimEquDistForwardProjection::New();
  //Comment for OTB 2.2.0 because OSSIM error code -> generate Exception Fault on VS 7.1 only on Debug mode.
  //        otb::BngInverseProjection::Pointer lBngProjection = otb::BngInverseProjection::New();
  //        otb::BngForwardProjection::Pointer lBngProjection2 = otb::BngForwardProjection::New();
  otb::BonneInverseProjection::Pointer                 lBonneProjection = otb::BonneInverseProjection::New();
  otb::BonneForwardProjection::Pointer                 lBonneProjection2 = otb::BonneForwardProjection::New();
  otb::CadrgInverseProjection::Pointer                 lCadrgProjection = otb::CadrgInverseProjection::New();
  otb::CadrgForwardProjection::Pointer                 lCadrgProjection2 = otb::CadrgForwardProjection::New();
  otb::CassiniInverseProjection::Pointer               lCassiniProjection = otb::CassiniInverseProjection::New();
  otb::CassiniForwardProjection::Pointer               lCassiniProjection2 = otb::CassiniForwardProjection::New();
  otb::CylEquAreaInverseProjection::Pointer            lCylEquAreaProjection = otb::CylEquAreaInverseProjection::New();
  otb::CylEquAreaForwardProjection::Pointer            lCylEquAreaProjection2 = otb::CylEquAreaForwardProjection::New();
  otb::Eckert4InverseProjection::Pointer               lEckert4Projection = otb::Eckert4InverseProjection::New();
  otb::Eckert4ForwardProjection::Pointer               lEckert4Projection2 = otb::Eckert4ForwardProjection::New();
  otb::Eckert6InverseProjection::Pointer               lEckert6Projection = otb::Eckert6InverseProjection::New();
  otb::Eckert6ForwardProjection::Pointer               lEckert6Projection2 = otb::Eckert6ForwardProjection::New();
  otb::GnomonicInverseProjection::Pointer              lGnomonicProjection = otb::GnomonicInverseProjection::New();
  otb::GnomonicForwardProjection::Pointer              lGnomonicProjection2 = otb::GnomonicForwardProjection::New();
  otb::LambertConformalConicInverseProjection::Pointer lLambertConformalConicProjection =
    otb::LambertConformalConicInverseProjection::New();
  otb::LambertConformalConicForwardProjection::Pointer lLambertConformalConicProjection2 =
    otb::LambertConformalConicForwardProjection::New();
  otb::LlxyInverseProjection::Pointer              lLlxyProjection = otb::LlxyInverseProjection::New();
  otb::LlxyForwardProjection::Pointer              lLlxyProjection2 = otb::LlxyForwardProjection::New();
  otb::EquDistCylInverseProjection::Pointer        lEquDistCylProjection = otb::EquDistCylInverseProjection::New();
  otb::EquDistCylForwardProjection::Pointer        lEquDistCylProjection2 = otb::EquDistCylForwardProjection::New();
  otb::MercatorInverseProjection::Pointer          lMercatorProjection = otb::MercatorInverseProjection::New();
  otb::MercatorForwardProjection::Pointer          lMercatorProjection2 = otb::MercatorForwardProjection::New();
  otb::MillerInverseProjection::Pointer            lMillerProjection = otb::MillerInverseProjection::New();
  otb::MillerForwardProjection::Pointer            lMillerProjection2 = otb::MillerForwardProjection::New();
  otb::MollweidInverseProjection::Pointer          lMollweidProjection = otb::MollweidInverseProjection::New();
  otb::MollweidForwardProjection::Pointer          lMollweidProjection2 = otb::MollweidForwardProjection::New();
  otb::NewZealandMapGridInverseProjection::Pointer lNewZealandMapGridProjection =
    otb::NewZealandMapGridInverseProjection::New();
  otb::NewZealandMapGridForwardProjection::Pointer lNewZealandMapGridProjection2 =
    otb::NewZealandMapGridForwardProjection::New();
  otb::ObliqueMercatorInverseProjection::Pointer lObliqueMercatorProjection =
    otb::ObliqueMercatorInverseProjection::New();
  otb::ObliqueMercatorForwardProjection::Pointer lObliqueMercatorProjection2 =
    otb::ObliqueMercatorForwardProjection::New();
  otb::OrthoGraphicInverseProjection::Pointer lOrthoGraphicProjection =
    otb::OrthoGraphicInverseProjection::New();
  otb::OrthoGraphicForwardProjection::Pointer lOrthoGraphicProjection2 =
    otb::OrthoGraphicForwardProjection::New();
  otb::PolarStereoInverseProjection::Pointer          lPolarStereoProjection = otb::PolarStereoInverseProjection::New();
  otb::PolarStereoForwardProjection::Pointer          lPolarStereoProjection2 = otb::PolarStereoForwardProjection::New();
  otb::PolyconicInverseProjection::Pointer            lPolyconicProjection = otb::PolyconicInverseProjection::New();
  otb::PolyconicForwardProjection::Pointer            lPolyconicProjection2 = otb::PolyconicForwardProjection::New();
  otb::SinusoidalInverseProjection::Pointer           lSinusoidalProjection = otb::SinusoidalInverseProjection::New();
  otb::SinusoidalForwardProjection::Pointer           lSinusoidalProjection2 = otb::SinusoidalForwardProjection::New();
  otb::SpaceObliqueMercatorInverseProjection::Pointer lSpaceObliqueMercatorProjection =
    otb::SpaceObliqueMercatorInverseProjection::New();
  otb::SpaceObliqueMercatorForwardProjection::Pointer lSpaceObliqueMercatorProjection2 =
    otb::SpaceObliqueMercatorForwardProjection::New();
  otb::StereographicInverseProjection::Pointer lStereographicProjection =
    otb::StereographicInverseProjection::New();
  otb::StereographicForwardProjection::Pointer lStereographicProjection2 =
    otb::StereographicForwardProjection::New();
  otb::TransCylEquAreaInverseProjection::Pointer lTransCylEquAreaProjection =
    otb::TransCylEquAreaInverseProjection::New();
  otb::TransCylEquAreaForwardProjection::Pointer lTransCylEquAreaProjection2 =
    otb::TransCylEquAreaForwardProjection::New();
  otb::TransMercatorInverseProjection::Pointer lTransMercatorProjection =
    otb::TransMercatorInverseProjection::New();
  otb::TransMercatorForwardProjection::Pointer lTransMercatorProjection2 =
    otb::TransMercatorForwardProjection::New();
  otb::UpsInverseProjection::Pointer           lUpsProjection = otb::UpsInverseProjection::New();
  otb::UpsForwardProjection::Pointer           lUpsProjection2 = otb::UpsForwardProjection::New();
  otb::UtmInverseProjection::Pointer           lUtmProjection = otb::UtmInverseProjection::New();
  otb::UtmForwardProjection::Pointer           lUtmProjection2 = otb::UtmForwardProjection::New();
  otb::VanDerGrintenInverseProjection::Pointer lVanDerGrintenProjection =
    otb::VanDerGrintenInverseProjection::New();
  otb::VanDerGrintenForwardProjection::Pointer lVanDerGrintenProjection2 =
    otb::VanDerGrintenForwardProjection::New();
  otb::Lambert2EtenduInverseProjection::Pointer lLambert2Etendu = otb::Lambert2EtenduInverseProjection::New();
  otb::Lambert2EtenduForwardProjection::Pointer lLambert2Etendu2 = otb::Lambert2EtenduForwardProjection::New();
  otb::Lambert93InverseProjection::Pointer      lLambert93 = otb::Lambert93InverseProjection::New();
  otb::Lambert93ForwardProjection::Pointer      lLambert93_2 = otb::Lambert93ForwardProjection::New();
  otb::SVY21InverseProjection::Pointer          lSVY21 = otb::SVY21InverseProjection::New();
  otb::SVY21ForwardProjection::Pointer          lSVY21_2 = otb::SVY21ForwardProjection::New();

  return EXIT_SUCCESS;
}


//Test the specific accessors for some map projection
int otbMapProjectionsTest(int itkNotUsed(argc), char * argv[])
{
  char * filename = argv[1];
  std::ofstream file;
  file.open(filename);

  otb::Eckert4InverseProjection::Pointer lEckert4Projection = otb::Eckert4InverseProjection::New();
  file << lEckert4Projection->GetFalseEasting() << std::endl;
  file << lEckert4Projection->GetFalseNorthing() << std::endl;
  lEckert4Projection->SetFalseEasting(500000);
  lEckert4Projection->SetFalseNorthing(500000);
  file << lEckert4Projection->GetFalseEasting() << std::endl;
  file << lEckert4Projection->GetFalseNorthing() << std::endl;
  lEckert4Projection->SetParameters(200000, 300000);
  file << lEckert4Projection->GetFalseEasting() << std::endl;
  file << lEckert4Projection->GetFalseNorthing() << std::endl;

  otb::Eckert4ForwardProjection::Pointer               lEckert4Projection2 = otb::Eckert4ForwardProjection::New();
  file << lEckert4Projection2->GetFalseEasting() << std::endl;
  file << lEckert4Projection2->GetFalseNorthing() << std::endl;
  lEckert4Projection2->SetFalseEasting(500000);
  lEckert4Projection2->SetFalseNorthing(500000);
  file << lEckert4Projection2->GetFalseEasting() << std::endl;
  file << lEckert4Projection2->GetFalseNorthing() << std::endl;
  lEckert4Projection2->SetParameters(200000, 300000);
  file << lEckert4Projection2->GetFalseEasting() << std::endl;
  file << lEckert4Projection2->GetFalseNorthing() << std::endl;


  otb::MollweidInverseProjection::Pointer          lMollweidProjection = otb::MollweidInverseProjection::New();
  file << lMollweidProjection->GetFalseEasting() << std::endl;
  file << lMollweidProjection->GetFalseNorthing() << std::endl;
  lMollweidProjection->SetFalseEasting(500000);
  lMollweidProjection->SetFalseNorthing(500000);
  file << lMollweidProjection->GetFalseEasting() << std::endl;
  file << lMollweidProjection->GetFalseNorthing() << std::endl;
  lMollweidProjection->SetParameters(200000, 300000);
  file << lMollweidProjection->GetFalseEasting() << std::endl;
  file << lMollweidProjection->GetFalseNorthing() << std::endl;


  otb::MollweidForwardProjection::Pointer          lMollweidProjection2 = otb::MollweidForwardProjection::New();
  file << lMollweidProjection2->GetFalseEasting() << std::endl;
  file << lMollweidProjection2->GetFalseNorthing() << std::endl;
  lMollweidProjection2->SetFalseEasting(500000);
  lMollweidProjection2->SetFalseNorthing(500000);
  file << lMollweidProjection2->GetFalseEasting() << std::endl;
  file << lMollweidProjection2->GetFalseNorthing() << std::endl;
  lMollweidProjection2->SetParameters(200000, 300000);
  file << lMollweidProjection2->GetFalseEasting() << std::endl;
  file << lMollweidProjection2->GetFalseNorthing() << std::endl;

  otb::SinusoidalInverseProjection::Pointer           lSinusoidalProjection = otb::SinusoidalInverseProjection::New();
  file << lSinusoidalProjection->GetFalseEasting() << std::endl;
  file << lSinusoidalProjection->GetFalseNorthing() << std::endl;
  lSinusoidalProjection->SetFalseEasting(500000);
  lSinusoidalProjection->SetFalseNorthing(500000);
  file << lSinusoidalProjection->GetFalseEasting() << std::endl;
  file << lSinusoidalProjection->GetFalseNorthing() << std::endl;
  lSinusoidalProjection->SetParameters(200000, 300000);
  file << lSinusoidalProjection->GetFalseEasting() << std::endl;
  file << lSinusoidalProjection->GetFalseNorthing() << std::endl;

  otb::SinusoidalForwardProjection::Pointer           lSinusoidalProjection2 = otb::SinusoidalForwardProjection::New();
  file << lSinusoidalProjection2->GetFalseEasting() << std::endl;
  file << lSinusoidalProjection2->GetFalseNorthing() << std::endl;
  lSinusoidalProjection2->SetFalseEasting(500000);
  lSinusoidalProjection2->SetFalseNorthing(500000);
  file << lSinusoidalProjection2->GetFalseEasting() << std::endl;
  file << lSinusoidalProjection2->GetFalseNorthing() << std::endl;
  lSinusoidalProjection2->SetParameters(200000, 300000);
  file << lSinusoidalProjection2->GetFalseEasting() << std::endl;
  file << lSinusoidalProjection2->GetFalseNorthing() << std::endl;

  file.close();
  return EXIT_SUCCESS;
}
