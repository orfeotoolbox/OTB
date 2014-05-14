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

#include "itkMacro.h"
#include "otbGreyLevelCooccurrenceIndexedList.h"

int otbGreyLevelCooccurrenceIndexedListNew(int argc, char * argv[])
{
  typedef float                                         PixelType;
  typedef otb::GreyLevelCooccurrenceIndexedList< PixelType > CooccurrenceIndexedListType;
  CooccurrenceIndexedListType::Pointer lightObj = CooccurrenceIndexedListType::New();

  std::cout << "Printing otb::GreyLevelCooccurrenceIndexedList: " << std::endl;
  lightObj->Print( std::cout );


  return EXIT_SUCCESS;
}
