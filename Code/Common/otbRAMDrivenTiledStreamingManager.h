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
#ifndef __otbRAMDrivenTiledStreamingManager_h
#define __otbRAMDrivenTiledStreamingManager_h

#include "otbStreamingManager.h"

namespace otb
{

/** \class RAMDrivenTiledStreamingManager
 *  \brief This class computes the divisions needed to stream an image in square tiles,
 *  according to a user-defined available RAM
 *
 * You can use SetAvailableRAMInMB to set the available RAM. An estimation of the pipeline
 * memory print will be done, and the number of divisions will then be computed to fit
 * the available RAM
 *
 * \sa StreamingImageFileWriter
 * \sa StreamingImageVirtualFileWriter
 */
template<class TImage>
class ITK_EXPORT RAMDrivenTiledStreamingManager : public StreamingManager<TImage>
{
public:
  /** Standard class typedefs. */
  typedef RAMDrivenTiledStreamingManager Self;
  typedef StreamingManager<TImage>       Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  typedef TImage                          ImageType;
  typedef typename Superclass::RegionType RegionType;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(RAMDrivenTiledStreamingManager, itk::LightObject);

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  /** The number of Megabytes available (if 0, the configuration option is
    used)*/
  itkSetMacro(AvailableRAMInMB, unsigned int);

  /** The number of Megabytes available (if 0, the configuration option is
    used)*/
  itkGetMacro(AvailableRAMInMB, unsigned int);

  /** Actually computes the stream divisions, according to the specified streaming mode,
   * eventually using the input parameter to estimate memory consumption */
  virtual void PrepareStreaming(itk::DataObject * input, const RegionType &region);

protected:
  RAMDrivenTiledStreamingManager();
  virtual ~RAMDrivenTiledStreamingManager();

  /** The splitter type used to generate the different strips */
  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)> SplitterType;

  /** The number of MegaBytes of RAM available */
  unsigned int m_AvailableRAMInMB;
private:
  RAMDrivenTiledStreamingManager(const RAMDrivenTiledStreamingManager &);
  void operator =(const RAMDrivenTiledStreamingManager&);
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRAMDrivenTiledStreamingManager.txx"
#endif

#endif
