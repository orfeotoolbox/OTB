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
#ifndef __otbImageToOSMVectorDataGenerator_h
#define __otbImageToOSMVectorDataGenerator_h

#include "otbVectorData.h"
#include "otbOSMDataToVectorDataGenerator.h"

namespace otb {

/** \class ImageToOSMVectorDataGenerator
 *
 *  \brief Helper class to compute the extent of the input image and
 *   use it to fill the bbox for the OSM request.
 *
 *  This class compute the extent of the input image. The extent is
 *  then used to fill the bbox values.
 *  The class OSMDataToVectorDataGenerator request the OSM server to get
 *  the OSM XML file storing the datas within this input image bbox.
 *
 * \sa otb::OSMDataToVectorDataGenerator
 */
template < class TImage>
class  ITK_EXPORT ImageToOSMVectorDataGenerator :
    public OSMDataToVectorDataGenerator
{
public:
  /** Standard class typedefs */
  typedef ImageToOSMVectorDataGenerator          Self;
  typedef OSMDataToVectorDataGenerator           Superclass;
  typedef itk::SmartPointer< Self >              Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToOSMVectorDataGenerator, OSMDataToVectorDataGenerator);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  // Image typedef
  typedef TImage                                ImageType;
  typedef typename ImageType::SizeType          SizeType;
  typedef typename ImageType::IndexType         IndexType;
  typedef typename ImageType::PointType         PointType;

  // VectorData
  typedef Superclass::VectorDataType            VectorDataType;

  // Struct to store the extent of the image
  struct ImageExtentType{
    double minX;
    double maxX;
    double minY;
    double maxY;
  };

  /** Method to set/get the input image */
  void SetInput( const ImageType  * input);

   /** Returns the input image */
  const ImageType* GetInput() const;

protected:
  virtual void GenerateData();

  // Method to compute the extent of the image
  void EstimateImageExtent();

  ImageToOSMVectorDataGenerator();
  virtual ~ImageToOSMVectorDataGenerator() {}

private:
  ImageToOSMVectorDataGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  ImageExtentType                            m_ImageExtent;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToOSMVectorDataGenerator.txx"
#endif

#endif
