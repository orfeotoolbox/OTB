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
#ifndef __otbMRFEnergy_h
#define __otbMRFEnergy_h

#include "itkConstNeighborhoodIterator.h"
#include "itkObject.h"
#include "itkArray.h"

namespace otb
{
/**
 * \class MRFEnergy
 * \brief This is the base class for energy function used in the MRF framework
 *
 * Derived class must reimplement the GetSingleValue() method.
 *
 * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
 *
 * \ingroup Markov
 */
template<class TInput1, class TInput2>
class ITK_EXPORT MRFEnergy : public itk::Object
{
public:
  typedef MRFEnergy                     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInput1                               InputImageType;
  typedef TInput2                               LabelledImageType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename LabelledImageType::PixelType LabelledImagePixelType;

  typedef itk::ConstNeighborhoodIterator
  <LabelledImageType>     LabelledNeighborhoodIterator;
  typedef itk::ConstNeighborhoodIterator
  <InputImageType>        InputNeighborhoodIterator;

  typedef itk::Array<double> ParametersType;

  itkNewMacro(Self);

  itkTypeMacro(MRFEnergy, itk::Object);

  itkSetMacro(NumberOfParameters, unsigned int);
  itkGetConstMacro(NumberOfParameters, unsigned int);

  // Get the parameters
  const ParametersType& GetParameters(void) const
  {
    return this->m_Parameters;
  }

  void SetParameters(const ParametersType& parameters)
  {
    if (parameters.Size() != m_NumberOfParameters)
      {
      itkExceptionMacro(<< "Invalid number of parameters");
      }
    m_Parameters = parameters;
    this->Modified();
  }

  virtual double GetSingleValue(const InputImagePixelType& value1, const LabelledImagePixelType& value2)
  {
    itkExceptionMacro(<< "GetSingleValue() has to be declared in child classes.");
  }

  virtual double GetValue(const InputImagePixelType& value1, const LabelledImagePixelType& value2)
  {
    return GetSingleValue(value1, value2);
  }

  virtual double GetValue(const LabelledNeighborhoodIterator& it, const LabelledImagePixelType& value2)
  {
    double              result = 0.0;
    unsigned int        centerIndex = it.GetCenterNeighborhoodIndex();
    InputImagePixelType value1; //TODO put also the other neighborhood ?
    bool                isInside = false;
    unsigned int        insideNeighbors = 0;
    for (unsigned long pos = 0; pos < it.Size(); ++pos)
      {
      if (pos !=  centerIndex) //TODO put outside loop for faster access ?
        {
        value1 = it.GetPixel(pos, isInside);
        if (isInside)
          {
          result += GetSingleValue(value1, value2);
          ++insideNeighbors;
          }
        }
      }
    return result / insideNeighbors;
  }

  virtual double GetValue(const InputNeighborhoodIterator& it, const LabelledImagePixelType& value2)
  {
    double              result = 0.0;
    unsigned int        centerIndex = it.GetCenterNeighborhoodIndex();
    InputImagePixelType value1; //TODO put also the other neighborhood ?
    bool                isInside = false;
    unsigned int        insideNeighbors = 0;
    for (unsigned long pos = 0; pos < it.Size(); ++pos)
      {
      if (pos !=  centerIndex) //TODO put outside loop for faster access ?
        {
        value1 = it.GetPixel(pos, isInside);
        if (isInside)
          {
          result += GetSingleValue(value1, value2);
          ++insideNeighbors;
          }
        }
      }
    return result / insideNeighbors;
  }

protected:
  // The constructor and destructor.
  MRFEnergy() :
    m_NumberOfParameters(1),
    m_Parameters(0) {};
  virtual ~MRFEnergy() {}
  unsigned int   m_NumberOfParameters;
  ParametersType m_Parameters;
};

template<class TInput2>
class ITK_EXPORT MRFEnergy<TInput2, TInput2> : public itk::Object
{
public:
  typedef MRFEnergy                     Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInput2                               LabelledImageType;
  typedef typename LabelledImageType::PixelType LabelledImagePixelType;

  typedef itk::ConstNeighborhoodIterator
  <LabelledImageType>     LabelledNeighborhoodIterator;
  typedef itk::Array<double> ParametersType;

  itkNewMacro(Self);

  itkTypeMacro(MRFEnergy, itk::Object);

  itkSetMacro(NumberOfParameters, unsigned int);
  itkGetConstMacro(NumberOfParameters, unsigned int);

  // Get the parameters
  const ParametersType& GetParameters(void) const
  {
    return this->m_Parameters;
  }

  void SetParameters(const ParametersType& parameters)
  {
    if (parameters.Size() != m_NumberOfParameters)
      {
      itkExceptionMacro(<< "Invalid number of parameters");
      }
    m_Parameters = parameters;
    this->Modified();
  }

  virtual double GetSingleValue(const LabelledImagePixelType& value1, const LabelledImagePixelType& value2)
  {
    itkExceptionMacro(<< "GetSingleValue() has to be declared in child classes.");
  }

  virtual double GetValue(const LabelledImagePixelType& value1, const LabelledImagePixelType& value2)
  {
    return GetSingleValue(value1, value2);
  }

  virtual double GetValue(const LabelledNeighborhoodIterator& it, const LabelledImagePixelType& value2)
  {
    double                 result = 0.0;
    unsigned int           centerIndex = it.GetCenterNeighborhoodIndex();
    LabelledImagePixelType value1; //TODO put also the other neighborhood ?
    bool                   isInside = false;
    unsigned int           insideNeighbors = 0;
    for (unsigned long pos = 0; pos < it.Size(); ++pos)
      {
      if (pos !=  centerIndex) //TODO put outside loop for faster access ?
        {
        value1 = it.GetPixel(pos, isInside);
        if (isInside)
          {
          result += GetSingleValue(value1, value2);
          ++insideNeighbors;
          }
        }
      }
    return result / insideNeighbors;
  }

protected:
  // The constructor and destructor.
  MRFEnergy() :
    m_NumberOfParameters(1),
    m_Parameters(0)
      {};
  virtual ~MRFEnergy() {}
  unsigned int   m_NumberOfParameters;
  ParametersType m_Parameters;
};
}

#endif
