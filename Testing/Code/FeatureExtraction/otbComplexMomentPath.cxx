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

#include "otbImageFileReader.h"
#include "otbComplexMomentPathFunction.h"
#include "itkExceptionObject.h"
#include "itkPolyLineParametricPath.h"

int otbComplexMomentPath( int argc, char * argv[] )
{
  unsigned int  p((unsigned int)::atoi(argv[1]));
  unsigned int  q((unsigned int)::atoi(argv[2]));
       
  const   unsigned int      Dimension = 2;
	  
  typedef itk::PolyLineParametricPath< Dimension >	        PathType;
  typedef otb::ComplexMomentPathFunction< PathType >              CMType;
  typedef CMType::ComplexType                                     ComplexType;

  // Dessiner un carré:
  PathType::ContinuousIndexType cindex;
  PathType::Pointer pathElt = PathType::New();

  pathElt->Initialize();

  cindex[0]=30;
  cindex[1]=30;
  pathElt->AddVertex(cindex);
  cindex[0]= 30;
  cindex[1]=130;
  pathElt->AddVertex(cindex);
  cindex[0]=130;
  cindex[1]=130;
  pathElt->AddVertex(cindex);
  cindex[0]=130;
  cindex[1]= 30;
  pathElt->AddVertex(cindex);

  CMType::Pointer function =CMType::New();

  function->SetQ(q);
  function->SetP(p);
	
  ComplexType Result;
	
  Result = function->Evaluate( *pathElt);
  std::cout << "function->Evaluate(Path)"<< Result << std::endl;	
 

  return EXIT_SUCCESS;
}

