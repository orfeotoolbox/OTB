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

#ifndef __otbListSampleGenerator_h
#define __otbListSampleGenerator_h

#include "itkProcessObject.h"
#include "itkListSample.h"
#include "itkPreOrderTreeIterator.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

namespace otb
{
/** \class ListSampleGenerator
 *  \brief Produces a ListSample from a VectorImage and a VectorData
 *
 *  This filter produces two ListSample for learning and validation of
 *  learning algorithms. The repartition between the learning and validation
 *  ListSample can be adjusted using the SetValidationTrainingProportion()
 *  method.
 *
 *  The size of the training and validation ListSample can be limited using the
 *  SetMaxTrainingSize() and SetMaxValidationSize() methods.
 *
 *  Classes are specified by the VectorData with a metadata identified by
 *  a specific key. This key can be provided by the SetClassKey() method
 *  (using "Class" as a default key).
 *
 *  The input VectorData is supposed to be fully contained within the image extent
 *
 */
template <class TImage, class TVectorData>
class ITK_EXPORT ListSampleGenerator :
  public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef ListSampleGenerator           Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ListSampleGenerator, itk::ProcessObject);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef TImage                           ImageType;
  typedef typename ImageType::Pointer      ImagePointerType;
  typedef typename ImageType::IndexType    ImageIndexType;
  typedef typename ImageType::RegionType   ImageRegionType;
  typedef TVectorData                      VectorDataType;
  typedef typename VectorDataType::Pointer VectorDataPointerType;
  
  /** List to store the pixel values */
  typedef typename ImageType::PixelType           SampleType;
  typedef itk::Statistics::ListSample<SampleType> ListSampleType;
  typedef typename ListSampleType::Pointer        ListSamplePointerType;

  /** List to store the corresponding labels */
  typedef int                                    ClassLabelType;
  typedef itk::FixedArray<ClassLabelType, 1>     LabelType;  //note could be templated by an std:::string
  typedef itk::Statistics::ListSample<LabelType> ListLabelType;
  typedef typename ListLabelType::Pointer        ListLabelPointerType;

  /** Connects the input image for which the sample list is going to be extracted */
  void SetInput(const ImageType *);
  const ImageType * GetInput() const;

  /** Connects the vector data for which the sample list is going to be extracted
   * if this is the only input vector data, both the training and validation samples
   * come from it */
  void SetInputVectorData(const VectorDataType *);
  const VectorDataType * GetInputVectorData() const;

  // Build the outputs
  typedef itk::DataObject::Pointer DataObjectPointer;
  virtual DataObjectPointer MakeOutput(unsigned int idx);
  //virtual void Update();

  /** Accessors */
  itkGetConstMacro(MaxTrainingSize, long int);
  itkSetMacro(MaxTrainingSize, long int);
  itkGetConstMacro(PolygonEdgeInclusion, bool);
  itkSetMacro(PolygonEdgeInclusion, bool);
  itkGetConstMacro(MaxValidationSize, long int);
  itkSetMacro(MaxValidationSize, long int);
  itkGetConstMacro(ValidationTrainingProportion, double);
  itkSetClampMacro(ValidationTrainingProportion, double, 0.0, 1.0);

  itkGetConstMacro(NumberOfClasses, unsigned short);
  typedef std::map<ClassLabelType, int> SampleNumberType;

  SampleNumberType GetClassesSamplesNumberTraining(void) const
  {
    return m_ClassesSamplesNumberTraining;
  }

  SampleNumberType GetClassesSamplesNumberValidation(void) const
  {
    return m_ClassesSamplesNumberValidation;
  }

  itkGetStringMacro(ClassKey);
  itkSetStringMacro(ClassKey);
  itkGetConstMacro(ClassMinSize, double);
  
  /** Returns the Training ListSample as a data object */
  ListSampleType * GetTrainingListSample();

  /** Returns the Trainingn label ListSample as a data object */
  ListLabelType * GetTrainingListLabel();

  /** Returns the label sample list as a data object */
  ListSampleType * GetValidationListSample();

  /** Returns the label sample list as a data object */
  ListLabelType * GetValidationListLabel();
  
  // Get the map size
  std::map<ClassLabelType, double> GetClassesSize() const
  {
    return m_ClassesSize;
  }
  
protected:
  ListSampleGenerator();
  virtual ~ListSampleGenerator() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Triggers the Computation of the sample list */
  void GenerateData(void);
   
  /** Compute the calss statistics*/
  void GenerateClassStatistics();

private:
  ListSampleGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typedef typename VectorDataType::DataNodeType         DataNodeType;
  typedef typename DataNodeType::PolygonType            PolygonType;
  typedef typename DataNodeType::PolygonPointerType     PolygonPointerType;
  typedef typename DataNodeType::PolygonListType        PolygonListType;
  typedef typename DataNodeType::PolygonListPointerType PolygonListPointerType;
  typedef itk::PreOrderTreeIterator<typename VectorDataType::DataTreeType> TreeIteratorType;
  
  void ComputeClassSelectionProbability();
  
  // Crop the polygon wrt the image largest region,
  // and return the resulting size in pixel units
  // This does not handle interior rings
  double GetPolygonAreaInPixelsUnits(DataNodeType* polygonDataNode, ImageType* image);

  long int m_MaxTrainingSize; // number of training samples (-1 = no limit)
  long int m_MaxValidationSize; // number of validation samples (-1 = no limit)
  double   m_ValidationTrainingProportion; // proportion of training vs validation
                                           // (0.0 = all training, 1.0 = all validation)

  bool           m_PolygonEdgeInclusion; // if true take into consideration pixel which are on polygon edge
                                           //  useful, when dealing with small polygon area (1 or two pixels)
                                           // false by default
  unsigned short m_NumberOfClasses;
  std::string    m_ClassKey;
  double         m_ClassMinSize;
  
  std::map<ClassLabelType, double> m_ClassesSize;
  std::map<ClassLabelType, double> m_ClassesProbTraining;
  std::map<ClassLabelType, double> m_ClassesProbValidation;

  std::map<ClassLabelType, int> m_ClassesSamplesNumberTraining; //Just a counter
  std::map<ClassLabelType, int> m_ClassesSamplesNumberValidation; //Just a counter

  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;
  RandomGeneratorType::Pointer m_RandomGenerator;
};
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbListSampleGenerator.txx"
#endif

#endif
