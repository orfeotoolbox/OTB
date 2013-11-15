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
#ifndef __otbPointSetFileReader_h
#define __otbPointSetFileReader_h

#include "otbPointSetSource.h"
#include "otbImageFileReader.h"

namespace otb
{
/**
 * \class PointSetFileReader
 * \brief Read a point set from a lidar file.
 *
 * \sa PointSetSource
 *
 * \ingroup IOFilters
 *
 */

template<class TOutputPointSet>
class ITK_EXPORT PointSetFileReader : public otb::PointSetSource<TOutputPointSet>
{
public:
  /** Standard class typedefs. */
  typedef PointSetFileReader                   Self;
  typedef otb::PointSetSource<TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>              Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetFileReader, PointSetSource);

  /** Typedef */
  typedef TOutputPointSet                  PointSetType;
  typedef typename PointSetType::PointType PointType;
  typedef typename PointSetType::PixelType PixelType;

  /** Specify the file to read. This is forwarded to the IO instance. */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

  /** Get Macro*/
  itkGetMacro(NumberOfPoints, long int);
  itkGetMacro(MinX, double);
  itkGetMacro(MaxX, double);
  itkGetMacro(MinY, double);
  itkGetMacro(MaxY, double);

  virtual void GenerateOutputInformation(void);

  /** Does the real work. */
  virtual void GenerateData();

protected:
  PointSetFileReader();
  virtual ~PointSetFileReader();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void TestFileExistanceAndReadability();

  std::string m_FileName; // The file to be read
  long int    m_NumberOfPoints;
  double      m_MinX;
  double      m_MaxX;
  double      m_MinY;
  double      m_MaxY;

private:
  PointSetFileReader(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  std::string m_ExceptionMessage;

};

} //namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPointSetFileReader.txx"
#endif

#endif
