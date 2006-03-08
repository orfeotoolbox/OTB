/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkRegularStepGradientDescent.h,v $
  Language:  C++
  Date:      $Date: 2002/06/18 15:53:32 $
  Version:   $Revision: 1.5 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef fltkREGULARSTEPGRADIENTDESCENT
#define fltkREGULARSTEPGRADIENTDESCENT

#include <fltkRegularStepGradientDescentGUI.h>
#include <itkRegularStepGradientDescentOptimizer.h>


namespace fltk {


/**
 *  This class implements a GUI to an itkRegularStepGradientDescent 
 *  optimization method.
 *
 *  It derives from fltkRegularStepGradientDescentGUI class which is
 *  just the graphic front end.
 *
 *  This class communicates with fltkRegularStepGradientDescentGUI 
 *  by using Command classes
 *
 */
class RegularStepGradientDescent : public RegularStepGradientDescentGUI
{

public:

  /**
   *  Type of the optimizer for which this class is a GUI
   */
  typedef  ::itk::RegularStepGradientDescentOptimizer      OptimizerType;


  /**
   *  Constructor
   */
  RegularStepGradientDescent()
  {
    m_Optimizer = OptimizerType::New();
  }
  

  /**
   *  Destructor
   */
  virtual ~RegularStepGradientDescent() {};


  /**
   *  Connect an particular optimizer
   */
  void SetOptimizer( OptimizerType * optimizer )
  { m_Optimizer = optimizer; }

  
  /**
   *  Return the optimizer
   */
  OptimizerType::Pointer GetOptimizer( void )
  { return m_Optimizer.GetPointer(); }
      

  /**
   *  Delegate the Start Optimization method to the optimizer
   */ 
  void StartOptimization( void ) 
  { m_Optimizer->StartOptimization(); }


  /**
   *  Delegate the Stop Optimization method to the optimizer
   */ 
  void StopOptimization( void ) 
  { m_Optimizer->StopOptimization(); }


  /**
   *  Delegate the Resume Optimization method to the optimizer
   */ 
  void ResumeOptimization( void ) 
  { m_Optimizer->ResumeOptimization(); }


  /**
   *  Delegate the SetMinimumStepLength method to the optimizer
   */ 
  void SetMinimumStepLength( double val ) 
  { m_Optimizer->SetMinimumStepLength( val ); }


  /**
   *  Delegate the SetMaximumStepLength method to the optimizer
   */ 
  void SetMaximumStepLength( double val ) 
  { m_Optimizer->SetMaximumStepLength( val ); }


  /**
   *  Delegate the SetGradientMagnitudeTolerance method to the optimizer
   */ 
  void SetGradientMagnitudeTolerance( double val ) 
  { m_Optimizer->SetGradientMagnitudeTolerance( val ); }


  /**
   *  Delegate the SetMaximumNumberOfIterations method to the optimizer
   */ 
  void SetNumberOfIterations( unsigned long val ) 
  { m_Optimizer->SetNumberOfIterations( val ); }



private:

  OptimizerType::Pointer   m_Optimizer;
    
};

} // end namespace fltk


#endif



