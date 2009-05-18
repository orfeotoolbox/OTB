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
#include "otbImageIOFactory.h"

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

  /** A special SetFileName() method for setting filename and
   * invokes the creating of the specific otb::ImageIOFactory.
   */
  virtual void SetFileName(const char* filename)
  {
    if ( filename && (filename == this->GetFileName() ) )
    { 
        return;
    }
    this->Superclass::SetFileName(filename);
    this->SetImageIO( ImageIOFactory::CreateImageIO( this->GetFileName(),
                        itk::ImageIOFactory::WriteMode ) );
  }  
  virtual void SetFileName (const std::string & filename)
  { 
    this->SetFileName( filename.c_str() ); 
  } 

protected:
  ImageFileWriter(){};
  ~ImageFileWriter(){};

private:
  ImageFileWriter(const Self&); //purposely not implemented
  void operator=(const Self&);  //purposely not implemented

};

} // end namespace otb

/*
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFileWriter.txx"
#endif
*/

#endif // __otbImageFileWriter_h

