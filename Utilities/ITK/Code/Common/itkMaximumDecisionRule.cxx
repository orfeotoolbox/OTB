/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMaximumDecisionRule.cxx,v $
  Language:  C++
  Date:      $Date: 2009-02-05 22:04:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkMaximumDecisionRule.h"

namespace itk{ 

void 
MaximumDecisionRule::PrintSelf(std::ostream& os, Indent indent) const 
{ 
  Superclass::PrintSelf(os, indent); 
}
 

unsigned int 
MaximumDecisionRule::Evaluate(const VectorType &discriminantScores) const
{
  double max = discriminantScores[0];
  unsigned int maxIndex = 0;
  unsigned int i;
  for (i = 1; i < discriminantScores.size(); i++)
    {
    if (discriminantScores[i] > max) 
      {
      max = discriminantScores[i];
      maxIndex = i;
      }
    }
  return maxIndex;
}

 
unsigned int 
MaximumDecisionRule::Evaluate(const ArrayType &discriminantScores) const
{
  double max = discriminantScores[0];
  unsigned int maxIndex = 0;
  unsigned int i;
  for (i = 1; i < discriminantScores.Size(); i++)
    {
    if (discriminantScores[i] > max) 
      {
      max = discriminantScores[i];
      maxIndex = i;
      }
    }
  return maxIndex;
}

unsigned int 
MaximumDecisionRule::Evaluate(const VariableLengthVectorType &discriminantScores) const
{
  double max = discriminantScores[0];
  unsigned int maxIndex = 0;
  unsigned int i;
  for (i = 1; i < discriminantScores.Size(); i++)
    {
    if (discriminantScores[i] > max) 
      {
      max = discriminantScores[i];
      maxIndex = i;
      }
    }
  return maxIndex;
}


} // end of namespace itk
