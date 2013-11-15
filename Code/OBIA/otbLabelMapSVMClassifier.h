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
#ifndef __otbLabelMapSVMClassifier_h
#define __otbLabelMapSVMClassifier_h

#include "itkInPlaceLabelMapFilter.h"
#include "otbSVMModel.h"
#include "itkListSample.h"
#include "otbAttributesMapLabelObject.h"

namespace otb {

/** \class LabelMapSVMClassifier
 * \brief Classify each LabelObject of the input LabelMap in place
 *
 * \sa otb::AttributesMapLabelObject
 * \sa otb::SVMModel
 * \sa itk::InPlaceLabelMapFilter
 */
template<class TInputLabelMap>
class ITK_EXPORT LabelMapSVMClassifier :
    public itk::InPlaceLabelMapFilter<TInputLabelMap>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapSVMClassifier                      Self;
  typedef itk::InPlaceLabelMapFilter<TInputLabelMap> Superclass;
  typedef itk::SmartPointer<Self>                    Pointer;
  typedef itk::SmartPointer<const Self>              ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputLabelMap                         LabelMapType;
  typedef typename LabelMapType::Pointer         LabelMapPointer;
  typedef typename LabelMapType::ConstPointer    LabelMaponstPointer;
  typedef typename LabelMapType::LabelObjectType LabelObjectType;

  typedef typename LabelObjectType::AttributesValueType     AttributesValueType;
  typedef typename LabelObjectType::ClassLabelType          ClassLabelType;
  typedef std::vector<AttributesValueType>                  MeasurementVectorType;

  typedef Functor::AttributesMapMeasurementFunctor
      <LabelObjectType, MeasurementVectorType>              MeasurementFunctorType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputLabelMap::ImageDimension);

  /** Type definitions for the SVM Model. */
  typedef SVMModel<AttributesValueType, ClassLabelType>   SVMModelType;
  typedef typename SVMModelType::Pointer                  SVMModelPointer;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(LabelMapSVMClassifier,
               itk::InPlaceLabelMapFilter);

  itkSetObjectMacro(Model, SVMModelType);

  void SetMeasurementFunctor(const MeasurementFunctorType& functor)
  {
    m_MeasurementFunctor = functor;
  }

  MeasurementFunctorType & GetMeasurementFunctor()
  {
    return m_MeasurementFunctor;
  }

protected:
  LabelMapSVMClassifier();
  ~LabelMapSVMClassifier() {};

  virtual void ThreadedProcessLabelObject( LabelObjectType * labelObject );

  virtual void ReleaseInputs();


private:
  LabelMapSVMClassifier(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The SVM model used for classification */
  SVMModelPointer m_Model;

  /** The functor used to build the measurement vector */
  MeasurementFunctorType m_MeasurementFunctor;

}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelMapSVMClassifier.txx"
#endif

#endif


