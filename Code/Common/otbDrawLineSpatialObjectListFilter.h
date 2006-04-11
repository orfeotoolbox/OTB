/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   06 avril 2006
  Role      :   Draw Line Spatial Object List
  $Id : $ 

=========================================================================*/
#ifndef __otbDrawLineSpatialObjectListFilter_h
#define __otbDrawLineSpatialObjectListFilter_h

#include "itkImageToImageFilter.h"
#include "itkLineSpatialObject.h"

#include "otbDrawLineSpatialObjectFilter.h"

#include <list>

namespace otb
{

/** \class DrawLineSpatialObjectListFilter
 * \brief Composite filter which draw lines in a binary image.
 *
 * This class implements a composite filter that draws a list of line in 
 * a binary image by using the otb::DrawLineSpatialObjectFilter that
 * draws each line of the list.
 *  
 *  
 */

template <class TInputImage, class TOutputImage>
class  ITK_EXPORT DrawLineSpatialObjectListFilter : 
public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** 	Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);
  itkStaticConstMacro(		OutputImageDimension, 
  				unsigned int,
                      		TOutputImage::ImageDimension);


  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  

  /** typedef for the classes standards. */
  typedef DrawLineSpatialObjectListFilter Self;
  typedef itk::ImageToImageFilter< TInputImage, TOutputImage > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  typedef itk::LineSpatialObject<2>	LineType;
  typedef typename LineType::Pointer	LinePointer;
  typedef std::list< LinePointer >	LinesListType;
  
  typedef typename LinesListType::const_iterator LineListIterator;
  
  typedef otb::DrawLineSpatialObjectFilter< OutputImageType, OutputImageType > DrawLineType; 

  /** Method for management of the "object factory". */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(DrawLineSpatialObjectListFilter, ImageToImageFilter);
  
  /** Definition of the input and output images */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** Definition of the size of the images. */
  typedef typename InputImageType::SizeType SizeType;
  
  /** Set/Get the image input of this process object. */
  virtual void SetInputLineSpatialObjectList(const LinesListType & list);
  LinesListType & GetInputLineSpatialObjectList(void);
  
  
protected:
  DrawLineSpatialObjectListFilter();
  virtual ~DrawLineSpatialObjectListFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateData();

private:
  DrawLineSpatialObjectListFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  LinesListType				m_InputList;
  typename DrawLineType::Pointer	m_DrawLineFilter;


};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDrawLineSpatialObjectListFilter.txx"
#endif

  
#endif
