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
#ifndef __otbSpectralSensitivityReader_h
#define __otbSpectralSensitivityReader_h

#include "itkProcessObject.h"
#include "itkImageBase.h"
#include "otbObjectList.h"
#include "otbFilterFunctionValues.h"

namespace otb
{

/** \class SpectralSensitivityReader
 *  \brief Read a file that contains spectral sensitivity
 *
 *  This class read a file that contains spectral sensitivity values and fill
 *  a structure that contains man and max wavelengths, the coefficients list and the step.
 *
 *
 * \ingroup OTBOpticalCalibration
 */

class ITK_EXPORT SpectralSensitivityReader : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef SpectralSensitivityReader     Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);
  /** Standard type macros */
  itkTypeMacro(SpectralSensitivityReader, itk::ProcessObject);

  typedef FilterFunctionValues::Pointer             FFVFPointerType;
  typedef FilterFunctionValues::ValuesVectorType    ValuesVectorType;
  typedef ObjectList<FilterFunctionValues>          WavelengthSpectralBandVectorType;
  typedef WavelengthSpectralBandVectorType::Pointer WavelengthSpectralBandVectorPointerType;
  typedef itk::ImageBase<2>                         ImageType;
  typedef ImageType::Pointer                        ImagePointerType;

  /** Get/Set filename */
  itkGetConstMacro(FileName, std::string);
  itkSetStringMacro(FileName);

  /** Get/Set path */
  itkGetConstMacro(DataPath, std::string);
  itkSetStringMacro(DataPath);

  /** Override of the GetOutput() method */
  virtual WavelengthSpectralBandVectorType * GetOutput(void);
  virtual const WavelengthSpectralBandVectorType * GetOutput(void) const;

  /** Image */
  itkSetObjectMacro(Image, ImageType);
  itkGetConstObjectMacro(Image, ImageType);

protected:
  SpectralSensitivityReader();
  ~SpectralSensitivityReader() ITK_OVERRIDE;

  /** Main computation method */
  void GenerateData() ITK_OVERRIDE;

  /** Find the filename using image metadata */
  void FindFileName();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /** Struct use to remove multiple spaces in file */
  struct BothAre
{
    char c;
    BothAre(char r) : c(r) {}
    bool operator()(char l, char r) const
    {
            return r == c && l == c;
    }
};


private:
  SpectralSensitivityReader(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  std::string      m_FileName;
  std::string      m_DataPath;
  ImagePointerType m_Image;
};
} // end namespace otb

#endif
