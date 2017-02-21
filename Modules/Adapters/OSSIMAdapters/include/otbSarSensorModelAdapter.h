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
#ifndef otbSarSensorModelAdapter_h
#define otbSarSensorModelAdapter_h

#include "otbDEMHandler.h"

namespace ossimplugins
{
class ossimSarSensorModel;
}

namespace otb
{

class ImageKeywordlist;

/**
 * \class SarSensorModelAdapter
 * \brief Wrapper class to access SarSensorModel specific methods
 *
 * This class is NOT intended to be used outside of OTB. Use the
 * InverseSensorModel and ForwardSensorModel. If you feel that you need to use
 * it directly, think again!
 *
 * \sa InverseSensorModel
 * \sa ForwardSensorModel
 * \ingroup Projection
 *
 *
 * \ingroup OTBOSSIMAdapters
 **/

class OTBOSSIMAdapters_EXPORT SarSensorModelAdapter: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef SarSensorModelAdapter         Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef ossimplugins::ossimSarSensorModel* InternalModelPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SarSensorModelAdapter, itk::Object);

  /** Load model from keyword list */
  bool LoadState(const ImageKeywordlist& image_kwl);

  /** Save keyword list from model */
  bool SaveState(ImageKeywordlist & image_kwl);
  
  /** Is sensor model valid method. return false if the m_SensorModel is null*/
  bool IsValidSensorModel() const;

  /** Deburst metadata if possible and return lines to keep in image file */
  bool Deburst(std::vector<std::pair<unsigned long, unsigned long>> & lines);

  static bool ImageLineToDeburstLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines, unsigned long imageLine, unsigned long & deburstLine);

  static void DeburstLineToImageLine(const std::vector<std::pair<unsigned long,unsigned long> >& lines, unsigned long deburstLine, unsigned long & imageLine);

  
protected:
  SarSensorModelAdapter();
  virtual ~SarSensorModelAdapter() ITK_OVERRIDE;

private:
  SarSensorModelAdapter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  InternalModelPointer m_SensorModel;
};

} // namespace otb

#endif
