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
#ifndef __otbStereorectificationDeformationFieldSource_h
#define __otbStereorectificationDeformationFieldSource_h

#include "itkImageSource.h"

namespace otb
{
template <class TInputImage, class TOutputImage >
class ITK_EXPORT StereorectificationDeformationFieldSource
  : public itk::ImageSource<TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef StereorectificationDeformationFieldSource Self;
  typedef itk::ImageSource<TOutputImage>            Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  typedef TInputImage                               InputImageType;
  typedef typename InputImageType::Pointer          InputImagePointerType;
  typedef TOutputImage                              OutputImageType;
  typedef typename OutputImageType::Pointer         OutputImagePointerType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( StereorectificationDeformationFieldSource, ImageSource );

  /** Get/Set the average elevation */
  itkSetMacro(AverageElevation,double);
  itkGetConstReferenceMacro(AverageElevation,double);
  
  /** Get/Set the scale */
  itkSetMacro(Scale,double);
  itkGetConstReferenceMacro(Scale,double);

  /** Get / Set the grid scale */
  itkSetMacro(GridStep,double);
  itkGetMacro(GridStep,double);

protected:
  /** Constructor */
  StereorectificationDeformationFieldSource( void );
  
  /** Destructor */
  ~StereorectificationDeformationFieldSource( void ) {};

  /** Generate output images information */
  virtual void GenerateOutputInformation();

  /** Compute the deformation field */
  virtual void GenerateData();

  /** PrintSelf method */
  virtual void PrintSelf( std::ostream& os, itk::Indent indent ) const;

private:
  StereorectificationDeformationFieldSource( const Self& ); // purposely
                                                            // not
                                                            // implemented
  void operator=( const Self& ); //purposely not implemented

  /** Disparity will be null for this elevation */
  double m_AverageElevation;

  /** A scale greater than 1 will lead to zoomed stereo-rectified
   *  pairs */
  double m_Scale;

  /** Controls the step of the resampling grid (in pixels). A finer
   * step will lead to more memory consumption. 
   */
  double m_GridStep;

  /** Left image */
  InputImagePointerType m_LeftImage;

  /** Right image */
  InputImagePointerType m_RightImage;

};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStereorectificationDeformationFieldSource.txx"
#endif

#endif
