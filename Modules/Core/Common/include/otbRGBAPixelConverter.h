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

#ifndef __otbRGBAPixelConverter_h
#define __otbRGBAPixelConverter_h

#include "itkProcessObject.h"
#include "itkRGBAPixel.h"
#include "itkRGBPixel.h"

namespace otb
{
/**
 * \class RGBAPixelConverter
 * \brief Convert an itk::RGBApixel<InternalType> into another pixel
 * type
 *
 * To be usable, the desired convertion must be implemented through
 * partial specialisation mecanism.
 *
 */


//RGBA to Scalar
template <class TInternalInputPixelType, class TOutputPixelType>
class ITK_EXPORT RGBAPixelConverter :
public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef RGBAPixelConverter                           Self;
  typedef itk::ProcessObject                           Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RGBAPixelConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename itk::RGBAPixel<TInternalInputPixelType>   InputPixelType;
  typedef TOutputPixelType                                   OutputPixelType;

  OutputPixelType Convert (InputPixelType input);

protected:
  RGBAPixelConverter(){}
  virtual ~RGBAPixelConverter(){}
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  RGBAPixelConverter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

//RGBA to RGBA
template <class TInternalInputPixelType, class TInternalOutputPixelType>
class ITK_EXPORT RGBAPixelConverter<TInternalInputPixelType,
                                    typename itk::RGBAPixel<TInternalOutputPixelType> > :
public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef RGBAPixelConverter                           Self;
  typedef itk::ProcessObject                           Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RGBAPixelConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename itk::RGBAPixel<TInternalInputPixelType>   InputPixelType;
  typedef typename itk::RGBAPixel<TInternalOutputPixelType>  OutputPixelType;

  OutputPixelType Convert (InputPixelType input);

protected:
  RGBAPixelConverter(){}
  virtual ~RGBAPixelConverter(){}
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  RGBAPixelConverter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

//RGBA to RGB
template <class TInternalInputPixelType, class TInternalOutputPixelType>
class ITK_EXPORT RGBAPixelConverter<TInternalInputPixelType,
                                    typename itk::RGBPixel<TInternalOutputPixelType> > :
public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef RGBAPixelConverter                           Self;
  typedef itk::ProcessObject                           Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(RGBAPixelConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename itk::RGBAPixel<TInternalInputPixelType>   InputPixelType;
  typedef typename itk::RGBPixel<TInternalOutputPixelType>  OutputPixelType;

  OutputPixelType Convert (InputPixelType input);

protected:
  RGBAPixelConverter(){}
  virtual ~RGBAPixelConverter(){}
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  RGBAPixelConverter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRGBAPixelConverter.txx"
#endif

#endif
