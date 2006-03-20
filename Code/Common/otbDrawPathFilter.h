/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   28 fevrier 2006
  Version   :   
  Role      :   Dessine des segments de Path sur une image
  $Id:$

=========================================================================*/
#ifndef __otbDrawPathFilter_h
#define __otbDrawPathFilter_h

#include "itkImageAndPathToImageFilter.h"

namespace otb
{
/** \class DrawPathFilter
 * \brief Classe de base pour inserer dans une image des segments de lignes. 
 *
 * 
 * \ingroup PathFilters
 * \ingroup ImageFilters
 */

template <class TInputImage, class TInputPath,class TOutputImage>
class ITK_EXPORT DrawPathFilter : public itk::ImageAndPathToImageFilter<TInputImage,TInputPath,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef DrawPathFilter                                                        Self;
  typedef itk::ImageAndPathToImageFilter<TInputImage,TInputPath,TOutputImage>   Superclass;
  typedef itk::SmartPointer<Self>                                               Pointer;
  typedef itk::SmartPointer<const Self>                                         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(DrawPathFilter, itk::ImageAndPathToImageFilter);

  /** Some convenient typedefs. */
  typedef          TInputImage                    InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointer;
  typedef typename InputImageType::ConstPointer   InputImageConstPointer;
  typedef typename InputImageType::RegionType     InputImageRegionType; 
  typedef typename InputImageType::PixelType      InputImagePixelType; 
  typedef          TInputPath                     InputPathType;
  typedef typename InputPathType::Pointer         InputPathPointer;
  typedef typename InputPathType::ConstPointer    InputPathConstPointer;
  typedef typename InputPathType::InputType       InputPathInputType;
  typedef typename InputPathType::OutputType      InputPathOutputType;
  typedef typename InputPathType::IndexType       InputPathIndexType;
  typedef typename InputPathType::OffsetType      InputPathOffsetType;
  typedef          TOutputImage                   OutputImageType;
  typedef typename OutputImageType::Pointer       OutputImagePointer;
  typedef typename OutputImageType::ConstPointer  OutputImageConstPointer;
  
  typedef typename OutputImageType::RegionType    OutputImageRegionType; 
  typedef typename OutputImageType::PixelType     OutputImagePixelType; 
  typedef typename OutputImageType::ValueType     OutputImageValueType; 
  

//OTB-FA-00011-CS
  itkSetMacro(Value, OutputImagePixelType);
  itkGetConstReferenceMacro(Value, OutputImagePixelType);
  
protected:
  DrawPathFilter();
  virtual ~DrawPathFilter() {};

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;  
  virtual void GenerateData();
 
private:
  DrawPathFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OutputImagePixelType m_Value;

  
  
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDrawPathFilter.txx"
#endif

#endif
 
