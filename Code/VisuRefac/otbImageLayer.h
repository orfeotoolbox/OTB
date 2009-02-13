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

#include "itkObject.h"
#include "itkHistogram.h"
#include "itkDenseFrequencyContainer.h"
#include "otbObjectList.h"

//#include "otbRenderingFunction.h"
//#include "otbBlendingFunction.h"

namespace otb
{
/** \class ImageLayer
*   \brief This class is a layer container.
*   It contains everything related to a layer in the viewer model.
*   
*   \sa ImageViewerModel
*/

template <class TImage> 
class ImageLayer
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ImageLayer                        Self;
  typedef itk::Object                       Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(ImageLayer,Object);

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

  /** Normalization function */
  //typedef otb::NormalizationFunction<InternalPixelType> NormalizationFunctionType;
  
  /** Set/Get the image */
  itkSetObjectMacro(Image,ImageType);
  itkGetObjectMacro(Image,ImageType);

  /** Set/Get the quicklook */
  itkSetObjectMacro(Quicklook,ImageType);
  itkGetObjectMacro(Quicklook,ImageType);

  /** Set/Get the histogram list */
  itkSetObjectMacro(HistogramList,HistogramListType);
  itkGetObjectMacro(HistogramList,HistogramListType);

  /** Set/Get the layer name */
  itkSetStringMacro(LayerName);
  itkGetStringMacro(LayerName);

  /** Set/Get the upper clamp values */
  itkSetMacro(UpperClamp,PixelType);
  itkGetMacro(UpperClamp,PixelType);

  /** Set/Get the lower clamp values */
  itkSetMacro(LowerClamp,PixelType);
  itkGetMacro(LowerClamp,PixelType);

  /** Set/Get the red channel index */
  itkSetMacro(RedChannelIndex,unsigned int);
  itkGetMacro(RedChannelIndex,unsigned int);

  /** Set/Get the green channel index */
  itkSetMacro(GreenChannelIndex,unsigned int);
  itkGetMacro(GreenChannelIndex,unsigned int);

  /** Set/Get the blue channel index */
  itkSetMacro(BlueChannelIndex,unsigned int);
  itkGetMacro(BlueChannelIndex,unsigned int);

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

  /** Layer name */
  std::string              m_LayerName;

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

  /** Red channel index */
  unsigned int             m_RedChannelIndex;
  
  /** Green channel index */
  unsigned int             m_GreenChannelIndex;

  /** Blue channel index */
  unsigned int             m_BlueChannelIndex;

}; // end class 
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageLayer.txx"
#endif

#endif


