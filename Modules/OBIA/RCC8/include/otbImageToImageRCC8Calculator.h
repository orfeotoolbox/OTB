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
#ifndef __otbImageToImageRCC8Calculator_h
#define __otbImageToImageRCC8Calculator_h

#include "itkImageToImageFilter.h"
#include "itkObjectFactory.h"
#include "otbRCC8Value.h"
#include "otbImage.h"

namespace otb
{
/**
 * \class ImageToImageRCC8Calculator
 * \brief This class compute the RCC8 relation between the regions from two segmentation images.
 *
 * The RCC8 system comes from qualitative spatial reasoning.
 * It is a set of pairwise disjoint exhaustive relation between two closed region of space.
 * There are 8 possible relations :
 * DC: Disconnected
 * EC: Externaly connected
 * PO: Partial overlap
 * TPP: Tangential proper part
 * NTPP: Non tangential proper part
 * TPPI: Tangential proper part inverse
 * NTPPI: Non tangential proper part inverse
 * EQ: Equivalence
 *
 * The goal of this class is to determine which of these 8 relations link the two inputs regions represented
 * by the segmentation images. Since this class will further be used iteratively on a possibly large set
 * of region, it is optimised : the decision is managed by a decision tree, and the input data size is reduced
 * to the smallest set needed to determine the relation.
 *
 * \ingroup OTBRCC8
 */
template <class TInputImage>
class ITK_EXPORT ImageToImageRCC8Calculator : public itk::ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef ImageToImageRCC8Calculator                        Self;
  typedef itk::ImageToImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToImageRCC8Calculator, ImageToImageFilter);
  /** Types definitions for the input image. */
  typedef TInputImage                      ImageType;
  typedef typename ImageType::Pointer      ImagePointerType;
  typedef typename ImageType::ConstPointer ImageConstPointerType;
  typedef typename ImageType::PixelType    PixelType;
  typedef typename ImageType::IndexType    IndexType;
  typedef typename ImageType::RegionType   RegionType;
  /** Internal bool image type */
  typedef bool                                                 BoolPixelType;
  typedef otb::Image<BoolPixelType, ImageType::ImageDimension> BoolImageType;
  typedef typename BoolImageType::Pointer                      BoolImagePointerType;

  /** RCC8 values type */
  typedef RCC8Value RCC8ValueType;
  /**
   * Get the RCC8 relation.
   * \return The RCC8 relation value.
   */
  RCC8ValueType GetValue(void);
  /**
   * Set the first input image.
   * \param image
   */
  void SetInput1(ImageType * image);
  /**
   * Set the second input image.
   * \param image
   */
  void SetInput2(ImageType * image);
  /**
   * Get the first input image.
   * \return The first input image.
   */
  ImageType * GetInput1(void);
  /**
   * Get the second input image.
   * \return The second input image.
   */
  ImageType * GetInput2(void);
  /** Set/Get the inside value of the region of image 1*/
  itkSetMacro(InsideValue1, PixelType);
  itkGetMacro(InsideValue1, PixelType);
  /** Set/Get the inside value of the region of image 2*/
  itkSetMacro(InsideValue2, PixelType);
  itkGetMacro(InsideValue2, PixelType);
  /** Set external knowledge to help the decision process */
  itkSetMacro(Level1APrioriKnowledge, bool);
  itkSetMacro(Level3APrioriKnowledge, bool);
  itkGetMacro(Level1APrioriKnowledge, bool);
  itkGetMacro(Level3APrioriKnowledge, bool);
protected:
  /** Constructor */
  ImageToImageRCC8Calculator();
  /** Destructor */
  ~ImageToImageRCC8Calculator() ITK_OVERRIDE {}
  /**
   * Compute the minimal image region required.
   * \return The minimal region required.
   */
  RegionType ComputeMinimalRegion(void);
  /**
   * Compute the intersection between regions edges.
   * \return true if the intersection is not empty.
   */
  bool ComputeEdgeEdgeBool(void);
  /**
   * Compute the intersection between exterior of region1 and
   * interior of region2.
   * \return true if the intersection is not empty.
   */
  bool ComputeExterInterBool(void);
  /**
   * Compute the intersection between interior of region1 and
   * exterior of region2.
   * \return true if the intersection is not empty.
   */
  bool ComputeInterExterBool(void);
  /**
   * Compute the intersection between regions interiors.
   * \return true if the intersection is not empty.
   */
  bool ComputeInterInterBool(void);
  /**
   * Compute the relation value from the input booleans. Please note
   * that the actual computed value is set to the m_Value parameters, and has
   * nothing to do with the returned boolean, which indicates if the determination
   * process was successful.
   * \param edgeEdgeBool True if edge-edge intersection is not empty.
   * \param interExterBool True if interior-exterior intersection is not empty.
   * \param exterInterBool True if exterior-interior intersection is not empty.
   * \return True if the decision process was successful.
   */
  bool ComputeRelation(bool edgeEdgeBool, bool interExterBool, bool exterInterBool);
  /**
   * Test if the boolean image is totally black or not. This is a based on the lazy operator
   * paradigm.
   * \param image The image to test.
   * \return True or false.
   */
  bool IsBoolImageNotEmpty(BoolImagePointerType image);
  /**
   * Compute a bool image of minimal ROI size, surrounded by a false padding, and corresponding
   * to the input image.
   * \param image The image to convert.
   * \param insideValue The inside value.
   * \return The converted image
   */
  BoolImagePointerType ConvertToBoolImage(ImagePointerType image, PixelType insideValue);
  /** Main computation method */
  void GenerateData(void) ITK_OVERRIDE;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

private:
  /** The RCC8 relation value */
  RCC8ValueType m_Value;
  /** The minimal ROI needed to compute the relation value */
  RegionType m_MinimalROI;
  /**  Decision tree Level 1 A priori knowledge */
  bool m_Level1APrioriKnowledge;
  /**  Decision tree Level 3 A priori knowledge */
  bool m_Level3APrioriKnowledge;
  /** Inside value 1 */
  PixelType m_InsideValue1;
  /** Inside value 2 */
  PixelType m_InsideValue2;
  /** Internal bool image representation*/
  BoolImagePointerType m_BoolImage1;
  BoolImagePointerType m_BoolImage2;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToImageRCC8Calculator.txx"
#endif

#endif
