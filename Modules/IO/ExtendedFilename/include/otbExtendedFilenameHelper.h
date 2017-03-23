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
#ifndef otbExtendedFilenameHelper_h
#define otbExtendedFilenameHelper_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{

/** \class ExtendedFilenameHelper
 *  \brief Helper to handle extended filenames.
 *
 *  \sa ImageFileReader
 *
 * \ingroup OTBExtendedFilename
 */

class ITK_EXPORT ExtendedFilenameHelper : public itk::Object
{
public:
/** Standard class typedefs. */
  typedef ExtendedFilenameHelper        Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::Object                   Superclass;

  itkTypeMacro(ExtendedFilenameHelper, itk::Object);
  itkNewMacro(Self);

  typedef std::map< std::string, std::string > OptionMapType;

  virtual void SetExtendedFileName(const char * extFname);
  const OptionMapType & GetOptionMap(void) const;
  
  itkGetStringMacro(ExtendedFileName);
  itkGetStringMacro(SimpleFileName);

  struct GenericBandRange : std::pair<int,int>
  {
    GenericBandRange() {}

    GenericBandRange(int a);

    GenericBandRange(const std::pair<int,int>& a);

    GenericBandRange(int a,int b);

    bool SetString(const std::string& str, size_t start=0 , size_t size=std::string::npos);

    void Print(std::ostream& os);
  };

  /**  Decode the string into a list of GenericBandRange, band indexes are
   *  1-based. */
  std::vector<ExtendedFilenameHelper::GenericBandRange> GetGenericBandRange(const std::string &bandRange) const;

  /** Resolve the list of band ranges into real band indexes, according to
   *  a total number of bands in the image. Note that the output indexes are
   *  zero-based (0 is the first component) */
  bool ResolveBandRange(const std::string &bandRange, const unsigned int &nbBands, std::vector<unsigned int> &output) const;

protected:
  ExtendedFilenameHelper() {}
  ~ExtendedFilenameHelper() ITK_OVERRIDE {}

private:
  ExtendedFilenameHelper(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented
  itkSetStringMacro(SimpleFileName);

  std::string                          m_ExtendedFileName;
  std::string                          m_SimpleFileName;
  OptionMapType                        m_OptionMap;

};
} // end namespace otb

#endif // otbExtendedFilenameHelper_h
