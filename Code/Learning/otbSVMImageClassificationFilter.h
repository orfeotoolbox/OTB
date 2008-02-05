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
#ifndef _otbImageClassificationFilter_h
#define _otbImageClassificationFilter_h

#include "otbSVMClassifier.h"
#include "itkInPlaceImageFilter.h"
#include "itkFixedArray.h"
#include "itkListSample.h"
#include "otbSVMClassifier.h"

namespace otb
{
  /** \class ImageClassificationFilter
   *  \brief 
   *
   * \ingroup 
   * \ingroup
   */
template <class TInputImage, class TOutputImage, unsigned int VMaxSampleDimension = 10>
  class ITK_EXPORT SVMImageClassificationFilter
  : public itk::InPlaceImageFilter<TInputImage,TOutputImage>
  {
    public:
    /** Standard typedefs */
    typedef SVMImageClassificationFilter                      Self;
    typedef itk::InPlaceImageFilter<TInputImage,TOutputImage> Superclass;
    typedef itk::SmartPointer<Self>                           Pointer;
    typedef itk::SmartPointer<const Self>                     ConstPointer;
    
    /** Type macro */
    itkNewMacro(Self);
    
    /** Creation through object factory macro */
    itkTypeMacro(SVMImageClassificationFilter,InPlaceImageFilter);

    /** The max dimension of the sample to classify.
     *  This filter internally uses itk::FixedArray as input for the classifier,
     *  so the max sample size has to be fixed at compilation time.
     */
    itkStaticConstMacro(MaxSampleDimension,unsigned int,VMaxSampleDimension);

    typedef TInputImage                                InputImageType;
    typedef typename InputImageType::ConstPointer      InputImageConstPointerType;
    typedef typename InputImageType::InternalPixelType ValueType;

    typedef TOutputImage                               OutputImageType;
    typedef typename OutputImageType::Pointer          OutputImagePointerType;
    typedef typename OutputImageType::RegionType       OutputImageRegionType;
    typedef typename OutputImageType::PixelType        LabelType;

    typedef itk::FixedArray<ValueType,VMaxSampleDimension>     MeasurementVectorType;
    typedef itk::Statistics::ListSample<MeasurementVectorType> ListSampleType;
    typedef typename ListSampleType::Pointer                   ListSamplePointerType;
    typedef otb::SVMClassifier<ListSampleType, LabelType>      ClassifierType;
    typedef typename ClassifierType::Pointer                   ClassifierPointerType;
    typedef SVMModel< ValueType, LabelType >                   ModelType;
    typedef typename ModelType::Pointer                        ModelPointerType;
    typedef std::vector<ClassifierPointerType>                 ClassifierPointerVectorType;
    
    /** Set/Get the svm model */
    itkSetObjectMacro(Model,ModelType);
    itkGetObjectMacro(Model,ModelType);

    protected:
    /** Constructor */
    SVMImageClassificationFilter();
    /** Destructor */
    virtual ~SVMImageClassificationFilter() {};

    /** Threaded generate data */
    virtual void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread,int threadId);
    /** Before threaded generate data */
    virtual void BeforeThreadedGenerateData();
    /**PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
    
    private:
    SVMImageClassificationFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    /** The SVM model used for classification */
    ModelPointerType m_Model;
    /** Vector of classifier (one classifier per thread) */
    ClassifierPointerVectorType m_ClassifierVector;
  };
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMImageClassificationFilter.txx"
#endif

#endif
