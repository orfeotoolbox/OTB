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
  std::ostringstream msg; \
  msg << "" x << std::endl; \
  this->GetLogger()->Write( itk::LoggerBase::FATAL, msg.str() ); \
  itkGenericExceptionMacro(<< "Fatal error caught. Aborting..."); \
  }

#define otbAppLogCRITICAL(x) \
  { \
  std::ostringstream msg; \
  msg << "" x << std::endl; \
  this->GetLogger()->Write( itk::LoggerBase::CRITICAL, msg.str() ); \
  }

#define otbAppLogWARNING(x) \
  { \
  std::ostringstream msg; \
  msg << "" x << std::endl; \
  this->GetLogger()->Write( itk::LoggerBase::WARNING, msg.str() ); \
  }

#define otbAppLogINFO(x) \
  { \
  std::ostringstream msg; \
  msg << "" x << std::endl; \
  this->GetLogger()->Write( itk::LoggerBase::INFO, msg.str() ); \
  }

#define otbAppLogDEBUG(x) \
  { \
  std::ostringstream msg; \
  msg << "" x << std::endl; \
  this->GetLogger()->Write( itk::LoggerBase::DEBUG, msg.str() ); \
  }

#endif
