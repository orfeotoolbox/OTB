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
#ifndef __otbHorizontalPixelWiseBlockMatchingImageFilter_h
#define __otbHorizontalPixelWiseBlockMatchingImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbImage.h"

namespace otb
{

namespace Functor
{
template <class TInputImage, class TOutputMetricImage>
ITK_EXPORT class SSDBlockMatching
{
public:
  typedef itk::ConstNeighborhoodIterator<TInputImage> ConstNeigghborhoodIteratorType;
  typedef typename TOutputMetricImage::ValueType      MetricValueType;

  // Implement the SSD operator
  inline MetricValueType operator()(ConstNeigghborhoodIteratorType & a, ConstNeigghborhoodIteratorType & b) const
  {
    typename ConstNeigghborhoodIteratorType::Iterator itA, itB;

    MetricValueType ssd = 0;
    
    // For some reason, iterators do not work on neighborhoods
    for(unsigned int i = 0; i<a.Size(); ++i)
      {
      ssd += (a.GetPixel(i)-b.GetPixel(i))*(a.GetPixel(i)-b.GetPixel(i));
      }

    return ssd;
  }
};

}

template <class TInputImage, class TOutputMetricImage, class TOutputDisparityImage = TOutputMetricImage, class TMaskImage = otb::Image<unsigned char>, 
          class TBlockMatchingFunctor = Functor::SSDBlockMatching<TInputImage,TOutputMetricImage> >
class ITK_EXPORT HorizontalPixelWiseBlockMatchingImageFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputDisparityImage>
{
public:
  /** Standard class typedef */
  typedef HorizontalPixelWiseBlockMatchingImageFilter       Self;
  typedef itk::ImageToImageFilter<TInputImage,
                                  TOutputDisparityImage>    Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(HorizontalPixelWiseBlockMatchingImageFilter, ImageToImageFilter);

  /** Usefull typedefs */
  typedef TInputImage                                       InputImageType;
  typedef TOutputMetricImage                                OutputMetricImageType;
  typedef TOutputDisparityImage                             OutputDisparityImageType;
  typedef TMaskImage                                        InputMaskImageType;
  typedef TBlockMatchingFunctor                             BlockMatchingFunctorType;

  typedef typename InputImageType::SizeType                 SizeType;
  typedef typename InputImageType::IndexType                IndexType;
  typedef typename InputImageType::RegionType               RegionType;

  /** Set left input */
  void SetLeftInput( const TInputImage * image);

  /** Set right input */
  void SetRightInput( const TInputImage * image);

  /** Set mask input (optional) */
  void SetMaskInput(const TMaskImage * image);

  /** Get the inputs */
  const TInputImage * GetLeftInput() const;
  const TInputImage * GetRightInput() const;
  const TMaskImage  * GetMaskInput() const;

  /** Get the metric output */
  const TOutputMetricImage * GetMetricOutput() const;
  TOutputMetricImage * GetMetricOutput();

  /** Get the disparity output */
  const TOutputDisparityImage * GetDisparityOutput() const;
  TOutputDisparityImage * GetDisparityOutput();

  /** Set unsigned int radius */
  void SetRadius(unsigned int radius)
  {
    m_Radius.Fill(radius);
  }

  /** Set/Get the radius of the area on which metric is evaluated */
  itkSetMacro(Radius, SizeType);
  itkGetConstReferenceMacro(Radius, SizeType);
  
  /*** Set/Get the minimum disparity to explore */
  itkSetMacro(MinimumDisparity,int);
  itkGetConstReferenceMacro(MinimumDisparity,int);

  /*** Set/Get the maximum disparity to explore */
  itkSetMacro(MaximumDisparity,int);
  itkGetConstReferenceMacro(MaximumDisparity,int);

  /** True if metric should be minimized. False otherwise */
  itkSetMacro(Minimize, bool);
  itkGetConstReferenceMacro(Minimize,bool);
  itkBooleanMacro(Minimize);

protected:
  /** Constructor */
  HorizontalPixelWiseBlockMatchingImageFilter();

  /** Destructor */
  virtual ~HorizontalPixelWiseBlockMatchingImageFilter();

  /** Generate input requrested region */
  virtual void GenerateInputRequestedRegion();

  /** Before threaded generate data */
  virtual void BeforeThreadedGenerateData();
  
  /** Threaded generate data */
  virtual void ThreadedGenerateData(const RegionType & outputRegionForThread, int threadId);
  
private:
  HorizontalPixelWiseBlockMatchingImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemeFnted

   /** The radius of the blocks */
  SizeType                      m_Radius;

  /** The min disparity to explore */
  int                           m_MinimumDisparity;

  /** The max disparity to explore */
  int                           m_MaximumDisparity;

  /** Should we minimize or maximize ? */
  bool                          m_Minimize;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHorizontalPixelWiseBlockMatchingImageFilter.txx"
#endif

#endif
