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
#ifndef __otbImageLayer_h
#define __otbImageLayer_h

#include "otbLayer.h"
#include "itkVariableLengthVector.h"
#include "itkDenseFrequencyContainer.h"
#include "otbRenderingImageFilter.h"
#include "itkExtractImageFilter.h"
#include "itkListSample.h"
#include "otbListSampleToVariableDimensionHistogramGenerator.h"
#include "otbStandardRenderingFunction.h"

namespace otb
{
/** \class ImageLayer
*   \brief This class is a layer container.
*   It contains everything related to a layer in the viewer model.
*   
*   \sa ImageViewerModel
*   \sa Layer
*/

template <class TImage, class TOutputImage = otb::Image<itk::RGBPixel<unsigned char>, 2 > >  
class ImageLayer
  : public Layer<TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ImageLayer                        Self;
  typedef Layer<TOutputImage>               Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(ImageLayer,Layer);

  /** Image typedef */
  typedef TImage                                              ImageType;
  typedef typename ImageType::Pointer                         ImagePointerType;
  typedef typename ImageType::PixelType                       PixelType;
  typedef typename ImageType::InternalPixelType               InternalPixelType;
  
  /** Output image typedef */
  typedef TOutputImage                                        OutputImageType;

  /** Histogram typedef */
   typedef itk::Statistics::DenseFrequencyContainer            DFContainerType;
  
  typedef itk::VariableLengthVector<InternalPixelType>        SampleType;
  typedef itk::Statistics::ListSample<SampleType>             ListSampleType;
 
  typedef otb::ListSampleToVariableDimensionHistogramGenerator
  <ListSampleType,InternalPixelType,DFContainerType>          HistogramFilterType;
  typedef typename HistogramFilterType::HistogramType         HistogramType;
  typedef typename HistogramType::Pointer                     HistogramPointerType;

  
  /** Rendering part */
  typedef RenderingImageFilter<TImage,TOutputImage>                    RenderingFilterType;
  typedef typename RenderingFilterType::RenderingFunctionType          RenderingFunctionType;
  typedef typename RenderingFunctionType::Pointer                      RenderingFunctionPointerType;
  typedef Function::StandardRenderingFunction<InternalPixelType,
				    typename TOutputImage::PixelType>  DefaultRenderingFunctionType;
  typedef itk::ExtractImageFilter<ImageType,ImageType>                 ExtractFilterType;

  /** Set/Get the image */
  itkSetObjectMacro(Image,ImageType);
  itkGetObjectMacro(Image,ImageType);

  /** Set/Get the quicklook */
  itkSetObjectMacro(Quicklook,ImageType);
  itkGetObjectMacro(Quicklook,ImageType);

  /** Set/Get the histogram list */
  itkSetObjectMacro(Histogram,HistogramType);
  itkGetObjectMacro(Histogram,HistogramType);

  /** Set/Get the rendering function */
  itkSetObjectMacro(RenderingFunction,RenderingFunctionType);
  itkGetObjectMacro(RenderingFunction,RenderingFunctionType);

  /** Set/Get the number of bins for histogram generation */
  itkSetMacro(NumberOfHistogramBins,unsigned int);
  itkGetMacro(NumberOfHistogramBins,unsigned int);

  /** Actually render the image */
  virtual void Render();

protected:
  /** Constructor */
  ImageLayer();
  /** Destructor */
  ~ImageLayer();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
 
  /** Update the histogram */
  virtual void RenderHistogram();

  /** Update the images */
  virtual void RenderImages();

private:
  ImageLayer(const Self&);     // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** Pointer to the quicklook */
  ImagePointerType             m_Quicklook;
  
  /** Pointer to the image */
  ImagePointerType             m_Image;

  /** Joint Histogram */
  HistogramPointerType         m_Histogram;

  /** Rendering function */
  RenderingFunctionPointerType m_RenderingFunction;
  
  /** Number of bins for histograms generation */
  unsigned int                 m_NumberOfHistogramBins;

}; // end class 
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageLayer.txx"
#endif

#endif
