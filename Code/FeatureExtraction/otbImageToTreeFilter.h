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
  typedef typename InputImageType::Pointer        InputImagePointer;
  typedef typename InputImageType::ConstPointer   InputImageConstPointer;
  typedef typename InputImageType::RegionType     InputImageRegionType; 
  typedef typename InputImageType::PixelType      InputImagePixelType; 
  typedef typename InputImageType::SizeType       InputImageSizeType;                 
  typedef typename InputImageType::ValueType      InputImageValueType;  
  typedef typename InputImageType::IndexType      InputImageIndexType;  

  typedef typename Superclass::OutputTreeType          OutputTreeType;
  typedef typename Superclass::OutputTreePointer       OutputTreePointer;
  typedef typename Superclass::OutputTreeConstPointer  OutputTreeConstPointer;

  typedef itk::DataObject                       DataObject;
  typedef itk::DataObject::Pointer              DataObjectPointer;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  
  DataObjectPointer  MakeOutput(unsigned int idx);

  /** Set the input image of this process object.  */
  void SetInput(unsigned int idx, const InputImageType *input);

  /** Get the input image of this process object.  */
  const InputImageType * GetInput(unsigned int idx);

  /** Get the output Mesh of this process object.  */
  OutputTreeType * GetOutput(void);

  /** Prepare the output */
  void GenerateOutputInformation(void);

protected:
  ImageToTreeFilter();
  ~ImageToTreeFilter();

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;  

private:
  ImageToTreeFilter(const ImageToTreeFilter&); //purposely not implemented
  void operator=(const ImageToTreeFilter&);    //purposely not implemented
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToTreeFilter.txx"
#endif

#endif
 
