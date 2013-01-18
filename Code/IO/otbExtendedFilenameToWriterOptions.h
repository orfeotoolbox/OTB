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
#ifndef __otbExtendedFilenameToWriterOptions_h
#define __otbExtendedFilenameToWriterOptions_h

#include "itkObject.h"
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
 *
 *  \sa ImageFileWriter
 */

class ITK_EXPORT ExtendedFilenameToWriterOptions : public itk::Object
{
public:
/** Standard class typedefs. */
  typedef ExtendedFilenameToWriterOptions        Self;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;
  typedef itk::Object                            Superclass;

  itkTypeMacro(ExtendedFilenameToWriterOptions, itk::Object);
  itkNewMacro(Self);

  typedef ExtendedFilenameHelper                    FNameHelperType;
  typedef FNameHelperType::OptionMapType            MapType;
  typedef MapType::iterator                         MapIteratorType;

  /** The writer option structure. */
  typedef GDALImageIO::GDALCreationOptionsType      GDALCOType;


  struct OptionType
  {
    std::pair< bool, std::string  >              simpleFileName;
    std::pair< bool, bool  >                     writeGEOMFile;
    std::pair< bool, GDALCOType >                gdalCreationOptions;
    std::pair<bool,  std::string>                streamingType;
    std::pair<bool,  std::string>                streamingSizeMode;
    std::pair<bool,  double>                     streamingSizeValue;
    std::vector<std::string>                     optionList;
  };

  /* Set Methods */
  void SetExtendedFileName(const char * extFname);
  /* Get Methods */
  bool SimpleFileNameIsSet () const;
  const char* GetSimpleFileName () const;
  bool WriteGEOMFileIsSet () const;
  bool GetWriteGEOMFile () const;
  bool gdalCreationOptionsIsSet () const;
  GDALCOType GetgdalCreationOptions () const;
  bool StreamingTypeIsSet () const;
  std::string GetStreamingType() const;
  bool StreamingSizeModeIsSet() const;
  std::string GetStreamingSizeMode() const;
  bool StreamingSizeValueIsSet() const;
  double GetStreamingSizeValue() const;

protected:
  ExtendedFilenameToWriterOptions();
  virtual ~ExtendedFilenameToWriterOptions() {}

private:
  ExtendedFilenameToWriterOptions(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  FNameHelperType::Pointer m_FilenameHelper;
  OptionType               m_Options;

};
} // end namespace otb

#endif // __otbExtendedFilenameToWriterOptions_h
