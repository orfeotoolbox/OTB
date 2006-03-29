/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   29 mars 2006
  Role      :  ImageToPointSetFilter Class 
  $Id$ 

=========================================================================*/
#ifndef __otbImageToPointSetFilter_h
#define __otbImageToPointSetFilter_h

#include "otbPointSetSource.h"

namespace otb
{

/** \class ImageToPointSetFilter
 * \brief 
 *
 * ImageToPointSetFilter is the base class for all process objects that output
 * Point Set data and require image data as input. Specifically, this class
 * defines the SetInput() method for defining the input to a filter.
 *
 * \ingroup ImageFilters
 */
template <class TInputImage, class TOutputPointSet>
class ITK_EXPORT ImageToPointSetFilter : public PointSetSource<TOutputPointSet>
{
public:
  /** Standard class typedefs. */
  typedef ImageToPointSetFilter            Self;
  typedef PointSetSource<TOutputPointSet>  Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToPointSetFilter, PointSetSource);

  /** Create a valid output. */
  itk::DataObject::Pointer  MakeOutput(unsigned int idx);

  /** Some Image related typedefs. */
  typedef   TInputImage                             InputImageType;
  typedef   typename InputImageType::Pointer        InputImagePointer;
  typedef   typename InputImageType::ConstPointer   InputImageConstPointer;
  typedef   typename InputImageType::RegionType     InputImageRegionType; 
  typedef   typename InputImageType::PixelType      InputImagePixelType; 

  /** Some PointSet related typedefs. */
  typedef   typename Superclass::OutputPointSetType     OutputPointSetType;
  typedef   typename Superclass::OutputPointSetPointer  OutputPointSetPointer;

  /** Set the input image of this process object.  */
  void SetInput(unsigned int idx, const InputImageType *input);

  /** Get the input image of this process object.  */
  const InputImageType * GetInput(unsigned int idx);

  /** Get the output Point Set of this process object.  */
  OutputPointSetType * GetOutput(void);

  /** Prepare the output */
  void GenerateOutputInformation(void);
     
protected:
  ImageToPointSetFilter();
  ~ImageToPointSetFilter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
 
private:
  ImageToPointSetFilter(const ImageToPointSetFilter&); //purposely not implemented
  void operator=(const ImageToPointSetFilter&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToPointSetFilter.txx"
#endif

#endif
