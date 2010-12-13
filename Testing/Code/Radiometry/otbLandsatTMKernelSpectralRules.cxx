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
#include "itkExceptionObject.h"
#include "itkFixedArray.h"
#include "otbLandsatTMIndices.h"
#include <vector>
#include <algorithm>

int otbLandsatTMKernelSpectralRules(int argc, char * argv[])
{

  typedef double PrecisionType;
  typedef itk::FixedArray< PrecisionType, 8 >     InputPixelType;



  double TM1 = (::atof(argv[1]));
  double TM2 = (::atof(argv[2]));
  double TM3 = (::atof(argv[3]));
  double TM4 = (::atof(argv[4]));
  double TM5 = (::atof(argv[5]));
  double TM61 = (::atof(argv[6]));
  double TM62 = (::atof(argv[7]));
  double TM7 = (::atof(argv[8]));

  InputPixelType pixel;
  pixel[0] = TM1;
  pixel[1] = TM2;
  pixel[2] = TM3;
  pixel[3] = TM4;
  pixel[4] = TM5;
  pixel[5] = TM61;
  pixel[6] = TM62;
  pixel[7] = TM7;

  std::vector< PrecisionType > v123;
  v123.push_back(TM1);
  v123.push_back(TM2);
  v123.push_back(TM3);

  PrecisionType max123 = *(max_element ( v123.begin(), v123.end() ));
  PrecisionType min123 = *(min_element ( v123.begin(), v123.end() ));

  PrecisionType TV1 = 0.7;
  PrecisionType TV2 = 0.5;

  typedef otb::Functor::LandsatTM::ThickCloudsSpectralRule<InputPixelType> R1FunctorType;
  R1FunctorType r1Funct = R1FunctorType();
  bool result = r1Funct(pixel);
  bool goodResult = (min123 >= (TV1 * max123))
    and (max123 <= TV1 * TM4)
    and (TM5 <= TV1 * TM4)
    and (TM5 >= TV1 * max123)
    and (TM7 <= TV1 * TM4);

  std::cout << "Rule 1 " << goodResult << " " << result << std::endl;
  if( result!=goodResult ) return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}
