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
#ifndef __otbSOMClassifier_h
#define __otbSOMClassifier_h

#include "itkProcessObject.h"
#include "itkMembershipSample.h"

namespace otb{ 

/** \class SOMClassifier 
 *  \brief SOM-based classifier
 */

template< class TSample, class TSOMMap, class TLabel>
class ITK_EXPORT SOMClassifier : 
      public itk::ProcessObject
{
public:
  /** Standard class typedef*/
  typedef SOMClassifier Self;
  typedef itk::ProcessObject Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

 /** Standard macros */
  itkTypeMacro(SOMClassifier, itk::ProcessObject);
  itkNewMacro(Self) ;

  /** Output type for GetClassSample method */
  typedef itk::Statistics::MembershipSample< TSample > OutputType ;

  /** typedefs from TSample object */
  typedef TSample SampleType;
  typedef typename SampleType::Pointer SamplePointerType;
  typedef typename SampleType::MeasurementType MeasurementType ;
  typedef typename SampleType::MeasurementVectorType MeasurementVectorType ;
  typedef typename SampleType::MeasurementVectorType::ValueType InputPixelType ;

  typedef TSOMMap SOMMapType;
  typedef typename SOMMapType::Pointer SOMMapPointerType;

  typedef itk::Statistics::MembershipSample<SampleType> OutputType;
  typedef typename OutputType::Pointer OutputPointerType;
  
  typedef TLabel ClassLabelType ;

  void SetMap(SOMMapType * sommap);
  SOMMapType * GetMap(void);
  itkSetObjectMacro(Sample,SampleType);
  itkGetObjectMacro(Sample,SampleType);
  itkSetObjectMacro(Output,OutputType);
  itkGetObjectMacro(Output,OutputType);

protected:
  SOMClassifier() ;
  virtual ~SOMClassifier() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Starts the classification process */
  void GenerateData() ;

 private:
  
  SamplePointerType m_Sample;
  OutputPointerType m_Output;
 
} ; // end of class
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOMClassifier.txx"
#endif

#endif







