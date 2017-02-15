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
 * - &band : select a band composition different from the input image,
 *           syntax is band=r1,r2,r3,...,rn  where each ri is a band range
 *           that can be :
 *             - a single index (1-based) : '2' means 2nd band, '-1' means last band
 *             - a range of bands : '3:' means 3rd band until the last one
 *                 ':-2' means the first bands until the second to last
 *                 '2:4' means bands 2,3 and 4
 *
 *  \sa ImageFileReader
 *
 * \ingroup OTBExtendedFilename
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
    std::pair< bool, std::string  >  bandRange;
    std::vector<std::string>         optionList;
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
  bool SkipGeomIsSet () const;
  bool GetSkipGeom () const;
  bool SkipRpcTagIsSet () const;
  bool GetSkipRpcTag () const;

  /** Test if band range extended filename is set */
  bool BandRangeIsSet () const;
  /**  Decode the string into a list of GenericBandRange, band indexes are
   *  1-based. */
  std::vector<ExtendedFilenameHelper::GenericBandRange> GetBandRange() const;

  /** Resolve the list of band ranges into real band indexes, according to
   *  a total number of bands in the image. Note that the output indexes are
   *  zero-based (0 is the first component) */
  bool ResolveBandRange(const unsigned int &nbBands, std::vector<unsigned int> &output) const;

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
