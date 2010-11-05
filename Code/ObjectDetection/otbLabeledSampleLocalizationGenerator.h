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
#include "itkEuclideanDistance.h"


namespace otb
{
/** \class LabeledSampleLocalizationGenerator
 *  \brief Produces a VectorData from potentialy multiple VectorData
 *
 *  This generator produces a unique vector data containing labeled positions
 *  extracted from inputs. 
 *
 *  Input points are transmited to the output. In addition, 'no class'
 *  points are randomly picked inside input polygons making sure
 *  they are at least at a given distance (InhibitionRadius) of every
 *  known points.
 *  
 *  Classes are specified by the VectorData with a metadata identified by
 *  a specific key. This key can be provided by the SetClassKey() method
 *  (using "Class" as a default key).
 *  Features associated with 
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
  typedef itk::Statistics::EuclideanDistance<PointType>          EuclideanDistanceType;
  
  /** Connects the VectorDatas from which the localizations are going to be extracted. */
  void PushBackInput(const VectorDataType *);
  const VectorDataType * GetInput(unsigned int idx) const;

  virtual void Update();

  /** Accessors */
  itkGetConstMacro(ClassKey, std::string);
  itkSetMacro(ClassKey, std::string);
  itkGetConstMacro(NoClassIdentifier, std::string);
  itkSetMacro(NoClassIdentifier, std::string);
  itkGetConstMacro(RandomLocalizationDensity, double);
  itkSetMacro(RandomLocalizationDensity, double);
  itkGetConstMacro(InhibitionRadius, double);
  itkSetMacro(InhibitionRadius, double);
  itkGetConstMacro(PseudoRandom, bool);
  itkSetMacro(PseudoRandom, bool);
  
protected:
  LabeledSampleLocalizationGenerator();
  virtual ~LabeledSampleLocalizationGenerator() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Triggers the Computation of the sample list */
  void GenerateData(void);
  /** Create a dictionary containing all differente values of ClassKey
    * field */
  void ClassKeyValuesCollector();
  PointVectorType RandomPointsGenerator(DataNodeType * node);
  
private:
  LabeledSampleLocalizationGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typename VectorDataType::Pointer m_OutputVectorData;

  RandomGeneratorType::Pointer m_RandomGenerator;

  std::string               m_ClassKey;
  std::vector<std::string>  m_ClassKeyDictionary;
  std::string               m_NoClassIdentifier;
  double                    m_RandomLocalizationDensity;
  double                    m_InhibitionRadius;

  bool                      m_PseudoRandom;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabeledSampleLocalizationGenerator.txx"
#endif

#endif
