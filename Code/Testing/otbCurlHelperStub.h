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
#ifndef __otbCurlHelperStub_h
#define __otbCurlHelperStub_h

#include "otbCurlHelperInterface.h"

namespace otb
{
/**
 * \class CurlHelperStub
 * \brief Class to test classes using the curl capabilities from OTB
 *
 * This class is available only for testing purposes
 *
 */
class ITK_ABI_EXPORT CurlHelperStub : public CurlHelperInterface
{
public:
  /** Standard class typedefs. */
  typedef CurlHelperStub                Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef CurlHelperInterface           Superclass;

  itkTypeMacro(CurlHelperStub, CurlHelperInterface);
  itkNewMacro(Self);

  bool TestUrlAvailability(const std::string& url) const;
  int RetrieveUrlInMemory(const std::string& urlString, std::string& output) const;
  int RetrieveFile(const std::ostringstream& urlStream, std::string filename) const;
  int RetrieveFile(const std::string& urlString, std::string filename) const;

  int RetrieveFileMulti(const std::vector<std::string>& listURLs,
                        const std::vector<std::string>& listFiles,
                        int maxConnect) const;
protected:
  CurlHelperStub() {}
  virtual ~CurlHelperStub() {}

private:
  CurlHelperStub(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented


};
}
#endif
