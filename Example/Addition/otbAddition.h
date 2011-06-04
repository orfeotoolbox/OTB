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

#include "otbWrapperApplication.h"


namespace otb
{
namespace Wrapper
{

class ITK_EXPORT Addition : public Application
{
public:
  /** Standard class typedefs. */
  typedef Addition                      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Self, otb::Application);

protected:
  Addition();

  virtual ~Addition();

private:
  void DoCreateParameters();

  void DoUpdateParameters();

  void DoExecute();

};

}

}

extern "C"
{
  itk::ObjectFactoryBase* itkLoad();
}
