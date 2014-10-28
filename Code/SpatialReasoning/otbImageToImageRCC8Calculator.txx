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
#ifndef __otbImageToImageRCC8Calculator_txx
#define __otbImageToImageRCC8Calculator_txx

#include "otbImageToImageRCC8Calculator.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkAndImageFilter.h"
#include "itkImageRegionIterator.h"
#include "otbBinaryImageMinimalBoundingRegionCalculator.h"
#include "otbMacro.h"

//TODELETE  #include "otbImageFileWriter.h"
//TODELETE  #include "itkCastImageFilter.h"

namespace otb
{
/**
 * Constructor
 */
template<class TInputImage>
ImageToImageRCC8Calculator<TInputImage>
::ImageToImageRCC8Calculator()
{
  m_Value        = OTB_RCC8_DC;
  m_InsideValue1  = static_cast<PixelType>(255);
  m_InsideValue2  = static_cast<PixelType>(255);
  m_Level1APrioriKnowledge = false;
  m_Level3APrioriKnowledge = false;
  this->SetNumberOfRequiredInputs(2);
}
/**
 * Set the first input image.
 * \param image
 */
template<class TInputImage>
void
ImageToImageRCC8Calculator<TInputImage>
::SetInput1(ImageType * image)
{
  this->SetNthInput(0, const_cast<ImageType *>(image));
}
/**
 * Set the second input image.
 * \param image
 */
template<class TInputImage>
void
ImageToImageRCC8Calculator<TInputImage>
::SetInput2(ImageType * image)
{
  this->SetNthInput(1, const_cast<ImageType *>(image));
}
/**
 * Get the first input image.
 * \return The first input image.
 */
template<class TInputImage>
typename ImageToImageRCC8Calculator<TInputImage>
::ImageType*
ImageToImageRCC8Calculator<TInputImage>
::GetInput1(void)
{
  return dynamic_cast<ImageType*>(this->itk::ProcessObject::GetInput(0));
}
/**
 * Get the second input image.
 * \return The second input image.
 */
template<class TInputImage>
typename ImageToImageRCC8Calculator<TInputImage>
::ImageType*
ImageToImageRCC8Calculator<TInputImage>
::GetInput2(void)
{
  return dynamic_cast<ImageType*>(this->itk::ProcessObject::GetInput(1));
}
/**
 * Get the RCC8 relation.
 * \return The RCC8 relation value.
 */
template <class TInputImage>
typename ImageToImageRCC8Calculator<TInputImage>
::RCC8ValueType
ImageToImageRCC8Calculator<TInputImage>
::GetValue(void)
{
  return m_Value;
}
/**
 * Compute the minimal image region required.
 * \return The minimal region required.
 */
template <class TInputImage>
typename ImageToImageRCC8Calculator<TInputImage>
::RegionType
ImageToImageRCC8Calculator<TInputImage>
::ComputeMinimalRegion(void)
{
  // Input images pointers
  typename ImageType::Pointer    image1 = this->GetInput1();
  typename ImageType::Pointer    image2 = this->GetInput2();
  typename ImageType::RegionType region1, region2, region;
  typedef otb::BinaryImageMinimalBoundingRegionCalculator<ImageType> RegionCalculator;
  typename RegionCalculator::Pointer rc = RegionCalculator::New();
  rc->SetInput(image1);
  rc->SetPad(2);
  rc->SetInsideValue(this->GetInsideValue1());
  rc->Update();
  region1 = rc->GetRegion();
  rc = RegionCalculator::New();
  rc->SetInput(image2);
  rc->SetPad(2);
  rc->SetInsideValue(this->GetInsideValue2());
  rc->Update();
  region2 = rc->GetRegion();
  // otbMsgDebugMacro(<<"RCC8Calculator->ComputeMinimalRegion() Region1: index: "<<region1.GetIndex()<<" size: "<<region1.GetSize());
  // otbMsgDebugMacro(<<"RCC8Calculator->ComputeMinimalRegion() Region2: index: "<<region2.GetIndex()<<" size: "<<region2.GetSize());

  //TODELETE     std::cout<<"RCC8Calculator->ComputeMinimalRegion() Region1: index: "<<region1.GetIndex()<<" size: "<<region1.GetSize()<<std::endl;
//TODELETE      std::cout<<"RCC8Calculator->ComputeMinimalRegion() Region2: index: "<<region2.GetIndex()<<" size: "<<region2.GetSize()<<std::endl;

  typename ImageType::SizeType  size;
  typename ImageType::IndexType index;

  for (int i = 0; i < ImageType::ImageDimension; ++i)
    {
    index[i] = std::min(region1.GetIndex()[i], region2.GetIndex()[i]);
    int potSize = std::max(region1.GetIndex()[i] + region1.GetSize()[i],
                           region2.GetIndex()[i] + region2.GetSize()[i]);
    size[i] = (potSize - index[i] < 0 ? 0 : potSize - index[i]);
    }
  region.SetIndex(index);
  region.SetSize(size);
  region.PadByRadius(2);
  region.Crop(image1->GetLargestPossibleRegion());
  region.Crop(image2->GetLargestPossibleRegion());
  // otbMsgDebugMacro(<<"RCC8Calculator->ComputeMinimalRegion(): index: "<<index<<" size: "<<size);
  //TODELETE   std::cout<<"RCC8Calculator->ComputeMinimalRegion(): index: "<<index<<" size: "<<size<<std::endl;
  return region;
}
/**
 * Compute a bool image of minimal ROI size, surrounded by a false padding, and corresponding
 * to the input image.
 * \param image The image to convert.
 * \param insideValue The inside value.
 * \return The converted image
 */
template<class TInputImage>
typename ImageToImageRCC8Calculator<TInputImage>
::BoolImagePointerType
ImageToImageRCC8Calculator<TInputImage>
::ConvertToBoolImage(ImagePointerType image, PixelType insideValue)
{
  typedef itk::ImageRegionConstIterator<ImageType> ConstIterator;
  typedef itk::ImageRegionIterator<BoolImageType>  Iterator;
  typename BoolImageType::Pointer  output = BoolImageType::New();
  typename BoolImageType::SizeType boolImageSize;
  boolImageSize[0] = m_MinimalROI.GetSize()[0] + 2;
  boolImageSize[1] = m_MinimalROI.GetSize()[1] + 2;
  typename BoolImageType::IndexType boolImageIndex;
  boolImageIndex[0] = m_MinimalROI.GetIndex()[0] - 1;
  boolImageIndex[1] = m_MinimalROI.GetIndex()[1] - 1;
  //otbMsgDebugMacro(<<"RCC8Calculator->ConvertToBoolImage() size: "<<boolImageSize<<" index: "<<boolImageIndex);

  typename BoolImageType::RegionType boolRegion;
  boolRegion.SetSize(boolImageSize);
  boolRegion.SetIndex(boolImageIndex);
  output->SetRegions(boolRegion);
  output->Allocate();
  output->FillBuffer(false);

  ConstIterator inputIt(image, m_MinimalROI);
  Iterator outputIt(output, m_MinimalROI);
  inputIt.GoToBegin();
  outputIt.GoToBegin();
  while (!inputIt.IsAtEnd() && !outputIt.IsAtEnd())
    {
    outputIt.Set(inputIt.Get() == insideValue);
    ++inputIt;
    ++outputIt;
    }

  return output;
}
/**
 * Compute the intersection between regions edges.
 * \return true if the intersection is not empty.
 */
template<class TInputImage>
bool
ImageToImageRCC8Calculator<TInputImage>
::ComputeEdgeEdgeBool(void)
{

  /// Definition of the Filters used to compute the boolean
  typedef itk::SubtractImageFilter<BoolImageType, BoolImageType,
      BoolImageType>
  SubtractFilterType;
  typedef itk::BinaryBallStructuringElement<bool,
      BoolImageType::ImageDimension>
  BinaryBallStructuringElementType;
  typedef itk::BinaryDilateImageFilter<BoolImageType, BoolImageType, BinaryBallStructuringElementType> DilateFilterType;
  typedef itk::AndImageFilter<BoolImageType, BoolImageType, BoolImageType>                             AndFilterType;
  /// Declaration and instantiation
  typename DilateFilterType::Pointer   dilateFilter1 = DilateFilterType::New();
  typename DilateFilterType::Pointer   dilateFilter2 = DilateFilterType::New();
  typename SubtractFilterType::Pointer subtractFilter1 = SubtractFilterType::New();
  typename SubtractFilterType::Pointer subtractFilter2 = SubtractFilterType::New();
  typename AndFilterType::Pointer      andFilter = AndFilterType::New();
  /// Configuration of the erosion filter
  BinaryBallStructuringElementType structElement1, structElement2;
  structElement1.SetRadius(1);
  structElement2.SetRadius(1);
  structElement1.CreateStructuringElement();
  structElement2.CreateStructuringElement();
  dilateFilter1->SetKernel(structElement1);
  dilateFilter2->SetKernel(structElement2);
  /// The erosion is performed to get the surounding edge of this
  /// region by substraction to the original image
  dilateFilter1->SetInput(m_BoolImage1);
  dilateFilter1->Update();
  subtractFilter1->SetInput2(m_BoolImage1);
  subtractFilter1->SetInput1(dilateFilter1->GetOutput());
  subtractFilter1->Update();
  /// The erosion is performed to get the surounding edge of this
  /// region by substraction to the original image
  dilateFilter2->SetInput(m_BoolImage2);
  dilateFilter2->Update();
  subtractFilter2->SetInput2(m_BoolImage2);
  subtractFilter2->SetInput1(dilateFilter2->GetOutput());
  subtractFilter2->Update();
  /// Now we can compute the intersection between the 2 edges
  andFilter->SetInput1(subtractFilter1->GetOutput());
  andFilter->SetInput2(subtractFilter2->GetOutput());
  andFilter->Update();
  /// test if the intersection is empty or not
  return this->IsBoolImageNotEmpty(andFilter->GetOutput());
}
/**
 * Compute the intersection between exterior of region1 and
 * interior of region2.
 * \return true if the intersection is not empty.
 */
template<class TInputImage>
bool
ImageToImageRCC8Calculator<TInputImage>
::ComputeExterInterBool(void)
{
  /// Definition of the filters used
  typedef itk::InvertIntensityImageFilter<BoolImageType, BoolImageType>    InvertFilterType;
  typedef itk::AndImageFilter<BoolImageType, BoolImageType, BoolImageType> AndFilterType;
  /// Declaration and instantiation
  typename InvertFilterType::Pointer invert = InvertFilterType::New();
  typename AndFilterType::Pointer    andFilter = AndFilterType::New();
  /// The exterior is the inverted input image
  invert->SetMaximum(true);
  invert->SetInput(m_BoolImage1);
  andFilter->SetInput1(m_BoolImage2);
  andFilter->SetInput2(invert->GetOutput());
  andFilter->Update();
  /// test if the intersection is empty or not
  return IsBoolImageNotEmpty(andFilter->GetOutput());
}
/**
 * Compute the intersection between interior of region1 and
 * exterior of region2.
 * \return true if the intersection is not empty.
 */
template<class TInputImage>
bool
ImageToImageRCC8Calculator<TInputImage>
::ComputeInterExterBool(void)
{
  /// Definition of the filters used
  typedef itk::InvertIntensityImageFilter<BoolImageType, BoolImageType>    InvertFilterType;
  typedef itk::AndImageFilter<BoolImageType, BoolImageType, BoolImageType> AndFilterType;
//TODELETE     typedef otb::Image<unsigned char, 2> TmpImageType;
//TODELETE     typedef itk::CastImageFilter<BoolImageType, TmpImageType> CastFilterType;

//TODELETE     typedef ImageFileWriter<TmpImageType> WriterType;
/// Declaration and instantiation
  typename InvertFilterType::Pointer invert = InvertFilterType::New();
  typename AndFilterType::Pointer    andFilter = AndFilterType::New();
  /// The exterior is the inverted input image
  invert->SetMaximum(true);
  invert->SetInput(m_BoolImage2);

//TODELETE     typename CastFilterType::Pointer caster = CastFilterType::New();
//TODELETE    caster->SetInput(invert->GetOutput());
//TODELETE     typename WriterType::Pointer writer = WriterType::New();
//TODELETE      writer->SetFileName("invert.tif");
//TODELETE     writer->SetInput(caster->GetOutput());
//TODELETE      writer->Update();

  andFilter->SetInput1(m_BoolImage1);
  andFilter->SetInput2(invert->GetOutput());
  andFilter->Update();

//TODELETE     caster = CastFilterType::New();
//TODELETE     caster->SetInput(andFilter->GetOutput());
//TODELETE     writer = WriterType::New();
//TODELETE     writer->SetFileName("and.tif");
//TODELETE     writer->SetInput(caster->GetOutput());
//TODELETE     writer->Update();

  /// test if the intersection is empty or not
  return IsBoolImageNotEmpty(andFilter->GetOutput());
}
/**
 * Compute the intersection between regions interiors.
 * \return true if the intersection is not empty.
 */
template<class TInputImage>
bool
ImageToImageRCC8Calculator<TInputImage>
::ComputeInterInterBool(void)
{
  /// Definition of the filters used
  typedef itk::AndImageFilter<BoolImageType, BoolImageType, BoolImageType> AndFilterType;

  /// Declaration and instantiation
  typename AndFilterType::Pointer andFilter = AndFilterType::New();
  /// The exterior is the inverted input image
  andFilter->SetInput1(m_BoolImage1);
  andFilter->SetInput2(m_BoolImage2);
  andFilter->Update();
  /// test if the intersection is empty or not
  return IsBoolImageNotEmpty(andFilter->GetOutput());
}
/**
 * Compute the relation value from the input booleans. Please note
 * that the actual computed value is set to the m_Value parameters, and has
 * nothing to do with the returned boolean, which indicates if the determination
 * process was successful.
 * \param edgeEdgeBool True if edge-edge intersection is not empty.
 * \param interExterBool True if interior-exterior intersection is not empty.
 * \param ExterInterBool True if exterior-interior intersection is not empty.
 * \return True if the decision process was successful.
 */
template<class TInputImage>
bool
ImageToImageRCC8Calculator<TInputImage>
::ComputeRelation(bool edgeEdgeBool, bool interExterBool, bool exterInterBool)
{
  // otbMsgDebugMacro(<<"RCC8Calculator->ComputeRelation()");
  // This decision process is based on a decision tree
  if ((!interExterBool) && (edgeEdgeBool) && (!exterInterBool))
    {
    m_Value = OTB_RCC8_EQ;
    return true;
    }
  else if ((!interExterBool) && (edgeEdgeBool) && (exterInterBool))
    {
    m_Value = OTB_RCC8_TPP;
    return true;
    }
  else if ((interExterBool) && (!edgeEdgeBool) && (!exterInterBool))
    {
    m_Value = OTB_RCC8_NTPPI;
    return true;
    }
  else if ((interExterBool) && (!edgeEdgeBool) && (exterInterBool))
    {
    m_Value = OTB_RCC8_DC;
    return true;
    }
  else if ((interExterBool) && (edgeEdgeBool) && (!exterInterBool))
    {
    m_Value = OTB_RCC8_TPPI;
    return true;
    }
  else
    {
    return false;
    }
}
/**
 * Test if the boolean image is totally black or not. This is a based on the lazy operator
 * paradigm.
 * \param The image to test.
 * \return True or false.
 */
template<class TInputImage>
bool
ImageToImageRCC8Calculator<TInputImage>
::IsBoolImageNotEmpty(BoolImagePointerType image)
{
  typedef itk::ImageRegionConstIterator<BoolImageType> IteratorType;
  // TODO : we'll eventually need to change something.
  IteratorType it(image, image->GetLargestPossibleRegion());
  it.GoToBegin();
  while (!it.IsAtEnd())
    {
    if (it.Get())
      {
      return true;
      }
    ++it;
    }
  return false;
}
/**
 * Main computation method.
 */
template <class TInputImage>
void
ImageToImageRCC8Calculator<TInputImage>
::GenerateData(void)
{
  // otbMsgDebugMacro(<<"RCC8Calculator->GenerateData()");
  /// First we compute the minimal region of interest we will use for the relation computation
  m_MinimalROI = this->ComputeMinimalRegion();
  /// If they are disjoint, the answer is trivial
  if ((m_MinimalROI.GetSize()[0] <= 1) || (m_MinimalROI.GetSize()[1] <= 1))
    {
    /// The relation is DC
    m_Value = OTB_RCC8_DC;
    // otbMsgDebugMacro(<<"RCC8Calculator->GenerateData(): Disjoint regions");
    }
  else
    {
    /// else each input images is cast to boolean type and reduced to
    // the minimal region
    m_BoolImage1 = ConvertToBoolImage(this->GetInput1(), m_InsideValue1);
    m_BoolImage2 = ConvertToBoolImage(this->GetInput2(), m_InsideValue2);
    // otbMsgDebugMacro(<<"RCC8Calculator->GenerateData(): Bool images computed: "<<m_BoolImage1->GetLargestPossibleRegion().GetSize());
    /// Then the boolean which will be used to determine the relation
    /// are declared
    bool edgeEdgeBool, interExterBool, exterInterBool, interInterBool;
    /// The boolean edgeEdge is needed in each case, so it si computed
    /// now
    edgeEdgeBool = ComputeEdgeEdgeBool();
    // otbMsgDebugMacro(<<"RCC8Calculator->GenerateData(): edgeEdge "<<edgeEdgeBool);
    //TODELETE  std::cout<<"RCC8Calculator->GenerateData(): edgeEdge "<<edgeEdgeBool<<std::endl;
    /// Here comes the outside knowledge
    if (this->GetLevel1APrioriKnowledge())
      {
      /// If the Level1APrioriKnowledge is set, then the
      /// interExterBool is set to true
      // otbMsgDebugMacro(<<"RCC8Calculator->GenerateData(): Level1APrioriKnowledge.");
      interExterBool = true;
      }
    else
      {
      /// Else it must be computed
      interExterBool = ComputeInterExterBool();
      // otbMsgDebugMacro(<<"RCC8Calculator->GenerateData(): interExter "<<interExterBool);
      }
    /// At this stage we can determine if the relation is of type NTPP
    //TODELETE  std::cout<<"RCC8Calculator->GenerateData(): interExter "<<interExterBool<<std::endl;
    if ((!interExterBool) && (!edgeEdgeBool))
      {
      m_Value = OTB_RCC8_NTPP;
      }
    else
      {
      /// If not, we must consider the intersection between exterior
      if (this->GetLevel3APrioriKnowledge())
        {
        /// If the Level3APRioriKnowledge flag is set, this boolean
        /// can be determined from the two others
        // otbMsgDebugMacro(<<"RCC8Calculator->GenerateData(): Level3APrioriKnowledge.");
        exterInterBool = true;
        }
      else
        {
        /// Else it must be computed
        exterInterBool = ComputeExterInterBool();
        // otbMsgDebugMacro(<<"RCC8Calculator->GenerateData(): ExterInter "<<exterInterBool);
        }
      //TODELETE      std::cout<<"RCC8Calculator->GenerateData(): ExterInter "<<exterInterBool<<std::endl;
      /// If it is not sufficient to compute the relation
      if (!ComputeRelation(edgeEdgeBool, interExterBool, exterInterBool))
        {
        /// Compute the last boolean
        interInterBool = ComputeInterInterBool();
        //TODELETE    std::cout<<"RCC8Calculator->GenerateData(): InterInter "<<interInterBool<<std::endl;
        // otbMsgDebugMacro(<<"RCC8Calculator->GenerateData(): InterInter "<<interInterBool);
        /// Which allow the full determination
        if ((interExterBool) && (edgeEdgeBool) && (exterInterBool) && (!interInterBool))
          {
          m_Value = OTB_RCC8_EC;
          }
        else
          {
          m_Value = OTB_RCC8_PO;
          }
        }
      }
    }
}
/**
* PrintSelf method
*/
template<class TInputImage>
void
ImageToImageRCC8Calculator<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk

#endif
