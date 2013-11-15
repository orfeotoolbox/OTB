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

#ifndef __otbLabelToSimulationParametersBase_h
#define __otbLabelToSimulationParametersBase_h


#include "itkProcessObject.h"
#include "itkArray.h"

namespace otb
{

/**
 * \class LabelToSimulationParametersBase
 * \brief Base class for all classes that take a label and outputs two set of parameters for two steps (step1, step2)
 *
 * step1 and 2 refers to PROSPECT and SAIL parameters.
 *
 * Prospect Parameters
 * Cab    = 30;   % chlorophyll content (ug.cm-2)
 *  Car   = 10;   % carotenoid content (ug.cm-2)
 * Cbrown  = 0.0;  % brown pigment content (arbitrary units)
 * Cw    = 0.015;  % EWT (cm)
 * Cm    = 0.009;  % LMA (g.cm-2)
 * N   = 1.2;  % structure coefficient
 *
 * Sail Parameters
 * lai   = 2;    % leaf area index
 * angl  = 50;   % average leaf angle (deg)
 * psoil = 1;    % soil coefficient
 * skyl  = 70;   % % diffuse/direct radiation
 * hspot = 0.2;  % hot spot
 * ihot  = 1.0;  %
 * tts   = 30;   % solar zenith angle (deg)
 * tto   = 0;    % observer zenith angle (deg)
 * psi   = 0;    % azimuth (deg)
 *
 * \sa ProcessObject
 */

template <class TLabel>
class ITK_EXPORT LabelToSimulationParametersBase : public itk::ProcessObject
{
public:

   /**Standard "Self" & Superclass typedef*/
   typedef LabelToSimulationParametersBase     Self;
   typedef itk::ProcessObject                  Superclass;
   typedef itk::SmartPointer<Self>             Pointer;
   typedef itk::SmartPointer<const Self>       ConstPointer;

   /** Some convenient typedefs. */
   typedef TLabel                                LabelType;
   typedef double                                ParametersValueType;
   typedef itk::Array<ParametersValueType>       ParametersType;


   /** Standard Macro*/
   itkTypeMacro(LabelToSimulationParametersBase, ProcessObject);

   itkSetMacro(Label, LabelType);
   itkGetMacro(Label, LabelType);

   itkSetMacro(Step1Parameters, ParametersType);
   itkGetMacro(Step1Parameters, ParametersType);

   itkSetMacro(Step2Parameters, ParametersType);
   itkGetMacro(Step2Parameters, ParametersType);

   //virtual const ParametersType & GetStep1Parameters() = 0;
   //virtual const ParametersType & GetStep2Parameters() = 0;

protected:
   LabelToSimulationParametersBase(){};
   virtual ~LabelToSimulationParametersBase() {}



private:
   LabelToSimulationParametersBase(const Self &); //purposely not implemented
   void operator =(const Self&); //purposely not implemented

   ParametersType m_Step1Parameters;
   ParametersType m_Step2Parameters;
   LabelType m_Label;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif
