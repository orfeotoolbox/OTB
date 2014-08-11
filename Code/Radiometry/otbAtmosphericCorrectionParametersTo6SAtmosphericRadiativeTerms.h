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
#ifndef __otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms_h
#define __otbAtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms_h

#include "otbMacro.h"
#include "itkProcessObject.h"
#include "otbAtmosphericCorrectionParameters.h"
#include "otbAtmosphericRadiativeTerms.h"

namespace otb
{
/**
 * \class AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
 * \brief This class computes the atmospheric radiative terms with 6S.
 *
 * It enables to compute a AtmosphericRadiativeTerms from a AtmosphericCorrectionParameters,
 * which is used in the ReflectanceToSurfaceReflectanceImageFilter.
 *
 * \sa AtmosphericRadiativeTerms
 * \sa AtmosphericCorrectionParameters
 * \sa ReflectanceToSurfaceReflectanceImageFilter
 * \ingroup DataSources
 * \ingroup Radiometry
 * \deprecated This class has been replaced by
 *  otb::RadiometryCorrectionParametersToAtmosphericRadiativeTerms,
 *  which only contains a static function that computes the radiative terms.
 */
class ITK_EXPORT AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms
  : public itk::ProcessObject
{
public:
  /** Standard typedefs */
  typedef AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms Self;
  typedef itk::ProcessObject                                           Superclass;
  typedef itk::SmartPointer<Self>                                      Pointer;
  typedef itk::SmartPointer<const Self>                                ConstPointer;

  /** Runtime information */
  itkTypeMacro(AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms, itk::ProcessObject);
  /** Creation through the object factory */
  itkNewMacro(Self);
  /** Template parameters typedefs */
  typedef AtmosphericCorrectionParameters              AtmosphericCorrectionParametersType;
  typedef AtmosphericCorrectionParametersType::Pointer AtmosphericCorrectionParametersPointer;
  typedef AtmosphericRadiativeTerms                    AtmosphericRadiativeTermsType;
  typedef AtmosphericRadiativeTermsType::Pointer       AtmosphericRadiativeTermsPointer;

  /** Set the Atmospheric Correction Parameters input of this process object */
  void SetInput(const AtmosphericCorrectionParametersType *object);

  /** Get the Atmospheric Correction Parameters input of this process object */
  AtmosphericCorrectionParametersType * GetInput(void);

  DataObjectPointer MakeOutput(unsigned int);
  void GraftOutput(itk::DataObject *graft);
  void GraftNthOutput(unsigned int idx, itk::DataObject *graft);

  /** Get the Atmospheric Radiative Terms output of this process object.  */
  virtual AtmosphericRadiativeTermsType * GetOutput(void);
  virtual AtmosphericRadiativeTermsType * GetOutput(unsigned int idx);

  /** Generate the output.*/
  virtual void GenerateData();

protected:
  /** Constructor */
  AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms();
  /** Destructor */
  virtual ~AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms() {}
  /** PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
