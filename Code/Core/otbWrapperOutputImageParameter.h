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
#ifndef __otbWrapperOutputImageParameter_h
#define __otbWrapperOutputImageParameter_h

#include "otbVectorImage.h"
#include "otbWrapperParameter.h"
#include "otbImageFileWriter.h"

namespace otb
{
namespace Wrapper
{
/** \class OutputImageParameter
 *  \brief This class represents a OutputImage parameter
 */

class ITK_EXPORT OutputImageParameter : public Parameter
{
public:
  /** Standard class typedef */
  typedef OutputImageParameter           Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::ImageFileWriter<VectorImageType> WriterType;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(OutputImageParameter,Parameter);

  /** Set the value */
  itkSetObjectMacro(Image, VectorImageType);

  /** Get the value */
  itkGetObjectMacro(Image, VectorImageType);

  /** Return any value */
  void SetValue(VectorImageType* image);

  /** Return any value */
  VectorImageType* GetValue( void );

  /** Return true if a filename is set */
  bool HasValue() const;

  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);
  itkGetObjectMacro(Writer, WriterType);

  void Write();

protected:
  /** Constructor */
  OutputImageParameter();
  /** Destructor */
  virtual ~OutputImageParameter();

  VectorImageType::Pointer m_Image;
  std::string              m_FileName;
  WriterType::Pointer      m_Writer;

private:
  OutputImageParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class OutputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
