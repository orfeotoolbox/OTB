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
#ifndef otbCurlHelperInterface_h
#define otbCurlHelperInterface_h

#include <iosfwd>
#include "itkObject.h"
#include "itkObjectFactory.h"
#include "OTBCurlAdaptersExport.h"

namespace otb
{
/**
 * \class CurlHelperInterface
 * \brief Class to use the curl capabilities from OTB
 *
 * This class implement the interface of the curl access allowing for
 * different implementation to be used (include stubs for test).
 *
 *
 * \ingroup OTBCurlAdapters
 */
class OTBCurlAdapters_EXPORT CurlHelperInterface : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef CurlHelperInterface           Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::Object                   Superclass;

  itkTypeMacro(CurlHelperInterface, itk::Object);

  virtual bool TestUrlAvailability(const std::string& url) const = 0;

  virtual int RetrieveFile(const std::ostringstream& urlStream, std::string filename) const = 0;

  virtual int RetrieveFile(const std::string& urlString, std::string filename) const = 0;

  virtual int RetrieveUrlInMemory(const std::string& urlString, std::string& output) const = 0;

  virtual int RetrieveFileMulti(const std::vector<std::string>& listURLs,
                                const std::vector<std::string>& listFiles,
                                int maxConnect) const = 0;

  static bool IsCurlAvailable();
  static bool IsCurlMultiAvailable();

protected:
  CurlHelperInterface() {}
  ~CurlHelperInterface() ITK_OVERRIDE {}

private:
  CurlHelperInterface(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

};
}
#endif
