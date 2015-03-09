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
#include "itkFixedArray.h"
#include "otbLandsatTMIndices.h"

int otbLandsatTMLinguisticLabels(int itkNotUsed(argc), char * argv[])
{
  typedef double                           OutputPixelType;
  typedef itk::FixedArray< double, 8 >     InputPixelType;

  typedef otb::Functor::LandsatTM::LinguisticVariables<InputPixelType> LVFunctorType;

  LVFunctorType lvFunct = LVFunctorType();

  double TM1 = (::atof(argv[1]));
  double TM2 = (::atof(argv[2]));
  double TM3 = (::atof(argv[3]));
  double TM4 = (::atof(argv[4]));
  double TM5 = (::atof(argv[5]));
  double TM61 = (::atof(argv[6]));
  double TM62 = (::atof(argv[7]));
  double TM7 = (::atof(argv[8]));

  unsigned int lo = LVFunctorType::Low;
  unsigned int me = LVFunctorType::Medium;
  unsigned int hi = LVFunctorType::High;


  InputPixelType pixel;
  pixel[0] = TM1;
  pixel[1] = TM2;
  pixel[2] = TM3;
  pixel[3] = TM4;
  pixel[4] = TM5;
  pixel[5] = TM61;
  pixel[6] = TM62;
  pixel[7] = TM7;


  OutputPixelType bright = otb::Functor::LandsatTM::Bright<InputPixelType, OutputPixelType>()( pixel );
  OutputPixelType vis = otb::Functor::LandsatTM::Vis<InputPixelType, OutputPixelType>()( pixel );
  OutputPixelType nir = otb::Functor::LandsatTM::NIR<InputPixelType, OutputPixelType>()( pixel );
  OutputPixelType mir1 = otb::Functor::LandsatTM::MIR1<InputPixelType, OutputPixelType>()( pixel );
  OutputPixelType mir2 = otb::Functor::LandsatTM::MIR2<InputPixelType, OutputPixelType>()( pixel );
  OutputPixelType tir = otb::Functor::LandsatTM::TIR<InputPixelType, OutputPixelType>()( pixel );
  OutputPixelType mirtir = otb::Functor::LandsatTM::MIRTIR<InputPixelType, OutputPixelType>()( pixel );
  OutputPixelType ndsivis = otb::Functor::LandsatTM::NDSIVis<InputPixelType, OutputPixelType>()( pixel );
  OutputPixelType ndbbbi = otb::Functor::LandsatTM::NDBBBI<InputPixelType, OutputPixelType>()( pixel );
  OutputPixelType ndvi = otb::Functor::LandsatTM::NDVI<InputPixelType, OutputPixelType>()( pixel );
  OutputPixelType ndbsi = otb::Functor::LandsatTM::NDBSI<InputPixelType, OutputPixelType>()( pixel );

  LVFunctorType::OutputPixelType result = lvFunct( pixel );


  // Bright
  {
  unsigned int goodResult = lo;
  if( bright > 40/255. ) goodResult = me;
  if( bright > 60/255. ) goodResult = hi;

  if( result[ LVFunctorType::bright ] != goodResult )
    return EXIT_FAILURE;
  }

  // Vis
  {
  unsigned int goodResult = lo;
  if( vis > 30/255. ) goodResult = me;
  if( vis > 50/255. ) goodResult = hi;

  if( result[ LVFunctorType::vis ] != goodResult )
    return EXIT_FAILURE;
  }

  // Nir
  {
  unsigned int goodResult = lo;
  if( nir > 40/255. ) goodResult = me;
  if( nir > 60/255. ) goodResult = hi;

  if( result[ LVFunctorType::nir ] != goodResult )
    return EXIT_FAILURE;
  }

  // Mir1
  {
  unsigned int goodResult = lo;
  if( mir1 > 40/255. ) goodResult = me;
  if( mir1 > 60/255. ) goodResult = hi;

  if( result[ LVFunctorType::mir1 ] != goodResult )
    return EXIT_FAILURE;
  }

  // Mir2
  {
  unsigned int goodResult = lo;
  if( mir2 > 30/255. ) goodResult = me;
  if( mir2 > 50/255. ) goodResult = hi;

  if( result[ LVFunctorType::mir2 ] != goodResult )
    return EXIT_FAILURE;
  }

  // Tir
  {
  unsigned int goodResult = lo;
  if( tir > 0 ) goodResult = me;
  if( tir > 28 ) goodResult = hi;

  if( result[ LVFunctorType::tir ] != goodResult )
    return EXIT_FAILURE;
  }

  // Mirtir
  {
  unsigned int goodResult = lo;
  if( mirtir > 180 ) goodResult = me;
  if( mirtir > 220 ) goodResult = hi;

  if( result[ LVFunctorType::mirtir ] != goodResult )
    return EXIT_FAILURE;
  }


  // Ndsivis
  {
  unsigned int goodResult = lo;
  if( ndsivis > 0.0 ) goodResult = me;
  if( ndsivis > 0.5 ) goodResult = hi;

  if( result[ LVFunctorType::ndsivis ] != goodResult )
    return EXIT_FAILURE;
  }

  // Ndbbbi
  {
  unsigned int goodResult = lo;
  if( ndbbbi > -0.20 ) goodResult = me;
  if( ndbbbi > 0.10 ) goodResult = hi;

  if( result[ LVFunctorType::ndbbbi ] != goodResult )
    return EXIT_FAILURE;
  }

  // Ndvi
  {
  unsigned int goodResult = lo;
  if( ndvi > 0.35 ) goodResult = me;
  if( ndvi > 0.60 ) goodResult = hi;

  if( result[ LVFunctorType::ndvi ] != goodResult )
    return EXIT_FAILURE;
  }


  // Ndbsi
  {
  unsigned int goodResult = lo;
  if( ndbsi > -0.20 ) goodResult = me;
  if( ndbsi > 0.0 ) goodResult = hi;

  if( result[ LVFunctorType::ndbsi ] != goodResult )
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
