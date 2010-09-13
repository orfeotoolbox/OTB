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
#ifndef __otbLabelMapToSampleListFilter_h
#define __otbLabelMapToSampleListFilter_h

#include <algorithm>

#include "itkObject.h"
#include "otbAttributesMapLabelObject.h"

namespace otb
{
/** \class LabelMapToSampleListFilter
 * \brief This class converts a LabelObjectMap to a SampleList for
 * learning and classification.
 *
 * The conversion from label object to measurement vector is made
 * through a functor. This allows to support any kind of LabelObject
 * via proper redefinition of the functor.
 */

template <class TInputLabelMap, class TOutputSampleList, 
    class TMeasurementFunctor = Functor::AttributesMapMeasurementFunctor
    <typename TInputLabelMap::LabelObjectType, typename TOutputSampleList::MeasurementVectorType > >
class ITK_EXPORT LabelMapToSampleListFilter : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef LabelMapToSampleListFilter    Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LabelMapToSampleListFilter, Object);

  /** template typedefs */
  typedef TInputLabelMap                              InputLabelMapType;
  typedef typename InputLabelMapType::ConstPointer    InputLabelMapConstPointerType;
  typedef typename InputLabelMapType::LabelObjectType LabelObjectType;
   
  typedef TOutputSampleList                           OutputSampleListType;
  typedef typename OutputSampleListType::Pointer      OutputSampleListPointerType;
  typedef typename OutputSampleListType
  ::MeasurementVectorType                             MeasurementVectorType;
  
  typedef TMeasurementFunctor                         MeasurementFunctorType;

  /** Set/Get the input label map  */
  itkSetConstObjectMacro(InputLabelMap,InputLabelMapType);
  itkGetConstObjectMacro(InputLabelMap,InputLabelMapType);
  itkGetObjectMacro(OutputSampleList,OutputSampleListType);

  /** Get a hook on the functor for settings */
  void SetMeasurementFunctor(const MeasurementFunctorType& functor)
  {
    m_MeasurementFunctor = functor;
  }
  
  MeasurementFunctorType & GetMeasurementFunctor()
  {
    return m_MeasurementFunctor;
  }

  /** Update */
  virtual void Update();

protected:
  LabelMapToSampleListFilter();
  virtual ~LabelMapToSampleListFilter();

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  LabelMapToSampleListFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Input and output pointers */
  InputLabelMapConstPointerType m_InputLabelMap;
  OutputSampleListPointerType   m_OutputSampleList;

  /** The functor used to build the measurement vector */
  MeasurementFunctorType        m_MeasurementFunctor;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapToSampleListFilter.txx"
#endif

#endif
