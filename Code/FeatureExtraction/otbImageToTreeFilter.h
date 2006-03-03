/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P. Imbo
  Language  :   C++
  Date      :   20 fevrier 2006
  Version   :   
  Role      :   Classe de base d'une TreeList 
  $Id:$

=========================================================================*/
#ifndef __otbImageToTreeFilter_h
#define __otbImageToTreeFilter_h

#include "otbTreeSource.h"
namespace otb
{
/** \class ImageToTreeFilter
 * \brief Classe de base pour filter des données ayant en entrée une image et en sortie une list de Tree. 
 *
 * 
 * \ingroup TreeListFilters
 * \ingroup TreeLists
 */

template <class TInputImage, class TOutputTree>
class ITK_EXPORT ImageToTreeFilter : public TreeSource<TOutputTree> 
{
public:
  /** Standard class typedefs. */
  typedef ImageToTreeFilter                 Self;
  typedef TreeSource<TOutputTree>	    Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToTreeFilter, TreeSource);

  /** Some convenient typedefs. */
  typedef          TInputImage                    InputImageType;
  typedef typename InputImageType::ConstPointer   InputImagePointer;
  typedef typename InputImageType::RegionType     InputImageRegionType; 
  typedef typename InputImageType::PixelType      InputImagePixelType; 
  typedef typename InputImageType::SizeType       InputImageSizeType;                 
  typedef typename InputImageType::ValueType      InputImageValueType;  
  typedef typename InputImageType::IndexType      InputImageIndexType;  

  typedef typename Superclass::OutputTreeType              OutputTreeType;
  typedef typename Superclass::OutputTreePointerType       OutputTreePointerType;
  typedef typename Superclass::OutputTreeConstPointerType  OutputTreeConstPointerType;

  typedef typename Superclass::PathType  	        PathType;
  typedef typename Superclass::PathPointerType  	PathPointerType;

  typedef typename Superclass::ShapeType     	        ShapeType;
  typedef typename Superclass::ShapePointerType     	ShapePointerType;
  typedef typename Superclass::ShapeConstPointerType   	ShapeConstPointerType;
  
  typedef typename Superclass::ShapeTreeType 	          ShapeTreeType;  
  typedef typename Superclass::ShapeTreePointerType       ShapePointerType;
  typedef typename Superclass::ShapeTreeConstPointerType  ShapeConstPointerType;

  typedef typename Superclass::PointPlaneType              PointPlaneType;
  typedef typename Superclass::PointPlaneListType          PointPlaneListType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  
  /** Set/Get the image input of this process object. */
  virtual void SetInput( const InputImageType * image);
  const InputImageType * GetInput(void);

protected:
  ImageToTreeFilter();
  virtual ~ImageToTreeFilter() {};

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;  

private:
  ImageToTreeFilter(const Self&); //purposely not implemented
  void operator=(const Self&);    //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToTreeFilter.txx"
#endif

#endif
 
