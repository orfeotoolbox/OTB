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
#ifndef __otbImportGeoInformationImageFilter_h
#define __otbImportGeoInformationImageFilter_h

#include "itkCastImageFilter.h"

namespace otb
{
/** \class ImportGeoInformationImageFilter
 *  \brief This filter is a helper class to import metadata
 *  from an existing image into a non-georeferenced image.
 *
 *  It derives from itk::CastImageFilter since it overwrites
 *  its output if possible. Default behavior is InPlaceOn as
 *  the other filters which only modify metadata information
 *  This class has been written as a workaround
 *  for the bug http://public.kitware.com/Bug/bug.php?op=show&bugid=4625&pos=0.
 *  There should be no needs for this filter in a standard pipeline.
 *
 * \sa InPlaceImageFilter
 * \sa ImageBase
 */
template <class TImage, class TSourceImage>
class ITK_EXPORT ImportGeoInformationImageFilter
  : public itk::CastImageFilter<TImage, TImage>
{
public:
  /** Standard typedefs */
  typedef ImportGeoInformationImageFilter      Self;
  typedef itk::CastImageFilter<TImage, TImage> Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImportGeoInformationImageFilter, CastImageFilter);

  /** Template parameters typedefs */
  typedef TImage                           ImageType;
  typedef typename ImageType::Pointer      ImagePointerType;
  typedef typename ImageType::ConstPointer ImageConstPointerType;
  typedef TSourceImage                     SourceImageType;
  typedef SourceImageType                  SourceImagePointerType;

  /**
   * Set the source for geo information.
   * \param source The source image.
   */
  void SetSource(const TSourceImage * source);
  /**
   * Get the source for geo information.
   * \return The source image.
   */
  const TSourceImage * GetSource(void);

protected:
  /** Constructor */
  ImportGeoInformationImageFilter();
  /** Destructor */
  virtual ~ImportGeoInformationImageFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** Generate input requested region */
  virtual void GenerateInputRequestedRegion(void);
  /** Generate output information */
  virtual void GenerateOutputInformation(void);

  /** Override VerifyInputInformation() since this filter's inputs do
   * not need to occupy the same physical space.
   *
   * \sa ProcessObject::VerifyInputInformation
   */
  virtual void VerifyInputInformation() {}

private:
  ImportGeoInformationImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImportGeoInformationImageFilter.txx"
#endif

#endif
