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
#ifndef otbGDALDatasetWrapper_h
#define otbGDALDatasetWrapper_h

#include "itkLightObject.h"
#include "itkObjectFactory.h"

#include "otbConfigure.h"


class GDALDataset;


namespace otb
{

// only two states : the Pointer is Null or GetDataSet() returns a
// valid dataset
class GDALDatasetWrapper : public itk::LightObject
{
  friend class GDALDriverManagerWrapper;

public:
  typedef GDALDatasetWrapper      Self;
  typedef itk::LightObject        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GDALImageIO, itk::LightObject);

  /** Easy access to the internal GDALDataset object.
   *  Don't close it, it will be automatic */
  const GDALDataset * GetDataSet() const;
  GDALDataset * GetDataSet();

  /** Test if the dataset corresponds to a Jpeg2000 file format
   *  Return true if the dataset exists and has a JPEG2000 driver
   *  Return false in all other cases */
  bool IsJPEG2000() const;

  /**
   */
  int GetOverviewsCount() const;

  /**
   */
  unsigned int GetWidth() const;

  /**
   */
  unsigned int GetHeight() const;

  /**
   */
  size_t GetPixelBytes() const;

protected :
  GDALDatasetWrapper();

  ~GDALDatasetWrapper() ITK_OVERRIDE;


private:
  GDALDataset * m_Dataset;
}; // end of GDALDatasetWrapper


} // end namespace otb


#endif // otbGDALDatasetWrapper_h
