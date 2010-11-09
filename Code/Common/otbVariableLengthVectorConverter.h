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
#ifndef __otbVariableLengthVectorConverter_h
#define __otbVariableLengthVectorConverter_h

#include "itkProcessObject.h"
#include "itkVariableLengthVector.h"
#include "itkNumericTraits.h"
#include "itkExceptionObject.h"
#include "itkFixedArray.h"
#include "itkHistogram.h"
#include "itkSmartPointer.h"


namespace otb
{
/**
 * \class VariableLengthVectorConverter
 * \brief Convert any data container type into a VariableLengthVector.
 *
 * To be usable, the desired convertion must be implemented through
 * partial specialisation mecanism.
 * 
 */

//Base
template< class TInputType, class TPrecisionType = double >
class ITK_EXPORT VariableLengthVectorConverter :
public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef VariableLengthVectorConverter                Self;
  typedef itk::ProcessObject                           Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;
 
  /** Run-time type information (and related methods). */
  itkTypeMacro(VariableLengthVectorConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self); 

  typedef TPrecisionType                                            OutputPrecisionType;
  typedef typename itk::VariableLengthVector<OutputPrecisionType>   OutputType;
  typedef TInputType                                                InputType;
  
  OutputType Convert (InputType input)
  {
    itkGenericExceptionMacro( << "Type Convertion Not Implemented." << std::endl );
  }

protected:
  VariableLengthVectorConverter(){}
  virtual ~VariableLengthVectorConverter(){}
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << "Attempt to use inexistant implementation of the converter!" 
       << std::endl;
  }

private:
  VariableLengthVectorConverter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};



// Real Matrix
template< class TInternalInputType, class TPrecisionType >
class ITK_EXPORT VariableLengthVectorConverter<std::vector<std::vector<TInternalInputType> >,
                                               TPrecisionType> :
public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef VariableLengthVectorConverter                Self;
  typedef itk::ProcessObject                           Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;
 
  /** Run-time type information (and related methods). */
  itkTypeMacro(VariableLengthVectorConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self); 

  typedef TPrecisionType                                            OutputPrecisionType;
  typedef typename itk::VariableLengthVector<OutputPrecisionType>   OutputType;
  typedef typename std::vector<std::vector<TInternalInputType> >    InputType;

  OutputType Convert(InputType input);

protected:
  VariableLengthVectorConverter(){}
  virtual ~VariableLengthVectorConverter(){}
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << "Converter: std::vector<std::vector<RealType>>  => VariableLengthVector<RealType>" 
       << std::endl;
  }

private:
  VariableLengthVectorConverter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};



//Complex Matrix
template< class TInternalInputType, class TPrecisionType >
class ITK_EXPORT VariableLengthVectorConverter<std::vector<std::vector<std::complex<TInternalInputType> > >,
                                               TPrecisionType> :
public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef VariableLengthVectorConverter                Self;
  typedef itk::ProcessObject                           Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;
 
  /** Run-time type information (and related methods). */
  itkTypeMacro(VariableLengthVectorConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self); 

  typedef TPrecisionType                                                           OutputPrecisionType;
  typedef typename itk::VariableLengthVector<OutputPrecisionType>                  OutputType;
  typedef typename std::vector<std::vector<std::complex<TInternalInputType> > >    InputType;

  OutputType Convert(InputType input);

protected:
  VariableLengthVectorConverter(){}
  virtual ~VariableLengthVectorConverter(){}
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << "Converter: std::vector<std::vector<std::complex<RealType>>>  => VariableLengthVector<RealType>" 
       << std::endl;
  }

private:
  VariableLengthVectorConverter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};



//Fixed Array
template< class TInternalInputType, unsigned int VArrayDimension, class TPrecisionType >
class ITK_EXPORT VariableLengthVectorConverter<itk::FixedArray<TInternalInputType, VArrayDimension>,
                                               TPrecisionType> :
public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef VariableLengthVectorConverter                Self;
  typedef itk::ProcessObject                           Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;
 
  /** Run-time type information (and related methods). */
  itkTypeMacro(VariableLengthVectorConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self); 

  typedef TPrecisionType                                                           OutputPrecisionType;
  typedef typename itk::VariableLengthVector<OutputPrecisionType>                  OutputType;
  typedef typename itk::FixedArray<TInternalInputType, VArrayDimension>            InputType;

  OutputType Convert(InputType input);
 
protected:
  VariableLengthVectorConverter(){}
  virtual ~VariableLengthVectorConverter(){}
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << "Converter: itk::FixedArray<RealType, VArrayDimension>  => VariableLengthVector<RealType>" 
       << std::endl;
  }

private:
  VariableLengthVectorConverter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

//Histogram
template< class TMeasurement, unsigned int VMeasurementVectorSize, class TFrequencyContainer, class TPrecisionType >
class ITK_EXPORT VariableLengthVectorConverter<typename itk::SmartPointer< const itk::Statistics::Histogram<TMeasurement, 
                                                                                                            VMeasurementVectorSize, 
                                                                                                            TFrequencyContainer> >,
                                               TPrecisionType> :
public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef VariableLengthVectorConverter                Self;
  typedef itk::ProcessObject                           Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;
 
  /** Run-time type information (and related methods). */
  itkTypeMacro(VariableLengthVectorConverter, ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self); 

  typedef TPrecisionType                                                           OutputPrecisionType;
  typedef typename itk::VariableLengthVector<OutputPrecisionType>                  OutputType;
  typedef typename itk::SmartPointer< const itk::Statistics::Histogram<TMeasurement, 
                                                                       VMeasurementVectorSize, 
                                                                       TFrequencyContainer> >   InputType;

  OutputType Convert(InputType input);
 
protected:
  VariableLengthVectorConverter(){}
  virtual ~VariableLengthVectorConverter(){}
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
    os << "Converter: itk::Statistics::Histogram<RealType, VMeasurementVectorSize, TFrequencyContainer>  => VariableLengthVector<RealType>" 
       << std::endl;
  }

private:
  VariableLengthVectorConverter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

}// namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVariableLengthVectorConverter.txx"
#endif

#endif
