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

#ifndef __otbVectorDataToImageFilter_h
#define __otbVectorDataToImageFilter_h

#include "itkImageSource.h"

namespace otb
{
/** \class VectorDataToImageFilter
   *   \brief Render a vector data into an image
   *
 */


template <class TVectorData, class TImage>
  class ITK_EXPORT VectorDataToImageFilter : public itk::ImageSource<TImage>
{
  public:
    /** Standard class typedefs. */
    typedef VectorDataToImageFilter  Self;
    typedef itk::ImageSource<TImage>  Superclass;
    typedef itk::SmartPointer<Self>   Pointer;
    typedef itk::SmartPointer<const Self>  ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(VectorDataToImageFilter, ImageSource);

    /** Some typedefs. */
    typedef TVectorData VectorDataType;
    typedef TImage ImageType;


    virtual void SetInput( const VectorDataType *input);
    const VectorDataType * GetInput(void);


  protected:
    /** Constructor */
    VectorDataToImageFilter();
    /** Destructor */
    virtual ~VectorDataToImageFilter() {};
    /**PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

    virtual void GenerateData(void);


  private:
    VectorDataToImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented


}; // end class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToImageFilter.txx"
#endif

#endif