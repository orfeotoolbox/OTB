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

#include "otbVectorImage.h"
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

  typedef float                          PixelType;
  typedef otb::VectorImage<PixelType, 2> VectorImageType;

  /** Set the value */
  itkSetObjectMacro(Image, VectorImageType);

  /** Get the value */
  itkGetObjectMacro(Image, VectorImageType);

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

  VectorImageType::Pointer m_Image;

private:
  InputComplexImageParameter(const Parameter &); //purposely not implemented
  void operator =(const Parameter&); //purposely not implemented

}; // End class InputComplexImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
