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

#include "otbWrapperApplicationRegistry.h"

int otbWrapperApplicationRegistry(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  using otb::Wrapper::ApplicationRegistry;
  std::vector<std::string> list = ApplicationRegistry::GetAvailableApplications();

  std::cout << "Available applications :" << std::endl;
  std::vector<std::string>::const_iterator it;
  for (it = list.begin(); it != list.end(); ++it)
    {
    std::cout << *it << std::endl;
    }
  return EXIT_SUCCESS;
}
