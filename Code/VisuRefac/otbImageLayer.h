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
#include "itkHistogram.h"
#include "itkDenseFrequencyContainer.h"
#include "otbObjectList.h"

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
  typedef TImage                                        ImageType;
  typedef typename ImageType::Pointer                   ImagePointerType;
  typedef typename ImageType::PixelType                 PixelType;
  typedef typename ImageType::InternalPixelType         InternalPixelType;
  
  /** Histogram typedef */
  typedef itk::Statistics::DenseFrequencyContainer      DFContainerType;
  typedef itk::Statistics::Histogram
  <InternalPixelType,1,DFContainerType >                HistogramType;
  typedef typename HistogramType::Pointer               HistogramPointerType;
  
  /** Histogram list typedef */
  typedef otb::ObjectList<HistogramType>                HistogramListType;
  typedef typename HistogramListType::Pointer           HistogramListPointerType; 

  /** Set/Get the image */
  itkSetObjectMacro(Image,ImageType);
  itkGetObjectMacro(Image,ImageType);

  /** Set/Get the quicklook */
  itkSetObjectMacro(Quicklook,ImageType);
  itkGetObjectMacro(Quicklook,ImageType);

  /** Set/Get the histogram list */
  itkSetObjectMacro(HistogramList,HistogramListType);
  itkGetObjectMacro(HistogramList,HistogramListType);

  /** Set/Get the upper clamp values */
  itkSetMacro(UpperClamp,PixelType);
  itkGetMacro(UpperClamp,PixelType);

  /** Set/Get the lower clamp values */
  itkSetMacro(LowerClamp,PixelType);
  itkGetMacro(LowerClamp,PixelType);

  /** Actually render the image */
  virtual void Render(){}

protected:
  /** Constructor */
  ImageLayer();
  /** Destructor */
  ~ImageLayer();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
 
private:
  ImageLayer(const Self&);     // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** Pointer to the quicklook */
  ImagePointerType         m_Quicklook;
  
  /** Pointer to the image */
  ImagePointerType         m_Image;

  /** Histogram list */
  HistogramListPointerType m_HistogramList;

  /** Upper clamping values */
  PixelType                m_UpperClamp;

  /** Lower clamping values */
  PixelType                m_LowerClamp;

}; // end class 
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageLayer.txx"
#endif

#endif


