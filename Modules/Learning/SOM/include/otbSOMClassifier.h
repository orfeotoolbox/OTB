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

namespace otb
{

/** \class SOMClassifier
 *  \brief This class implements a SOM-Based classifier.
 *
 * The classifier iterates on the input list sample, feeding
 * the output membership sample with the one-dimensionnal index
 * of the winner neuron.
 *
 * Since this classifier differs from the base framework of itk in that it
 * does not use DecisionRule and Memberships function, it derives from itk::ProcessObject
 * instead of itk::SampleClassifierBase.
 *
 * \sa SOM, SOMMap, SOMActivationBuilder.
 *
 * \ingroup OTBSOM
 */

template<class TSample, class TSOMMap, class TLabel>
class ITK_EXPORT SOMClassifier :
  public itk::ProcessObject
{
public:
  /** Standard class typedef*/
  typedef SOMClassifier                 Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macros */
  itkTypeMacro(SOMClassifier, itk::ProcessObject);
  itkNewMacro(Self);

  /** typedefs from TSample object */
  typedef TSample                                               SampleType;
  typedef typename SampleType::Pointer                          SamplePointerType;
  typedef typename SampleType::MeasurementType                  MeasurementType;
  typedef typename SampleType::MeasurementVectorType            MeasurementVectorType;
  typedef typename SampleType::MeasurementVectorType::ValueType InputPixelType;

  /** SOM Map typedefs */
  typedef TSOMMap                      SOMMapType;
  typedef typename SOMMapType::Pointer SOMMapPointerType;

  /** Output typedefs */
  typedef itk::Statistics::MembershipSample<SampleType> OutputType;
  typedef typename OutputType::Pointer                  OutputPointerType;

  /** Label type typedef */
  typedef TLabel ClassLabelType;

  /// Accessors
  void SetMap(SOMMapType * sommap);
  SOMMapType * GetMap(void);
  itkSetObjectMacro(Sample, SampleType);
  itkGetObjectMacro(Sample, SampleType);
  
  OutputType * GetOutput();

protected:
  /** Constructor */
  SOMClassifier();
  /** Destructor */
  virtual ~SOMClassifier() {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Starts the classification process */
  void GenerateData();

private:
  /// The input sample
  SamplePointerType m_Sample;

}; // end of class
} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSOMClassifier.txx"
#endif

#endif
