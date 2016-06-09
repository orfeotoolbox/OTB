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
#ifndef __otbExtendedFilenameHelper_h
#define __otbExtendedFilenameHelper_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{

namespace internal
{
struct SingleBandRange
{
  int index;
};

}

struct GenericBandRange
  : std::pair<int,int>
{
  GenericBandRange()
    {
    }

  GenericBandRange(internal::SingleBandRange a)
    : std::pair<int,int>(a.index,a.index)
    {
    }

  GenericBandRange(std::pair<int,int> a)
    : std::pair<int,int>(a)
    {
      if (a.second>=0 && a.second < a.first)
        {
        throw std::range_error("Invalid range");
        }
    }
};

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

  void SetExtendedFileName(const char * extFname);
  OptionMapType GetOptionMap(void) const;

  itkGetStringMacro(ExtendedFileName);
  itkGetStringMacro(SimpleFileName);

  static bool ParseBandRange(std::string value, std::vector<GenericBandRange> &output);

protected:
  ExtendedFilenameHelper() {}
  virtual ~ExtendedFilenameHelper() {}

private:
  ExtendedFilenameHelper(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented
  itkSetStringMacro(SimpleFileName);

  std::string                          m_ExtendedFileName;
  std::string                          m_SimpleFileName;
  OptionMapType                        m_OptionMap;

};
} // end namespace otb

#endif // __otbExtendedFilenameHelper_h
