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
#ifndef __otbExtendedFilenameToReaderOptions_h
#define __otbExtendedFilenameToReaderOptions_h

#include "itkObject.h"
#include "otbExtendedFilenameHelper.h"

namespace otb
{

/** \class ExtendedFilenameToReaderOptions
 *  \brief Converts an extended filename to reader options.
 *
 *  \sa ImageFileReader
 */

class ITK_EXPORT ExtendedFilenameToReaderOptions : public itk::Object
{
public:
/** Standard class typedefs. */
  typedef ExtendedFilenameToReaderOptions        Self;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;
  typedef itk::Object                            Superclass;

  itkTypeMacro(ExtendedFilenameToReaderOptions, itk::Object);
  itkNewMacro(Self);

  typedef ExtendedFilenameHelper                    FNameHelperType;
  typedef FNameHelperType::OptionMapType            MapType;

  /** The reader option structure. */
  struct OptionType
  {
    std::pair< bool, std::string  >  simpleFileName;
    std::pair< bool, std::string  >  extGEOMFileName;
    std::pair< bool, unsigned int >  subDatasetIndex;
    std::pair< bool, unsigned int >  resolutionFactor;
    std::pair< bool, bool         >  skipCarto;
    unsigned int                     nbSetOptions;
  };

  /* Set Methods */
  void SetExtendedFileName(const char * extFname);
  /* Get Methods */
  bool SimpleFileNameIsSet () const;
  const char* GetSimpleFileName () const;
  bool ExtGEOMFileNameIsSet () const;
  const char* GetExtGEOMFileName () const;
  bool SubDatasetIndexIsSet () const;
  unsigned int GetSubDatasetIndex () const;
  bool ResolutionFactorIsSet () const;
  unsigned int GetResolutionFactor () const;
  bool SkipCartoIsSet () const;
  bool GetSkipCarto () const;

protected:
  ExtendedFilenameToReaderOptions();
  virtual ~ExtendedFilenameToReaderOptions() {}

private:
  ExtendedFilenameToReaderOptions(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  FNameHelperType::Pointer m_FilenameHelper;
  OptionType               m_Options;

};
} // end namespace otb

#endif // __otbExtendedFilenameToReaderOptions_h
