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
#ifndef __otbCurlHelper_h
#define __otbCurlHelper_h

#include "otbCurlHelperInterface.h"

namespace otb
{
/**
 * \class CurlHelper
 * \brief Class to use the curl capabilities from OTB
 *
 * This class is responsible for behaving properly when curl is
 * not available, i.e. the compilation should pass, the runtime should
 * not segfault but of course, the behavior will be different.
 *
 */
class ITK_EXPORT CurlHelper : public CurlHelperInterface
{
public:
  /** Standard class typedefs. */
  typedef CurlHelper                    Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef CurlHelperInterface           Superclass;

  itkTypeMacro(CurlHelper, CurlHelperInterface);
  itkNewMacro(Self);

  bool TestUrlAvailability(const std::string& url) const;
  bool IsCurlReturnHttpError(const std::string& url) const;
  int RetrieveFile(const std::ostringstream& urlStream, std::string filename) const;
  int RetrieveFile(const std::string& urlString, std::string filename) const;

  int RetrieveUrlInMemory(const std::string& urlString, std::string& output) const;

  int RetrieveFileMulti(const std::vector<std::string>& listURLs,
                        const std::vector<std::string>& listFiles,
                        int maxConnect) const;

  itkGetMacro(Timeout,long int);
  itkSetMacro(Timeout,long int);

protected:
  CurlHelper() :
    m_Browser("Mozilla/5.0 (Windows; U; Windows NT 6.0; en-GB; rv:1.8.1.11) "
              "Gecko/20071127 Firefox/2.0.0.11"),
    m_Timeout(10) {}
  virtual ~CurlHelper() {}

private:
  CurlHelper(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  // Need to use our writing function to handle windows segfaults
  // Need to be static cause the CURL_OPT is expecting a pure C
  // function or a static c++ method.
  static size_t CallbackWriteDataToFile(void* ptr, size_t size, size_t nmemb, void* data);

  static size_t CallbackWriteDataToStringStream(void* ptr, size_t size, size_t nmemb, void* data);

  static size_t CallbackWriteDataDummy(void* ptr, size_t size, size_t nmemb, void* data);

  // Browser Agent used
  std::string m_Browser;
  int         m_Timeout;
};
}
#endif
