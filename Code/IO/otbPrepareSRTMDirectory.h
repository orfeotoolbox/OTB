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
#ifndef __otbPrepareSRTMDirectory_h
#define __otbPrepareSRTMDirectory_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "base/ossimFilename.h"

namespace otb
{

/**
   * \class PrepareSRTMDirectory
   * \brief Prepare SRTM directory for Ossim from a full archive
   *
   *  As ossim has some issue with directories containing too many SRTM tiles
   * (>5000) this class enable to copy only the required tiles from
   * FullDEMDirectoryPath containing all the SRTM tiles to DEMDirectoryPath
   * that will be passed as the argument of the SetDEMDirectoryPath() of
   * the otb::DEMToImageGenerator for example
   *
   *
 */


  class ITK_EXPORT PrepareSRTMDirectory : public itk::Object
  {
    public:
      /** Standard class typedefs. */
      typedef PrepareSRTMDirectory                                      Self;
      typedef itk::SmartPointer<Self>                                Pointer;
      typedef itk::SmartPointer<const Self>                          ConstPointer;


      typedef itk::Object Superclass;

      itkTypeMacro(PrepareSRTMDirectory, Object);
      /** Method for creation through the object factory. */
      itkNewMacro(Self);

      itkGetMacro( ULLon, double );
      itkGetMacro( ULLat, double );
      itkGetMacro( LRLon, double );
      itkGetMacro( LRLat, double );
      itkGetMacro( FullDEMDirectoryPath, string );
      itkGetMacro( DEMDirectoryPath, string );

      itkSetMacro( ULLon, double );
      itkSetMacro( ULLat, double );
      itkSetMacro( LRLon, double );
      itkSetMacro( LRLat, double );
      itkSetMacro( FullDEMDirectoryPath, string );
      itkSetMacro( DEMDirectoryPath, string );


      virtual bool Evaluate();

    protected:
      PrepareSRTMDirectory();
      ~PrepareSRTMDirectory(){};
      void PrintSelf(std::ostream& os, itk::Indent indent) const;

    private:
      PrepareSRTMDirectory( const Self& ); //purposely not implemented
      void operator=( const Self& ); //purposely not implemented

      double m_ULLon;
      double m_ULLat;
      double m_LRLon;
      double m_LRLat;
      string m_FullDEMDirectoryPath;
      string m_DEMDirectoryPath;
  };

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPrepareSRTMDirectory.txx"
#endif

#endif
