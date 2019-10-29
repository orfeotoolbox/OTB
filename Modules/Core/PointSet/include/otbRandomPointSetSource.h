/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbRandomPointSetSource_h
#define otbRandomPointSetSource_h

#include "otbPointSetSource.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb
{

/** \class RandomPointSetSource
 *  \brief This class generate a random point set
 *
 * A point set with a specific number of points is generated as an output
 * of this filter. The point are N dimensional, depending on the specified template.
 *
 * The coordinate value of the point is uniformly distributed and the maximum and
 * minimum value for the range in each dimension is provided by the
 * \code SetMinPoint \endcode and \code SetMaxPoint \endcode methods.
 *
 * \ingroup DataSources
 *
 * \ingroup OTBPointSet
 */

template <class TOutputPointSet>
class ITK_EXPORT RandomPointSetSource : public PointSetSource<TOutputPointSet>
{

public:
  /** Standard class typedefs. */
  typedef RandomPointSetSource            Self;
  typedef PointSetSource<TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(RandomPointSetSource, PointSetSource);

  /** Some convenient typedefs. */
  typedef itk::DataObject::Pointer                               DataObjectPointer;
  typedef TOutputPointSet                                        OutputPointSetType;
  typedef typename OutputPointSetType::Pointer                   OutputPointSetPointer;
  typedef typename OutputPointSetType::PointsContainer           PointsContainerType;
  typedef typename PointsContainerType::Pointer                  PointsContainerPointer;
  typedef typename OutputPointSetType::PointType                 PointType;
  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator GeneratorType;
  typedef GeneratorType::Pointer                                 GeneratorPointerType;
  typedef GeneratorType::IntegerType                             SeedType;

  itkSetMacro(NumberOfPoints, unsigned int);
  itkGetMacro(NumberOfPoints, unsigned int);

  itkSetMacro(MinPoint, PointType);
  itkGetMacro(MinPoint, PointType);
  itkSetMacro(MaxPoint, PointType);
  itkGetMacro(MaxPoint, PointType);

  void SetSeed(SeedType seed)
  {
    m_Generator->SetSeed(seed);
  }

protected:
  RandomPointSetSource();
  ~RandomPointSetSource() override
  {
  }

  void GenerateData(void) override;

private:
  RandomPointSetSource(const Self&) = delete;
  void operator=(const Self&) = delete;

  unsigned int m_NumberOfPoints;

  GeneratorPointerType m_Generator;

  PointType m_MinPoint;
  PointType m_MaxPoint;
};
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRandomPointSetSource.hxx"
#endif

#endif
