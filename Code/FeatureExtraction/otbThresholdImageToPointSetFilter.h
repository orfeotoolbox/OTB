/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   30 mars 2006
  Role      :   Threshold To PointSet Filter 
  $Id$ 

=========================================================================*/
#ifndef __otbThresholdImageToPointSetFilter_h
#define __otbThresholdImageToPointSetFilter_h

#include "otbImageToPointSetFilter.h"
#include "itkPointSet.h"

namespace otb
{

/** \class ThresholdImageToPointSetFilter
 *
 */

template <class TInputImage, 
//          class TOutputPointSet>
         class TOutputPointSet = itk::PointSet<ITK_TYPENAME TInputImage::PixelType,2> >
class ThresholdImageToPointSetFilter :  
           public ImageToPointSetFilter< TInputImage,TOutputPointSet >
{
public:

  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);

  typedef TInputImage     InputImageType;
  typedef TOutputPointSet OutputPointSetType;

  typedef ThresholdImageToPointSetFilter                             Self;
  typedef ImageToPointSetFilter< InputImageType, OutputPointSetType> Superclass;
  typedef itk::SmartPointer<Self>                                    Pointer;
  typedef itk::SmartPointer<const Self>                              ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(ThresholdImageToPointSetFilter, ImageToPointSetFilter);
  
  typedef typename Superclass::InputImagePixelType        InputPixelType;
  typedef typename Superclass::InputImagePointer          InputImagePointer;
  typedef typename Superclass::InputImageConstPointer     InputImageConstPointer;
  typedef typename Superclass::InputImageType::SizeType   SizeType;
  typedef typename Superclass::InputImageType::IndexType  IndexType;

  typedef   typename Superclass::OutputPointSetType             OutputPointSetType;
  typedef   typename Superclass::OutputPointSetPointer          OutputPointSetPointer;
  typedef   typename Superclass::OutputPointSetType::PixelType  OutputPointSetPixelType;

  itkSetMacro(Threshold,InputPixelType);
  itkGetConstReferenceMacro(Threshold, InputPixelType);

protected:
  ThresholdImageToPointSetFilter();
  virtual ~ThresholdImageToPointSetFilter() {};

  virtual void GenerateData();
  
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ThresholdImageToPointSetFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InputPixelType m_Threshold;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbThresholdImageToPointSetFilter.txx"
#endif

  
#endif
