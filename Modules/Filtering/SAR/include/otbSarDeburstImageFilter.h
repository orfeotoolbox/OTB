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
#ifndef otbSarDeburstImageFilter_h
#define otbSarDeburstImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkSmartPointer.h"

namespace otb
{
/** \class SarDeburstImageFilter
 *  \brief TODO
 */

template <class TImage> class ITK_EXPORT SarDeburstImageFilter :
    public itk::ImageToImageFilter<TImage,TImage>
{
public:
  // Standard class typedefs
  typedef SarDeburstImageFilter                  Self;
  typedef itk::ImageToImageFilter<TImage,TImage> Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(SarDeburstImageFilter,ImageToImageFilter);

  typedef TImage                                 ImageType;
  typedef typename ImageType::RegionType         RegionType;
  typedef typename ImageType::PointType          PointType;

  typedef std::pair<unsigned long, unsigned long> LinesRecordType;
  typedef std::vector<LinesRecordType>            LinesRecordVectorType;
  
protected:
  // Constructor
  SarDeburstImageFilter();

  // Destructor
  virtual ~SarDeburstImageFilter() ITK_OVERRIDE {};

  // Needs to be re-implemented since size of output is modified
  virtual void GenerateOutputInformation() ITK_OVERRIDE;

  // Needs to be re-implemented since size of output is modified
  virtual void GenerateInputRequestedRegion() ITK_OVERRIDE;

  // Actual processing
  virtual void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;

  RegionType OutputRegionToInputRegion(const RegionType& outputRegion) const;
  
private:
  SarDeburstImageFilter(const Self&); // purposely not implemented
  void operator=(const Self &); // purposely not implemented

  // Vector of line records
  LinesRecordVectorType m_LinesRecord;
  
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSarDeburstImageFilter.txx"
#endif



#endif
