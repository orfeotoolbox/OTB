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
#ifndef __otbImageFileWriter_h
#define __otbImageFileWriter_h

#include "itkImageFileWriter.h"

namespace otb
{

/** \class ImageFileWriter
 * \brief Ressource en ecriture pour ecrire une image depuis un fichier.
 *
 * \sa ImageSeriesWriter
 * \sa ImageIOBase
 *
 * \ingroup IOFilters
 *
 */


template <class TInputImage>
class ITK_EXPORT ImageFileWriter : public itk::ImageFileWriter<TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef ImageFileWriter                               Self;
  typedef itk::ImageFileWriter<TInputImage>             Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageFileWriter,itk::ImageFileWriter);

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer InputImagePointer;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::PixelType InputImagePixelType;

  /** A special version of the Update() method for writers.  It
   * invokes start and end events and handles releasing data. It
   * eventually calls GenerateData() which does the actual writing.
   * Note: the write method will write data specified by the
   * IORegion. If not set, then then the whole image is written.  Note
   * that the region will be cropped to fit the input image's
   * LargestPossibleRegion. */
  virtual void Write(void);


  virtual void SetFileName(std::string filename)
  {
    this->Superclass::SetFileName(filename);
    this->SetImageIO(NULL);
  }
protected:
  ImageFileWriter();
  ~ImageFileWriter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageFileWriter(const Self&); //purposely not implemented
  void operator=(const Self&);  //purposely not implemented

  bool m_UserSpecifiedIORegion;   //track whether the region is user specified
  bool m_FactorySpecifiedImageIO; //track whether the factory mechanism set the ImageIO


};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFileWriter.txx"
#endif

#endif // __otbImageFileWriter_h

