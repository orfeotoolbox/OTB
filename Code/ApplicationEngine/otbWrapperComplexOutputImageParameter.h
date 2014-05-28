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
#ifndef __otbWrapperComplexOutputImageParameter_h
#define __otbWrapperComplexOutputImageParameter_h

#include "itkImageBase.h"
#include "otbWrapperParameter.h"
#include "otbImageFileWriter.h"

namespace otb
{
namespace Wrapper
{
/** \class ComplexOutputImageParameter
 *  \brief This class represents a ComplexOutputImage parameter
 */

class ITK_ABI_EXPORT ComplexOutputImageParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef ComplexOutputImageParameter   Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef itk::ImageBase<2> ImageBaseType;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(ComplexOutputImageParameter, Parameter);

  /** Set the value */
  itkSetObjectMacro(Image, ImageBaseType);

  /** Get the value */
  itkGetObjectMacro(Image, ImageBaseType);

  /** Return any value */
  void SetValue(ImageBaseType* image);

  /** Return any value */
  ImageBaseType* GetValue( void );

  /** Set/Get PixelType to be used when saving */
  itkSetMacro(ComplexPixelType, ComplexImagePixelType);
  itkGetMacro(ComplexPixelType, ComplexImagePixelType);

  /** Set/Get available RAM value */
  itkSetMacro(RAMValue, unsigned int);
  itkGetMacro(RAMValue, unsigned int);

  /** Return true if a filename is set */
  bool HasValue() const;

  void SetFileName (const char* filename)
  {
    m_FileName = filename;
    SetActive(true);
  }
  void SetFileName (const std::string& filename)
  {
    this->SetFileName(filename.c_str());
  }

  itkGetStringMacro(FileName);

  void Write();

  itk::ProcessObject* GetWriter();

  void InitializeWriters();

protected:
  /** Constructor */
  ComplexOutputImageParameter();
  /** Destructor */
  virtual ~ComplexOutputImageParameter();

  template <class TInputImageType>
    void SwitchImageWrite();

  template <class TInputVectorImageType>
    void SwitchVectorImageWrite();

  //FloatVectorImageType::Pointer m_Image;
  ImageBaseType::Pointer m_Image;
  std::string            m_FileName;
  ComplexImagePixelType         m_ComplexPixelType;


  typedef otb::ImageFileWriter<ComplexFloatImageType>  ComplexFloatWriterType;
  typedef otb::ImageFileWriter<ComplexDoubleImageType> ComplexDoubleWriterType;


  typedef otb::ImageFileWriter<ComplexFloatVectorImageType>  ComplexVectorFloatWriterType;
  typedef otb::ImageFileWriter<ComplexDoubleVectorImageType> ComplexVectorDoubleWriterType;


  ComplexFloatWriterType::Pointer  m_ComplexFloatWriter;
  ComplexDoubleWriterType::Pointer m_ComplexDoubleWriter;

  ComplexVectorFloatWriterType::Pointer  m_ComplexVectorFloatWriter;
  ComplexVectorDoubleWriterType::Pointer m_ComplexVectorDoubleWriter;


private:
  ComplexOutputImageParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

  unsigned int                  m_RAMValue;

}; // End class OutputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
