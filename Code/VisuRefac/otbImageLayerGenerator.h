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
#ifndef __otbImageLayerGenerator_h
#define __otbImageLayerGenerator_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "otbStreamingShrinkImageFilter.h"

namespace otb
{
/** \class ImageLayerGenerator
*   \brief Helper class to generate image layer for visualization.
*   This class is a helper class for displayable layer generation.
*   It generates a layer with all the needed information and
*   optionnaly quicklook.
*
*   It ca also suggest a subsampling rate if a user want to generate
*   the quicklook by itself. 
*
*   The output layer can be passed to the AddLayer() method of the
*   ImageViewerModel.
*
*   It can also be tuned afterward.

*   Also it is quite a heavy thing to do, an image layer can still be 
*   built by hands.
*   
*   \sa ImageViewerModel
*   \sa ImageLayer
*/
template < class TImageLayer >  
class ImageLayerGenerator
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ImageLayerGenerator                Self;
  typedef itk::Object                        Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(ImageLayerGenerator,Object);

  /** Image layer typedef */
  typedef TImageLayer                          ImageLayerType;
  typedef typename ImageLayerType::Pointer     ImageLayerPointerType;

  /** Image typedef */
  typedef typename ImageLayerType::ImageType   ImageType;
  typedef typename ImageType::Pointer          ImagePointerType;

  /** Resampler typedef */
  typedef otb::StreamingShrinkImageFilter
  <ImageType,ImageType>                        ResampleFilterType;
  typedef typename ResampleFilterType::Pointer ResampleFilterPointerType;
  
  /** Get the generated layer */
  itkGetObjectMacro(Layer,ImageLayerType);

  /**
   * Trigger layer generation.
   */
  virtual void GenerateLayer();

  /**
   * Compute optimal subsampling rate.
   * \return the optimal subsampling rate.
   */
  virtual unsigned int GetOptimalSubSamplingRate();

  /** Set/Get the image */
  itkSetObjectMacro(Image,ImageType);
  itkGetObjectMacro(Image,ImageType);

  /** Set/Get the quicklook (in case GenerateQuicklook is off) */
  itkSetObjectMacro(Quicklook,ImageType);
  itkGetObjectMacro(Quicklook,ImageType);

  /** Set/Get the subsampling rate (in case GenerateQuicklook is off) */
  itkSetMacro(SubsamplingRate,unsigned int);
  itkGetMacro(SubsamplingRate,unsigned int);

  /** Activate/deactivate quicklook generation */
  itkSetMacro(GenerateQuicklook,bool);
  itkBooleanMacro(GenerateQuicklook);

protected:
  /** Constructor */
  ImageLayerGenerator();
  /** Destructor */
  ~ImageLayerGenerator();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Generate the layer information */
  virtual void GenerateLayerInformation();
 
  /** Generate the quicklook (this method is only called if
   * QuicklookGeneration is on).
   */
  virtual void GenerateQuicklook();

private:
  ImageLayerGenerator(const Self&);     // purposely not implemented
  void operator=(const Self&);          // purposely not implemented

  /** The generated image layer */
  ImageLayerPointerType m_Layer;

  /** The input image */
  ImagePointerType      m_Image;

  /** The quicklook (Will be overriden if GenerateQuicklook is on) */
  ImagePointerType      m_Quicklook;

  /** The quicklook subsampling rate (Will be overriden if
   * GenerateQuicklook is on)
   */
  unsigned int          m_SubsamplingRate;
  
  /** if true, the generator will also generate the quicklook */
  bool                  m_GenerateQuicklook;

  /** Streaming resample filter */
  ResampleFilterPointerType m_Resampler;

}; // end class 
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageLayerGenerator.txx"
#endif

#endif


