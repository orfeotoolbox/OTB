/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   11 avril 2006
  Version   :   
  Role      :   Image to Line Spatial Object List Filter
  $Id$

=========================================================================*/
#ifndef _otbImageToLineSpatialObjectListFilter_h
#define _otbImageToLineSpatialObjectListFilter_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkProcessObject.h"
#include "otbLineSpatialObjectList.h"
#include "itkLineSpatialObject.h"

namespace otb
{
/** \class ImageToLineSpatialObjectListFilter
 *  \brief Base class for all process objects that output LineSpatialObjectList.
 *
 * ImageToLineSpatialObjectListFilter is the base class for all process objects 
 * that output list of LineSpatialObject. 
 *
 */
 
 
template <class TInputImage>
class ITK_EXPORT ImageToLineSpatialObjectListFilter : public itk::ProcessObject
{
public:

  /** 	Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);


  typedef TInputImage InputImageType;
  
  
  /** Standard class typedefs. */
  typedef ImageToLineSpatialObjectListFilter             Self;
  typedef itk::ProcessObject                            Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToLineSpatialObjectListFilter,itk::ProcessObject);

  /** Some convenient typedefs. */
  typedef LineSpatialObjectList                           LinesListType;
  typedef typename LinesListType::LineType                  	  LineType;

  /** Definition of the input and output images */
  typedef typename InputImageType::PixelType InputPixelType;
  

  /** Definition of the size of the images. */
  typedef typename InputImageType::SizeType SizeType;

 
  /** Set/Get the input image */
  void SetInput(const InputImageType *image);
  const InputImageType * GetInput(void);

  /** Set/Get the list of LineSpatialObject of this process object.  */
  void SetOutput(const LinesListType *list);
  LinesListType * GetOutput(void); 


  
protected:
  ImageToLineSpatialObjectListFilter();
  virtual ~ImageToLineSpatialObjectListFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  

private:
  ImageToLineSpatialObjectListFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

 

};
  

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToLineSpatialObjectListFilter.txx"
#endif

#endif
