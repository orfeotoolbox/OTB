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
#ifndef __otbImageToGenericRSOutputParameters_h
#define __otbImageToGenericRSOutputParameters_h

#include "itkObject.h"
#include "otbGenericRSTransform.h"

namespace otb {

/** \class ImageToGenericRSOutputParameters
 *  \brief This class is a helper class to estimate the output
 *  parameters of an image after projection in a target SRS (defined
 *  here using its projectionRef).
 *
 * This class allow the computation of the output Origin|Spacing|Size
 * when projecting an image in a target SRS. The user can set the
 * target projectionRef via SetOutputProjectionRef(std::string).
 *
 * This class allows forcing use of a desired spacing. For
 * instance, if one need to compute the output Size  of an
 * image in the UTM srs with a spacing of 5 m, it can be set via the
 * method ForceSpacingTo(spacing).
 *
 * In the same way, one can force the image to have a final size using
 * ForceSizeTo(size). The spacing is then computed to get all the
 * pixels of the image in the output image.
 *
 * \sa GenericRSTransform
 * \ingroup Projection
 *
 * \ingroup OTBTransform
 **/

template<class TImage>
class  ImageToGenericRSOutputParameters :
    public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ImageToGenericRSOutputParameters      Self;
  typedef itk::Object                                Superclass;
  typedef itk::SmartPointer< Self >        Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToGenericRSOutputParameters, itk::Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImage typedefs */
  typedef TImage                                     ImageType;
  typedef typename ImageType::SizeType               SizeType;
  typedef typename ImageType::IndexType              IndexType;
  typedef typename ImageType::SpacingType            SpacingType;
  typedef typename ImageType::PointType              PointType;

  /** GenericRSTransform typedef */
  typedef GenericRSTransform<>                       GenericRSTransformType;
  typedef typename GenericRSTransformType::Pointer   GenericRSTransformPointerType;

  /** Struct to store the extent of the output image when using the
   *  methods SetOutputParameters.
   */
  struct OutputImageExtentType{
    double minX;
    double maxX;
    double minY;
    double maxY;
  };

  /** Method to Set/Get the input */
  itkSetConstObjectMacro(Input, ImageType);
  itkGetConstObjectMacro(Input, ImageType);

  /** Output Origin */
  itkSetMacro(OutputOrigin, PointType);
  itkGetMacro(OutputOrigin, PointType);

  /** Output Spacing */
  itkSetMacro(OutputSpacing, SpacingType);
  itkGetMacro(OutputSpacing, SpacingType);

  /** Output Size */
  itkSetMacro(OutputSize, SizeType);
  itkGetMacro(OutputSize, SizeType);

  /** Isotropic spacing flag */
  itkSetMacro(EstimateIsotropicSpacing, bool);
  itkGetMacro(EstimateIsotropicSpacing, bool);
  itkBooleanMacro(EstimateIsotropicSpacing);

  /**
   * Method to Force the use of the spacing selected by the user
   * The output size is computed using this spacing
   */
  void ForceSpacingTo(const SpacingType& spacing)
  {
    m_ForceSpacing = true;
    this->SetOutputSpacing(spacing);
  }

  /**
   * Method to Force the use of the size selected by the user, the
   * output spacing is computed using this one
   */
  void ForceSizeTo(const SizeType& size)
  {
    m_ForceSize = true;
    this->SetOutputSize(size);
  }

  /**
   * Method to Set/Get the output projectionRef
   * We cannot use the macro otbGetObjectMemberConstMacro cause the
   * SetOutputProjectionRef calls actually the method
   * SetInputProjectionRef of the GenericRSTransform class.
   */
  void SetOutputProjectionRef(const std::string&  ref)
  {
    m_Transform->SetInputProjectionRef(ref);
    this->Modified();
  }

  std::string GetInputProjectionRef() const
  {
    return m_Transform->GetOutputProjectionRef();
  }

  /** Set/Get Input Keywordlist*/
  void SetInputKeywordList(const ImageKeywordlist& kwl)
  {
    m_Transform->SetOutputKeywordList(kwl);
    this->Modified();
  }

  const ImageKeywordlist GetInputKeywordList()
  {
    return m_Transform->GetOutputKeywordList();
  }

  /** Method to trigger the output parameters */
  void Compute();

protected:
  ImageToGenericRSOutputParameters();
  virtual ~ImageToGenericRSOutputParameters() {}

private:
  ImageToGenericRSOutputParameters(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  void UpdateTransform();
  void EstimateOutputImageExtent();
  void EstimateOutputSpacing();
  void EstimateOutputSize();
  void EstimateOutputOrigin();

  typename ImageType::ConstPointer   m_Input;
  PointType                     m_OutputOrigin;
  SpacingType                   m_OutputSpacing;
  SizeType                      m_OutputSize;
  OutputImageExtentType         m_OutputExtent;

  GenericRSTransformPointerType m_Transform;

  bool                          m_ForceSpacing;
  bool                          m_ForceSize;
  bool                          m_EstimateIsotropicSpacing;

}; // end of class ImageToGenericRSOutputParameters

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToGenericRSOutputParameters.txx"
#endif

#endif
