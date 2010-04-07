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

#ifndef __otbRandomPointSetSource_h
#define __otbRandomPointSetSource_h

#include "otbPointSetSource.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb
{

template <class TOutputPointSet>
class ITK_EXPORT RandomPointSetSource
      : public PointSetSource<TOutputPointSet>
{

public:
  /** Standard class typedefs. */
  typedef RandomPointSetSource            Self;
  typedef PointSetSource<TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self)

  /** Run-time type information (and related methods). */
  itkTypeMacro(RandomPointSetSource, PointSetSource)

  /** Some convenient typedefs. */
  typedef itk::DataObject::Pointer                     DataObjectPointer;
  typedef TOutputPointSet                              OutputPointSetType;
  typedef typename OutputPointSetType::Pointer         OutputPointSetPointer;
  typedef typename OutputPointSetType::PointsContainer PointsContainerType;
  typedef typename PointsContainerType::Pointer        PointsContainerPointer;
  typedef typename OutputPointSetType::PointType       PointType;

  itkSetMacro(NumberOfPoints, unsigned int)
  itkGetMacro(NumberOfPoints, unsigned int)

  itkSetMacro(MinPoint, PointType)
  itkGetMacro(MinPoint, PointType)
  itkSetMacro(MaxPoint, PointType)
  itkGetMacro(MaxPoint, PointType)

  void SetSeed(ITK_UINT32 seed)
    {
    m_Generator->SetSeed(seed);
    }

protected:
  RandomPointSetSource();
  virtual ~RandomPointSetSource() {};

  virtual void GenerateData( void );

private:
  RandomPointSetSource(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  unsigned int m_NumberOfPoints;

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator::Pointer GeneratorPointer;
  GeneratorPointer m_Generator;

  PointType m_MinPoint;
  PointType m_MaxPoint;

};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRandomPointSetSource.txx"
#endif

#endif
