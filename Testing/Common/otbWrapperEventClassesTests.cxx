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

#include "otbWrapperEventsSender.h"
#include "otbWrapperEventsListener.h"
#include "otbWrapperEvent.h"


int otbWrapperEventNew(int argc, char* argv[])
{
  typedef otb::Wrapper::Event EmptyParameterType;
  EmptyParameterType event();

  return EXIT_SUCCESS;
}

int otbWrapperEventsSenderNew(int argc, char* argv[])
{
  typedef otb::Wrapper::EventsSender< otb::Wrapper::Event > EmptyParameterType;
  EmptyParameterType eventSender();

  return EXIT_SUCCESS;
}

int otbWrapperEventsListenerNew(int argc, char* argv[])
{
  typedef otb::Wrapper::EventsListener< otb::Wrapper::Event > EmptyParameterType;
  EmptyParameterType eventListener();

  return EXIT_SUCCESS;
}


