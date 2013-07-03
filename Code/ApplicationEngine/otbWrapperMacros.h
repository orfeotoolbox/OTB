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
#ifndef __otbWrapperMacros_h
#define __otbWrapperMacros_h


#define otbAppLogFATAL(x) \
  { \
  std::ostringstream message; \
  message << "" x << std::endl; \
  this->GetLogger()->Write( itk::LoggerBase::FATAL, message.str() ); \
  itkGenericExceptionMacro(<< "Fatal error caught. Aborting..."); \
  }

#define otbAppLogCRITICAL(x) \
  { \
  std::ostringstream message; \
  message << "" x << std::endl; \
  this->GetLogger()->Write( itk::LoggerBase::CRITICAL, message.str() ); \
  }

#define otbAppLogWARNING(x) \
  { \
  std::ostringstream message; \
  message << "" x << std::endl; \
  this->GetLogger()->Write( itk::LoggerBase::WARNING, message.str() ); \
  }

#define otbAppLogINFO(x) \
  { \
  std::ostringstream message; \
  message << "" x << std::endl; \
  this->GetLogger()->Write( itk::LoggerBase::INFO, message.str() ); \
  }

#define otbAppLogDEBUG(x) \
  { \
  std::ostringstream message; \
  message << "" x << std::endl; \
  this->GetLogger()->Write( itk::LoggerBase::DEBUG, message.str() ); \
  }

#endif
