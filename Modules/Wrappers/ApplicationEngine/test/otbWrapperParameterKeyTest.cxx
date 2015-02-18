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

#include "otbWrapperParameterKey.h"

int otbWrapperParameterKey(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{

  std::string theKey = "parent.current.child";

  otb::Wrapper::ParameterKey pKey;
  pKey.SetKey( theKey );

  if( pKey.GetKey() != theKey )
    {
      std::cout<<"Error in Set/GetKey method. Waiting for \""<<theKey<<"\" receiving "<<pKey.GetKey()<<"."<<std::endl;
      return EXIT_FAILURE;
    }

  otb::Wrapper::ParameterKey pKey1(theKey);
  if( pKey1.GetKey() != theKey )
    {
      std::cout<<"Error in construtor method. Waiting for \""<<theKey<<"\" receiving "<<pKey1.GetKey()<<"."<<std::endl;
      return EXIT_FAILURE;
    }

  otb::Wrapper::ParameterKey pKey2(theKey);
  if( pKey2.GetKey() != theKey )
    {
      std::cout<<"Error in construtor method. Waiting for \""<<theKey<<"\" receiving "<<pKey2.GetKey()<<"."<<std::endl;
      return EXIT_FAILURE;
    }


  if( pKey.GetLastElement() != "child" )
    {
      std::cout<<"Error in GetLastElement method. Waiting for \"child\" receiving "<<pKey.GetLastElement()<<"."<<std::endl;
      return EXIT_FAILURE;
    }

  if( pKey.GetFirstElement() != "parent" )
    {
      std::cout<<"Error in GetFirstElement method. Waiting for \"parent\" receiving "<<pKey.GetFirstElement()<<"."<<std::endl;
      return EXIT_FAILURE;
    }


 if( pKey.GetRoot() != "parent.current" )
    {
      std::cout<<"Error in GetRoot method. Waiting for \"parent.child\" receiving "<<pKey.GetRoot()<<"."<<std::endl;
      return EXIT_FAILURE;
    }


 std::vector<std::string> tmp = pKey.Split();
 if(tmp.size() != 3)
   {
     std::cout<<"Error in Split method. Waiting for a 3 size vector, receiving a vector of size "<<tmp.size()<<"."<<std::endl;
      return EXIT_FAILURE;
   }

 if( tmp[0] != "parent" || tmp[1] != "current" || tmp[2] != "child" )
   {
     std::cout<<"Error in Split method. Waiting for [parent, current, child], receiving :["<<tmp[0]<<", "<<tmp[1]<<", "<<tmp[2]<<"]"<<std::endl;
     return EXIT_FAILURE;
   }


  return EXIT_SUCCESS;
}
