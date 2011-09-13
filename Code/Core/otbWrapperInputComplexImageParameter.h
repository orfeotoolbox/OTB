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
#ifndef __otbWrapperInputComplexImageParameter_h
#define __otbWrapperInputComplexImageParameter_h

#include "otbImageFileReader.h"
#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{
/** \class InputComplexImageParameter
 *  \brief This class represents a InputComplexImage parameter
 */

class ITK_EXPORT InputComplexImageParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef InputComplexImageParameter           Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputComplexImageParameter,Parameter);

  /** Set value from filename */
  void SetFromFileName(const std::string& filename)
  {
    ImageFileReaderType::Pointer reader = ImageFileReaderType::New();
    reader->SetFileName(filename);
    reader->UpdateOutputInformation();
    m_Reader = reader;
    m_Image = reader->GetOutput();
    SetActive(true);
  }

  std::string GetFileName() const
  {
    if (m_Reader)
      {
      return m_Reader->GetFileName();
      }

    itkExceptionMacro(<< "No filename value");
  }

  ComplexVectorImageType* GetImage() const
  {
    return m_Image;
  }

  void SetImage(ComplexVectorImageType* image)
  {
     m_Image = image;
     m_Reader = ImageFileReaderType::Pointer();
  }

  bool HasValue() const
  {
    return m_Image.IsNotNull();
  }

  void ClearValue()
  {
    m_Image = ComplexVectorImageType::Pointer();
    m_Reader = ImageFileReaderType::Pointer();
  }
protected:
  /** Constructor */
  InputComplexImageParameter()
  {
    this->SetName("Input Image");
    this->SetKey("in");
  }

  /** Destructor */
  virtual ~InputComplexImageParameter()
  {}

  typedef otb::ImageFileReader<ComplexVectorImageType> ImageFileReaderType;
  ComplexVectorImageType::Pointer m_Image;
  ImageFileReaderType::Pointer m_Reader;

private:
  InputComplexImageParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class InputComplexImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
