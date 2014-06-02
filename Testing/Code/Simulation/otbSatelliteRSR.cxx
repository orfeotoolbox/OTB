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

#include "otbMacro.h"

#include "otbSatelliteRSR.h"

int otbSatelliteRSR(int argc, char * argv[])
{
  typedef otb::SatelliteRSR< double, double>  ResponseType;
  typedef ResponseType::Pointer  ResponsePointerType;

  if ( argc!=5 )
  {
    std::cout << argv[0] << std::endl << "\t" << "<RSR_filename>"<< "\t" << "<Nb total band>"<< "\t" << "<lambda>"<< "\t" << "<band>"<< std::endl;
    return EXIT_FAILURE;
  }
  //Instantiation
  ResponsePointerType  myResponse=ResponseType::New();

  myResponse->SetNbBands(atoi(argv[2]));
  //Load file into vector
  const std::string file(argv[1]);
  myResponse->Load(file);
  //itk::Indent ind;
  unsigned int NumBand = atoi(argv[4]);
  double lambda = atof(argv[3]);

  std::cout << myResponse <<std::endl;
  std::cout << "RSR("<<lambda<<","<< NumBand <<")= " << (*myResponse)(lambda, NumBand) << std::endl;
//   for(unsigned int i=0; i<myResponse->GetRSR()[NumBand]->GetResponse().size(); ++i)
//   {
//      std::cout << "RSR("<<myResponse->GetRSR()[NumBand]->GetResponse()[i]->first<<","<< NumBand <<")= " << myResponse->GetRSR()[NumBand]->GetResponse()[i]->second << std::endl;
//   }


  return EXIT_SUCCESS;
}

int otbSatelliteRSRCheckValue(int argc, char * argv[])
{
  typedef otb::SatelliteRSR< double, double>  ResponseType;
  typedef ResponseType::Pointer  ResponsePointerType;

  if ( argc!=6 )
  {
  std::cout << argv[0] << std::endl << "\t" << "<RSR_filename>"<< "\t" << "<Nb total band>"<< "\t"
            << "<lambda>"<< "\t" << "<band>"<< "\t" << "<expected>" << std::endl;
  return EXIT_FAILURE;
  }
  //Instantiation
  ResponsePointerType  myResponse=ResponseType::New();

  myResponse->SetNbBands(atoi(argv[2]));
  //Load file into vector
  const std::string file(argv[1]);
  myResponse->Load(file);
  //itk::Indent ind;
  unsigned int NumBand = atoi(argv[4]);
  double lambda = atof(argv[3]);
  double expected = atof(argv[5]);

  double rsr_value = (*myResponse)(lambda, NumBand);
  
  std::cout << "RSR("<<lambda<<","<< NumBand <<")= " << rsr_value << std::endl;

  if(fabs(rsr_value - expected) > 10e-3) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
