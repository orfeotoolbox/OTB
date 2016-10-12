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
#ifndef otbExtendedFilenameToReaderOptions_h
#define otbExtendedFilenameToReaderOptions_h

#include "otbExtendedFilenameHelper.h"

namespace otb
{

/** \class ExtendedFilenameToReaderOptions
 *  \brief Converts an extended filename to reader options.
 *
 * Available options for extended file name are:
 * - &geom : to specify an external geom file
 * - &sdataidx : sub-dataset index for composite files
 * - &resol : resolution factor for jpeg200 files
 * - &skipcarto : switch to skip the cartographic information
 * - &skipgeom  : switch to skip the geometric information
 *
 *  \sa ImageFileReader
 *
 * \ingroup OTBExtendedFilename
 */

class ITK_EXPORT ExtendedFilenameToReaderOptions : public ExtendedFilenameHelper
{
public:
/** Standard class typedefs. */
  typedef ExtendedFilenameToReaderOptions        Self;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;
  typedef ExtendedFilenameHelper                 Superclass;

  itkTypeMacro(ExtendedFilenameToReaderOptions, otb::ExtendedFilenameHelper);
  itkNewMacro(Self);

  typedef Superclass                                FNameHelperType;
  typedef FNameHelperType::OptionMapType            MapType;
  typedef MapType::iterator                         MapIteratorType;

  /** The reader option structure. */
  struct OptionType
  {
    std::pair< bool, std::string  >  simpleFileName;
    std::pair< bool, std::string  >  extGEOMFileName;
    std::pair< bool, unsigned int >  subDatasetIndex;
    std::pair< bool, unsigned int >  resolutionFactor;
    std::pair< bool, bool         >  skipCarto;
    std::pair< bool, bool         >  skipGeom;
    std::pair< bool, bool         >  skipRpcTag;
    std::vector<std::string>         optionList;
  };

  /* Set Methods */
  virtual void SetExtendedFileName(const char * extFname);
  /* Get Methods */
  bool SimpleFileNameIsSet () const;
  bool ExtGEOMFileNameIsSet () const;
  const char* GetExtGEOMFileName () const;
  bool SubDatasetIndexIsSet () const;
  unsigned int GetSubDatasetIndex () const;
  bool ResolutionFactorIsSet () const;
  unsigned int GetResolutionFactor () const;
  bool SkipCartoIsSet () const;
  bool GetSkipCarto () const;
  bool SkipGeomIsSet () const;
  bool GetSkipGeom () const;
  bool SkipRpcTagIsSet () const;
  bool GetSkipRpcTag () const;

protected:
  ExtendedFilenameToReaderOptions();
  ~ExtendedFilenameToReaderOptions() ITK_OVERRIDE {}

private:
  ExtendedFilenameToReaderOptions(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  OptionType               m_Options;

};
} // end namespace otb

#endif // otbExtendedFilenameToReaderOptions_h
