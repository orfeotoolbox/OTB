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

namespace otb
{
/** \class ListSampleGenerator
 *  \brief TODO
 *
 */
template < class TImage, class TVectorData > 
class ITK_EXPORT ListSampleGenerator :
  public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef ListSampleGenerator           Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer< Self >     Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ListSampleGenerator, itk::ProcessObject);
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  typedef TImage      ImageType;
  typedef TVectorData VectorDataType;
  
  /** List to store the pixel values */
  typedef typename ImageType::PixelType           SampleType;
  typedef itk::Statistics::ListSample<SampleType> ListSampleType;
  typedef typename ListSampleType::Pointer        ListSamplePointerType;
  
  /** List to store the corresponding labels */
  typedef itk::FixedArray<int, 1>                 LabelType; //note could be templated by an std:::string
  typedef itk::Statistics::ListSample<LabelType>  ListLabelType;
  typedef typename ListLabelType::Pointer         ListLabelPointerType;
  
  /** Connects the input image for which the sample list is going to be extracted */
  void SetInput( const ImageType * );
  const ImageType * GetInput() const;
  
  /** Connects the vector data for which the sample list is going to be extracted */
  void SetInputVectorData( const VectorDataType * );
  const VectorDataType * GetInputVectorData() const;
  
// Switch to the classic interface once OTB use the new stat framework?
// ListSample are a full DataObject
//  typedef itk::DataObject::Pointer DataObjectPointer;
//  virtual DataObjectPointer MakeOutput(unsigned int idx);
  virtual void Update();
  
  /** Accessors */
  itkGetMacro(MaxTrainingSize, int);
  itkSetMacro(MaxTrainingSize, int);
  itkGetMacro(MaxValidationSize, int);
  itkSetMacro(MaxValidationSize, int);
  itkGetMacro(ValidationTrainingRatio, double);
  itkSetMacro(ValidationTrainingRatio, double);
  
  itkGetObjectMacro(TrainingListSample, ListSampleType);
  itkGetObjectMacro(TrainingListLabel, ListLabelType);
  itkGetObjectMacro(ValidationListSample, ListSampleType);
  itkGetObjectMacro(ValidationListLabel, ListLabelType);
  
protected:
  ListSampleGenerator();
  virtual ~ListSampleGenerator() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;  
  
  /** Triggers the Computation of the sample list */
  void GenerateData( void );
  
private:
  ListSampleGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  int    m_MaxTrainingSize;
  int    m_MaxValidationSize;
  double m_ValidationTrainingRatio;
  
  ListSamplePointerType m_TrainingListSample;
  ListLabelPointerType  m_TrainingListLabel;
  ListSamplePointerType m_ValidationListSample;
  ListLabelPointerType  m_ValidationListLabel;
  
};
}// end of namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbListSampleGenerator.txx"
#endif

#endif
