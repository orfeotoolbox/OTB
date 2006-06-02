/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   13 fevrier 2006
  Version   :   
  Role      :   Dessine les segmentss de lignes du Path sur une image
  $Id:$

=========================================================================*/
#ifndef __otbDrawPathListFilter_h
#define __otbDrawPathListFilter_h

#include "otbPathListSource.h"
#include "itkImageSource.h"
#include "itkConceptChecking.h"

namespace otb
{
/** \class DrawPathListFilter
 * \brief Classe de base pour inserer dans une image les segments de lignes dans une image. 
 *
 * 
 * \ingroup PathListFilters
 * \ingroup PathLists
 */

template <class TInputImage, class TInputPath,class TOutputImage>
class ITK_EXPORT DrawPathListFilter : public PathListSource<TInputPath> 
{
public:
  /** Standard class typedefs. */
  typedef DrawPathListFilter                Self;
  typedef PathListSource<TInputPath>	    Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(DrawPathListFilter, PathListSource);

  /** Some convenient typedefs. */
  typedef          TInputImage                    InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointerType;
  typedef typename InputImageType::ConstPointer   InputImageConstPointerType;
  typedef typename InputImageType::RegionType     InputImageRegionType; 
  typedef typename InputImageType::PixelType      InputImagePixelType; 
  typedef typename InputImageType::SizeType       InputImageSizeType;                 
  typedef typename InputImageType::ValueType      InputImageValueType;  

  typedef typename Superclass::OutputPathType                  InputPathType;
  typedef typename Superclass::OutputPathListType              InputPathListType;
  typedef typename Superclass::OutputPathListPointerType       InputPathListPointerType;
  typedef typename Superclass::OutputPathListConstPointerType  InputPathListConstPointerType;


  typedef          TOutputImage                    OutputImageType;
  typedef typename OutputImageType::Pointer        OutputImagePointerType;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointerType;
  typedef typename OutputImageType::RegionType     OutputImageRegionType; 
  typedef typename OutputImageType::PixelType      OutputImagePixelType; 
  typedef typename OutputImageType::SizeType       OutputImageSizeType;                 
  typedef typename OutputImageType::ValueType      OutputImageValueType; 
  typedef typename OutputImageType::IndexType      OutputImageIndexType; 

  typedef itk::ProcessObject ProcessObjectType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  
  /** Set/Get the image input of this process object. */
  virtual void SetImageInput(const InputImageType *image);
  const InputImageType * GetImageInput(void);
  
  /** Set/Get the path input of this process object. */
  virtual void SetPathInput(InputPathListType *path);
  InputPathListType * GetPathInput(void);

  /** Get the image output of this process object. */
  virtual void SetImageOutput(OutputImageType *image);
  OutputImageType * GetImageOutput(void);
  
protected:
  DrawPathListFilter();
  virtual ~DrawPathListFilter() {};

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;  
  virtual void GenerateData();
  
private:
  DrawPathListFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  OutputImagePixelType m_PathValue;
  
  
  InputPathListType * m_PathList;
  
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDrawPathListFilter.txx"
#endif

#endif
 
