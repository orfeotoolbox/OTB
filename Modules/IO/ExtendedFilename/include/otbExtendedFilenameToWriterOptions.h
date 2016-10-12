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
#ifndef otbExtendedFilenameToWriterOptions_h
#define otbExtendedFilenameToWriterOptions_h

#include "otbExtendedFilenameHelper.h"
#include "otbGDALImageIO.h"

namespace otb
{

/** \class ExtendedFilenameToWriterOptions
 *  \brief Converts an extended filename to writer options.
 *
 * Available options for extended file name are:
 * - &writegeom=ON : to activate the creation of an external geom file
 * - &gdal:co:<KEY>=<VALUE> : the gdal creation option <KEY>
 * - streaming modes
 * - box
 * See http://wiki.orfeo-toolbox.org/index.php/ExtendedFileName
 *
 *  \sa ImageFileWriter
 *
 * \ingroup OTBExtendedFilename
 */

class ITK_EXPORT ExtendedFilenameToWriterOptions : public ExtendedFilenameHelper
{
public:
/** Standard class typedefs. */
  typedef ExtendedFilenameToWriterOptions        Self;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;
  typedef ExtendedFilenameHelper                 Superclass;

  itkTypeMacro(ExtendedFilenameToWriterOptions, otb::ExtendedFilenameHelper);
  itkNewMacro(Self);

  typedef Superclass                                FNameHelperType;
  typedef FNameHelperType::OptionMapType            MapType;
  typedef MapType::iterator                         MapIteratorType;

  /** The writer option structure. */
  typedef GDALImageIO::GDALCreationOptionsType      GDALCOType;


  struct OptionType
  {
    std::pair< bool, std::string  >              simpleFileName;
    std::pair< bool, bool  >                     writeGEOMFile;
    std::pair< bool, bool  >                     writeRPCTags;
    std::pair< bool, GDALCOType >                gdalCreationOptions;
    std::pair<bool,  std::string>                streamingType;
    std::pair<bool,  std::string>                streamingSizeMode;
    std::pair<bool,  double>                     streamingSizeValue;
    std::pair<bool,  std::string>                box;
    std::vector<std::string>                     optionList;
  };

  /* Set Methods */
  virtual void SetExtendedFileName(const char * extFname);
  /* Get Methods */
  bool SimpleFileNameIsSet () const;
  bool WriteGEOMFileIsSet () const;
  bool WriteRPCTagsIsSet() const;
  bool GetWriteGEOMFile () const;
  bool GetWriteRPCTags() const;
  bool gdalCreationOptionsIsSet () const;
  GDALCOType GetgdalCreationOptions () const;
  bool StreamingTypeIsSet () const;
  std::string GetStreamingType() const;
  bool StreamingSizeModeIsSet() const;
  std::string GetStreamingSizeMode() const;
  bool StreamingSizeValueIsSet() const;
  double GetStreamingSizeValue() const;

  bool BoxIsSet() const;
  std::string GetBox() const;


protected:
  ExtendedFilenameToWriterOptions();
  ~ExtendedFilenameToWriterOptions() ITK_OVERRIDE {}

private:
  ExtendedFilenameToWriterOptions(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  OptionType               m_Options;

};
} // end namespace otb

#endif // otbExtendedFilenameToWriterOptions_h
