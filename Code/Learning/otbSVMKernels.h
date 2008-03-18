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
#ifndef _otbSVMKernels_h
#define _otbSVMKernels_h

#include "itkNumericTraits.h"
#include "itkImageRegionIterator.h"

#include "svm.h"
#include <vector>

namespace otb
{
class CustomKernelFunctor : public GenericKernelFunctorBase
  {
    public:
  CustomKernelFunctor(): GenericKernelFunctorBase() { this->SetName("custom"); };
  virtual ~CustomKernelFunctor() {};
  
  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      return ( dot(x, x) - 2.0 * dot( x, y) + dot(y, y) );
    }
  };


class InvMultiQuadricKernelFunctor : public GenericKernelFunctorBase
  {
    public:
  InvMultiQuadricKernelFunctor(): GenericKernelFunctorBase() 
    {
      this->SetName("invMultiQuadric"); 
      this->SetValue<double>("const_coef", 1);
    };
  virtual ~InvMultiQuadricKernelFunctor() {};
  
  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      CustomKernelFunctor custom;
      double mq = this->GetValue<double>("const_coef") + custom(x, y, param );
      if ( mq == 0.0 )
	{
	  return itk::NumericTraits<double>::max();
	}
      return 1.0 / sqrt( mq );
    }
  };


class KModKernelFunctor : public GenericKernelFunctorBase
  {
    public:
  KModKernelFunctor(): GenericKernelFunctorBase() 
    { 
      this->SetName("KMod"); 
      this->SetValue<double>("const_coef", 1);
    };
  virtual ~KModKernelFunctor() {};
  
  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      CustomKernelFunctor custom;
      double mq = this->GetValue<double>("const_coef") + custom(x, y, param);
      
      if ( mq == 0.0 )
	{
	  return itk::NumericTraits<double>::max();
	}
      return exp( param.gamma / mq ) - 1.0;
    }
  };


class SAMKernelFunctor : public GenericKernelFunctorBase
  {
    public:
  SAMKernelFunctor(): GenericKernelFunctorBase() { this->SetName("SAM"); };
  virtual ~SAMKernelFunctor() {};
  
  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      double den = dot(x, x) * dot(y, y);
      if ( den <= 0. )
	{
	  return 0.0;
	}
      double ss = dot( x, y );
      return vcl_acos( ss / vcl_sqrt( den ) );
    }
  };


class RadialSAMKernelFunctor : public GenericKernelFunctorBase
  {
    public:
    RadialSAMKernelFunctor(): GenericKernelFunctorBase() { this->SetName("RadialSAM"); };
    virtual ~RadialSAMKernelFunctor() {};
    
    virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
      {
	SAMKernelFunctor sam;
	return vcl_exp( - param.gamma * sam( x, y, param ) );
      }
  };


class InvMultiQuadraticSAMKernelFunctor : public GenericKernelFunctorBase
  {
    public:
  InvMultiQuadraticSAMKernelFunctor(): GenericKernelFunctorBase() 
    { 
      this->SetName("invMultiQuadraticSAM"); 
      this->SetValue<double>("const_coef", 1);
    };
  virtual ~InvMultiQuadraticSAMKernelFunctor() {};
  
  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      SAMKernelFunctor sam;
      double mq = this->GetValue<double>("const_coef") + sam( x, y, param );
      
      if ( mq == 0. )
	{
	  return itk::NumericTraits<double>::max();
	}
      return 1. / sqrt( mq );
    }
  };


class KModSAMKernelFunctor : public GenericKernelFunctorBase
  {
  public:
    KModSAMKernelFunctor(): GenericKernelFunctorBase() 
      { 
	this->SetName("KModSAM"); 
	this->SetValue<double>("const_coef", 1);
      };
    virtual ~KModSAMKernelFunctor() {};
    
    virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
      {
	SAMKernelFunctor sam;
	double mq = this->GetValue<double>("const_coef") + sam( x, y, param );
	
	if ( mq == 0. )
	  {
	    return itk::NumericTraits<double>::max();
	  }
	return vcl_exp( param.gamma / mq ) - 1.0;
      }
  };

class RBFKernelFunctor : public GenericKernelFunctorBase
  {
  public:
    RBFKernelFunctor(): GenericKernelFunctorBase() 
      { 
	this->SetName("RBF"); 
	this->SetValue<double>("gamma_coef", 0.5);
      };
    virtual ~RBFKernelFunctor() {};
    
    virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
      {
	CustomKernelFunctor custom;
	double res = this->GetValue<double>("gamma_coef") * custom( x, y, param );
	
	return vcl_exp(-res);
      }
    
    virtual double derivative(const svm_node *x, const svm_node *y, const svm_parameter& param, int degree, int index = 0, bool isAtEnd = false, double constValue)const 
      {
	double gamma = this->GetValue<double>("gamma_coef");
	double kernelValue = 0.;
	double xval = 0.;
	double yval = 0.;
	int compt = 0;

	const svm_node *xtemp = x;
	const svm_node *ytemp = y;

	bool stop = false;
	while(xtemp->index != -1 && ytemp->index != -1 && stop == false)
	  {
	    if(xtemp->index == ytemp->index)
	      {
		if (compt == index)
		  {
		    xval = xtemp->value;
		    yval = ytemp->value;
		    stop = true;
		  }
		else
		  {
		    compt++;
		    ++ytemp;
		    ++xtemp;
		  }
	      }
	    else
	      {
		if(xtemp->index > ytemp->index)
		  ++ytemp;
		else
		  ++xtemp;
	      }			
	  }

	if (isAtEnd == true)
	  {
	    kernelValue = this->operator()(x, y, param);
	  }
	else
	  {
	    kernelValue = constValue;
	  }
	
	if (degree < 0)
	  {
	    return 0;
	  }
	switch (degree)
	  {
	  case 0: 
	    return kernelValue; 
	    break;
	  case 1: 
	    return (-2*gamma*(yval - xval)*kernelValue); 
	    break;
	  default: 
	    return (-2*gamma*((degree - 1) * this->derivative(x, y, param, degree-2, index, isAtEnd, constValue) + (yval - xval)* derivative(x, y, param, degree-1, index, isAtEnd, constValue))); 
	    break;
	  }
	
      }



  };
 
class RBFRBFSAMKernelFunctor : public GenericKernelFunctorBase
  {
    public:
  RBFRBFSAMKernelFunctor(): GenericKernelFunctorBase() 
    { 
      this->SetName("RBFRBFSAM"); 
      this->SetValue<double>("lin_coef", 1.);
    };
  virtual ~RBFRBFSAMKernelFunctor() {};
  
  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      CustomKernelFunctor    custom;
      RadialSAMKernelFunctor radialSam;
      return ( this->GetValue<double>("lin_coef") * vcl_exp( -param.gamma * custom(x, y, param) )
	+ ( 1.0 - this->GetValue<double>("lin_coef") ) * radialSam( x, y, param ) );
    }
  };


class PolyRBFSAMKernelFunctor : public GenericKernelFunctorBase
  {
    public:
  PolyRBFSAMKernelFunctor(): GenericKernelFunctorBase() 
    {
      this->SetName("polyRBFSAM");
      this->SetValue<double>("const_coef", 1);
      this->SetValue<double>("lin_coef", 1);
    };
  virtual ~PolyRBFSAMKernelFunctor() {};
  
  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      RadialSAMKernelFunctor radialSam;
      return this->GetValue<double>("const_lin") * vcl_pow( dot(x, y)+this->GetValue<double>("const_coef") , param.degree)
	+ ( 1.0 - this->GetValue<double>("const_coef") )
	* radialSam( x, y, param );
    }
  };


class RBFDiffKernelFunctor : public GenericKernelFunctorBase
  {
    public:
  RBFDiffKernelFunctor(): GenericKernelFunctorBase() { this->SetName("RBFDiff"); };
  virtual ~RBFDiffKernelFunctor() {};
  
  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {

      double total = 0.;
     
       while (x->index != -1 && y->index != -1)
	{
	  if (x->index == y->index)
	    {
	      total = total +  vcl_exp(-param.gamma*(x->value - y->value));
	      ++x;
	      ++y;
	    }
	  else 
	    {
	      if(x->index < y->index)
		{
		  ++x;
		}
	      else
		{
		  ++y;
		}
	    }
	}
       return total;
    }
  };
 

class CustomLinearKernelFunctor : public GenericKernelFunctorBase
  {
  public:
  CustomLinearKernelFunctor(): GenericKernelFunctorBase() { this->SetName("customLinear"); };
  virtual ~CustomLinearKernelFunctor() {};
  
  virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      CustomKernelFunctor custom;
      return( vcl_exp(-param.gamma*custom(x, y, param)) ); 
    }
  };


class GroupedRBFKernelFunctor : public GenericKernelFunctorBase
  {
  public:
    GroupedRBFKernelFunctor(): GenericKernelFunctorBase() { this->SetName("groupedRBF"); };
    virtual ~GroupedRBFKernelFunctor() {};
       
    virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
      {
      const char *parameters = param.custom;
      const char * twoPoints = ":";
      const char * position = strpbrk (parameters, twoPoints);
      int twoPointsPosition = 0;
      double total = 0.;
      double value = 0.;
      int numberOfGroups = atoi(parameters);
      
      
      twoPointsPosition = position - parameters;
      
      parameters = parameters + twoPointsPosition + 1;
      
      int i,j;
      std::vector<int> begin;
      begin.resize(numberOfGroups);
      std::vector<int> end;
      end.resize(numberOfGroups);
      
      for (i = 0; i < numberOfGroups; i++)
	{
	  begin[i] = atoi(parameters);
	  position = strpbrk (parameters, twoPoints);
	  twoPointsPosition = position - parameters;
	  parameters = parameters + twoPointsPosition + 1;
	  
	  end[i] = atoi(parameters);
	  parameters = parameters + twoPointsPosition + 1;
	}
      
     
     const svm_node * xBuff = x;
     const svm_node * yBuff = y;
           
      int sizeX = 0;
      int sizeY = 0;
      
      while (xBuff->index != 1)
	{
	  sizeX++;
	  ++xBuff;
	}
      
      while (yBuff->index != 1)
	{
	  sizeY++;
	  ++yBuff;
	}
      
      
      const svm_node * xTemp = NULL;
      const svm_node * yTemp = NULL;
      
      int index = 0;
      
      
      if (sizeX && sizeY)
	{
		svm_node* xGroup = new svm_node[sizeX];
		svm_node* yGroup = new svm_node[sizeY];
	  for (j = 0; j < numberOfGroups; j++)
	    {
	      xTemp = x;
	      yTemp = y;
	      index = 0;
	      
	      while (xTemp->index != 1 && yTemp->index != 1)
		{
		  xGroup[index].index = xTemp->index;
		  yGroup[index].index = yTemp->index;
		  if((xTemp->index < begin[j]) || (xTemp->index > end[j]))
		    {
		      xGroup[index].value = 0;
		      yGroup[index].value = 0;
		    }
		  else
		    {
		      xGroup[index].value = xTemp->value;
		      yGroup[index].value = yTemp->value;
		    }
		  ++index;
		  ++xTemp;	
		  ++yTemp;
		}
	  
	      // value can have different value according to j
	      CustomLinearKernelFunctor customLinear;	
	      value =  customLinear(xGroup, yGroup, param); 
	    
	      total += value;
	    }
		delete [] xGroup;
		delete [] yGroup;
	}
      
      
      else if ((sizeX > 0) && (sizeY == 0))
	{
		svm_node* xGroup = new svm_node[sizeX];
		svm_node* yGroup = new svm_node[sizeY];
	  
	  for (j = 0; j < numberOfGroups; j++)
	    {
	      xTemp = x;
	      index = 0;
	      
	      while (xTemp->index != -1)
		{
		  xGroup[index].index = xTemp->index;
		  yGroup[index].index = xTemp->index;
		  
		  if((xTemp->index < begin[j]) || (xTemp->index > end[j]))
		    {
		      xGroup[index].value = 0;
		      yGroup[index].value = 0;
		    }
		  else
		    {
		      xGroup[index].value = xTemp->value;
		      yGroup[index].value = 0;
		    }
		  ++index;		
		  ++xTemp;
		}
	
	       // value can have different value according to j
	      CustomLinearKernelFunctor customLinear;	
	      value =  customLinear(xGroup, yGroup, param); 
	     	      
	      total += value;	
	    }
		delete [] xGroup;
		delete [] yGroup;
	}
      
      
      
      else
	{ 
	  CustomLinearKernelFunctor customLinear;	
	  total =  static_cast<double>(numberOfGroups) * customLinear(x, y, param);
	}
      
      if(xTemp != NULL)
	delete xTemp;
      if(yTemp != NULL)
	delete yTemp;

      return total;
    }
  };
 




class GroupingAdaptiveKernelFunctor : public GenericKernelFunctorBase
  {
  public:
    GroupingAdaptiveKernelFunctor(): GenericKernelFunctorBase() 
      { 
	this->SetName("groupingAdaptive"); 
	this->SetValue<double>("lin_coef", 1.);
	this->SetValue<double>("const_coef", 1.);
      };
    virtual ~GroupingAdaptiveKernelFunctor() {};
    
    virtual double operator()(const svm_node *x, const svm_node *y, const svm_parameter& param)const
      {
      const char * parameters = param.custom;
      const char * twoPoints = ":";
      const char * position = strpbrk (parameters, twoPoints);
      int twoPointsPosition = 0;
      double total = 0.;
      double value = 0.;
      int numberOfGroups = atoi(parameters);
      
      
      twoPointsPosition = position - parameters;
      
      parameters = parameters + twoPointsPosition + 1;
      
      int i,j;
      
	  std::vector<int> begin;
	  begin.resize(numberOfGroups);
	  std::vector<int> end;
	  end.resize(numberOfGroups);
      
      for (i = 0; i < numberOfGroups; i++)
	{
	  begin[i] = atoi(parameters);
	  position = strpbrk (parameters, twoPoints);
	  twoPointsPosition = position - parameters;
	  parameters = parameters + twoPointsPosition + 1;
	  
	  end[i] = atoi(parameters);
	  parameters = parameters + twoPointsPosition + 1;
	}
      
     
     const svm_node * xBuff = x;
     const svm_node * yBuff = y;
           
     int sizeX = 0;
     int sizeY = 0;
     
     while (xBuff->index != 1)
       {
	 sizeX++;
	 ++xBuff;
       }
     
     while (yBuff->index != 1)
       {
	 sizeY++;
	 ++yBuff;
       }
     
     
     const svm_node * xTemp = NULL;
     const svm_node * yTemp = NULL;
     
     int index = 0;
     
     
     if (sizeX && sizeY)
       {
		svm_node* xGroup = new svm_node[sizeX];
		svm_node* yGroup = new svm_node[sizeY];
	 
	 for (j = 0; j < numberOfGroups; j++)
	   {
	     xTemp = x;
	     yTemp = y;
	     index = 0;   
	     
	     while (xTemp->index != 1 && yTemp->index != 1)
	       {
		 xGroup[index].index = xTemp->index;
		 yGroup[index].index = yTemp->index;
		 if((xTemp->index < begin[j]) || (xTemp->index > end[j]))
		   {
		     xGroup[index].value = 0;
		     yGroup[index].value = 0;
		   }
		 else
		   {
		     xGroup[index].value = xTemp->value;
		     yGroup[index].value = yTemp->value;
		   }
		 ++index;
		 ++xTemp;	
		 ++yTemp;
	       }		
	     
	     // value can have different value according to j
	     value =  vcl_pow(this->GetValue<double>("lin_coef")*dot(xGroup,yGroup)+this->GetValue<double>("const_coef"),static_cast<double>(param.degree)); 
	     
	     total += value;
	   }
 		delete [] xGroup;
		delete [] yGroup;
      }
     
	
     else if ((sizeX > 0) && (sizeY == 0))
       {
		svm_node* xGroup = new svm_node[sizeX];
		svm_node* yGroup = new svm_node[sizeY];
	 
	 for (j = 0; j < numberOfGroups; j++)
	   {
	     xTemp = x;
	     index = 0;
	     
	     while (xTemp->index != 1)
	       {
		 xGroup[index].index = xTemp->index;
		 yGroup[index].index = xTemp->index;
		 
		 if((xTemp->index < begin[j]) || (xTemp->index > end[j]))
		   {
		     xGroup[index].value = 0;
		     yGroup[index].value = 0;
		   }
		 else
		   {
		     xGroup[index].value = xTemp->value;
		     yGroup[index].value = 0;
		   }
		 ++index;		
		 ++xTemp;
	       }
	     
	     // value can have different value according to j
	     value = vcl_pow(this->GetValue<double>("lin_coef")*dot(xGroup,yGroup)+this->GetValue<double>("const_coef"),static_cast<double>(param.degree));
	       
	     total += value;	
	   }
		delete [] xGroup;
		delete [] yGroup;
       }
        
     else
       {
	 CustomKernelFunctor custom;
	 total = static_cast<double>(numberOfGroups) * custom(x, y, param);
       }
     
     return total;
      }
  };
 
}
#endif
