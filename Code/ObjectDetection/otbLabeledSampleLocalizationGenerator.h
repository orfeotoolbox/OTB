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
#ifndef __otbLabeledSampleLocalizationGenerator_h
#define __otbLabeledSampleLocalizationGenerator_h

#include "otbVectorDataSource.h"
#include "otbVectorData.h"
#include "itkPreOrderTreeIterator.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "itkEuclideanDistanceMetric.h"


namespace otb
{
/** \class LabeledSampleLocalizationGenerator
 *  \brief Produces a VectorData from potentialy multiple VectorData
 *
 *  This generator produces a unique vector data containing labeled positions
 *  extracted from inputs.
 *
 *  Input points are transmitted to the output. In addition, 'no class'
 *  points are randomly picked inside input polygons making sure
 *  they are at least at a given distance (InhibitionRadius) of every
 *  known points.
 *
 *  Classes are specified by the VectorData with a metadata field identified by
 *  a specific key. This key can be provided by the SetClassKey() method
 *  (using "Class" as a default key).
 *  The field is retrieved by GetFieldAsInt(), thus must be int-compatible
 *
 */
template <class TVectorData>
class ITK_EXPORT LabeledSampleLocalizationGenerator :
    public VectorDataSource<TVectorData>
{
public:
  /** Standard class typedefs */
  typedef LabeledSampleLocalizationGenerator           Self;
  typedef VectorDataSource<TVectorData>                Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(LabeledSampleLocalizationGenerator, VectorDataSource);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TVectorData                                           VectorDataType;
  typedef typename VectorDataType::Pointer                      VectorDataPointerType;
  typedef typename VectorDataType::DataNodeType                 DataNodeType;
  typedef typename DataNodeType::PolygonType::RegionType        RegionType;
  typedef typename DataNodeType::PointType                      PointType;
  typedef typename DataNodeType::PolygonType::VertexType        VertexType;
  typedef typename std::vector<PointType>                       PointVectorType;

  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType> TreeIteratorType;

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;
  typedef itk::Statistics::EuclideanDistanceMetric<PointType>    EuclideanDistanceMetricType;

  /** Connects the VectorDatas from which the localizations are going to be extracted. */
  void PushBackInput(const VectorDataType *);
  const VectorDataType * GetInput(unsigned int idx) const;

  /** Field name containing the class identifier */
  itkGetConstMacro(ClassKey, std::string);
  itkSetMacro(ClassKey, std::string);

  /** Identifier for the negative samples class */
  itkGetConstMacro(NoClassIdentifier, int);
  itkSetMacro(NoClassIdentifier, int);

  /** The density of auto-generated negative samples inside the polygons */
  itkGetConstMacro(RandomLocalizationDensity, double);
  itkSetMacro(RandomLocalizationDensity, double);

  /** The minimum distance between a generated negative sample and positive samples */
  itkGetConstMacro(InhibitionRadius, double);
  itkSetMacro(InhibitionRadius, double);

  /** The maximum iteration number during negative sample positions */
  itkGetConstMacro(NbMaxIteration, unsigned long int);
  itkSetMacro(NbMaxIteration, unsigned long int);

  /** Set the number of samples position to generate inside the inhibition radius window around a positive point */
  itkGetConstMacro(NumberOfPositiveSamplesPerPoint, unsigned int);
  itkSetMacro(NumberOfPositiveSamplesPerPoint, unsigned int);

  /** Set the seed for random number generator */
  void SetSeed(unsigned int seed)
  {
    m_RandomGenerator->SetSeed(seed);
  }

protected:
  LabeledSampleLocalizationGenerator();
  virtual ~LabeledSampleLocalizationGenerator() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Triggers the Computation of the sample list */
  void GenerateData(void);

  PointVectorType RandomPointsGenerator(DataNodeType * node);

  PointVectorType PointDensification(DataNodeType * node);

private:
  LabeledSampleLocalizationGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  std::string GetNextID()
  {
    std::ostringstream oss;
    oss << m_CurrentID++;
    return oss.str();
  }

  RandomGeneratorType::Pointer m_RandomGenerator;

  std::string               m_ClassKey;
  int                       m_NoClassIdentifier;
  double                    m_RandomLocalizationDensity;
  double                    m_InhibitionRadius;
  unsigned long int         m_NbMaxIteration;
  unsigned int              m_NumberOfPositiveSamplesPerPoint;
  unsigned int              m_CurrentID;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabeledSampleLocalizationGenerator.txx"
#endif

#endif
