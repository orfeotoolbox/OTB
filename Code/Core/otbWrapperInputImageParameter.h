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
#ifndef __otbWrapperInputImageParameter_h
#define __otbWrapperInputImageParameter_h

#include "otbImageFileReader.h"

#include "otbWrapperParameter.h"

namespace otb
{
namespace Wrapper
{
/** \class InputImageParameter
 *  \brief This class represents a InputImage parameter
 */

class ITK_EXPORT InputImageParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef InputImageParameter           Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputImageParameter,Parameter);

  /** Set the value */
  itkSetObjectMacro(Image, VectorImageType);

  /** Get the value */
  itkGetObjectMacro(Image, VectorImageType);

  /** Set value from filename */
  void SetFromFileName(const std::string& filename)
  {
    ImageFileReaderType::Pointer reader = ImageFileReaderType::New();
    reader->SetFileName(filename);
    reader->UpdateOutputInformation();

    // everything went fine, store the object references
    m_Reader = reader;
    m_Image = reader->GetOutput();
  }

  std::string GetFileName() const
  {
    if (m_Reader)
      {
      return m_Reader->GetFileName();
      }

    itkExceptionMacro(<< "No value yet");
  }

  /** Return any value */
  virtual boost::any GetAnyValue()
  {
    return boost::any(m_Image);
  }

protected:
  /** Constructor */
  InputImageParameter()
  {
    this->SetName("Input Image");
    this->SetKey("in");
  }

  /** Destructor */
  virtual ~InputImageParameter()
  {}

  VectorImageType::Pointer m_Image;

  typedef otb::ImageFileReader<VectorImageType> ImageFileReaderType;
  ImageFileReaderType::Pointer m_Reader;

private:
  InputImageParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class InputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
