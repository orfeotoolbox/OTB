//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include <otb/HermiteInterpolator.h>

#include <string>
#include <cassert>
#include <cmath>

namespace ossimplugins
{


HermiteInterpolator::HermiteInterpolator():
  theNPointsAvailable(0),
  theXValues(NULL),
  theYValues(NULL),
  thedYValues(NULL),
  prodC(NULL),
  sumC(NULL),
  isComputed(false)
{
}

HermiteInterpolator::HermiteInterpolator(int nbrPoints, double* x, double* y, double* dy):
  theNPointsAvailable(nbrPoints),
   prodC(NULL),
   sumC(NULL),
  isComputed(false)
{
  if(x != NULL)
  {
    theXValues = new double[theNPointsAvailable];
    for (int i=0;i<theNPointsAvailable;i++)
    {
      theXValues[i] = x[i];
    }
  }
  else
  {
    theXValues = NULL;
  }

  if(y != NULL)
  {
    theYValues = new double[theNPointsAvailable];
    for (int i=0;i<theNPointsAvailable;i++)
    {
      theYValues[i] = y[i];
    }
  }
  else
  {
    theYValues = NULL;
  }

  if(dy != NULL)
  {
    thedYValues = new double[theNPointsAvailable];
    for (int i=0;i<theNPointsAvailable;i++)
    {
      thedYValues[i] = dy[i];
    }
  }
  else
  {
    thedYValues = NULL;
  }

  for (int i = 1 ; i < theNPointsAvailable ; i++)
  {
    /**
     * @todo Verifier que l'interpolateur n'ai pas besoin ques les abscisses soitent strictement croissantes
     */

    /*
     * Les abscisses ne sont pas croissantes
     */
//    if (theXValues[i] <= theXValues[i-1])
//      std::cerr << "WARNING: Hermite interpolation assumes increasing x values" << std::endl;
    assert(theXValues && theXValues[i] > theXValues[i-1]);
  }
}

HermiteInterpolator::~HermiteInterpolator()
{
  Clear();
}

HermiteInterpolator::HermiteInterpolator(const HermiteInterpolator& rhs):
  theNPointsAvailable(rhs.theNPointsAvailable),
  prodC(NULL),
  sumC(NULL),
  isComputed(false)
{
  if(rhs.theXValues != NULL)
  {
    theXValues = new double[theNPointsAvailable];
    for (int i=0;i<theNPointsAvailable;i++)
    {
      theXValues[i] = rhs.theXValues[i];
    }
  }
  else
  {
    theXValues = NULL;
  }

  if(rhs.theYValues != NULL)
  {
    theYValues = new double[theNPointsAvailable];
    for (int i=0;i<theNPointsAvailable;i++)
    {
      theYValues[i] = rhs.theYValues[i];
    }
  }
  else
  {
    theYValues = NULL;
  }

  if(rhs.thedYValues != NULL)
  {
    thedYValues = new double[theNPointsAvailable];
    for (int i=0;i<theNPointsAvailable;i++)
    {
      thedYValues[i] = rhs.thedYValues[i];
    }
  }
  else
  {
    thedYValues = NULL;
  }
}

HermiteInterpolator& HermiteInterpolator::operator =(const HermiteInterpolator& rhs)
{
  Clear();
  theNPointsAvailable = rhs.theNPointsAvailable;
  isComputed = false;
  if(rhs.theXValues != NULL)
  {
    theXValues = new double[theNPointsAvailable];
    for (int i=0;i<theNPointsAvailable;i++)
    {
      theXValues[i] = rhs.theXValues[i];
    }
  }
  else
  {
    theXValues = NULL;
  }

  if(rhs.theYValues != NULL)
  {
    theYValues = new double[theNPointsAvailable];
    for (int i=0;i<theNPointsAvailable;i++)
    {
      theYValues[i] = rhs.theYValues[i];
    }
  }
  else
  {
    theYValues = NULL;
  }

  if(rhs.thedYValues != NULL)
  {
    thedYValues = new double[theNPointsAvailable];
    for (int i=0;i<theNPointsAvailable;i++)
    {
      thedYValues[i] = rhs.thedYValues[i];
    }
  }
  else
  {
    thedYValues = NULL;
  }
  prodC = NULL; 
  sumC = NULL;
   
  return *this;
}

// Interpolation method for the value and the derivative
int HermiteInterpolator::Interpolate(double x, double& y, double& dy) const
{
  //NOTE assume that x is increasing

  // Not enough points to interpolate
  if (theNPointsAvailable < 2) return -1;

  y = 0.0;
  dy = 0.0;

  double epsilon = 0.0000000000001;

  //Precompute useful value if they are not available
  if (!isComputed)
  {
    Precompute();
  }

  for (int i = 0; i < theNPointsAvailable; i++)
  {
    double si = 0.0;
    double hi = 1.0;
    double ui = 0; //derivative computation
    double r = x - theXValues[i];

    // check if the point is on the list
    if ( std::abs(r) < epsilon )
    {
      y = theYValues[i];
      dy = thedYValues[i];
      return 0;
    }

    for (int j = 0; j < theNPointsAvailable; j++)
    {
      if (j != i)
      {
        hi = hi * (x - theXValues[j]);
        ui = ui + 1 / (x - theXValues[j]);//derivative computation
      }
    }
    hi *= prodC[i];
    si = sumC[i];

    double f = 1.0 - 2.0 * r * si;

    y += (theYValues[i] * f + thedYValues[i] * r) * hi * hi;

    ui *= hi;//derivative computation

    double fp = 2.0 * hi * (ui * (1.0 - 2.0 * si * r) - hi * si);//derivative computation
    double d = hi * (hi + 2.0 * r * ui);//derivative computation

    dy += fp * theYValues[i] + d * thedYValues[i];//derivative computation

  }

  return 0;
}

// Interpolation method for the value only
// this is about 5 times faster and should be used when time
// is a constraint.
int HermiteInterpolator::Interpolate(double x, double& y) const
{
  //NOTE assume that x is increasing

  // Not enough points to interpolate
  if (theNPointsAvailable < 2) return -1;

  y = 0.0;

  //Precompute useful value if they are not available
  if (!isComputed)
  {
    Precompute();
  }

  for (int i = 0; i < theNPointsAvailable; i++)
  {
    double si = 0.0;
    double hi = 1.0;
    double r = x - theXValues[i];

    for (int j = 0; j < theNPointsAvailable; j++)
    {
      if (j != i)
      {
        hi = hi * (x - theXValues[j]);
      }
    }
    hi *= prodC[i];
    si = sumC[i];

    double f = 1.0 - 2.0 * r * si;

    y += (theYValues[i] * f + thedYValues[i] * r) * hi * hi;

  }

  return 0;
}

int HermiteInterpolator::Precompute() const
{
  prodC = new double[theNPointsAvailable];
  sumC= new double[theNPointsAvailable];

  for (int i = 0; i < theNPointsAvailable; i++)
  {
    prodC[i] = 1;
    sumC[i] = 0;
    for (int j = 0; j < theNPointsAvailable; j++)
    {
      if (j != i)
      {
        double v = 1.0 / (theXValues[i] - theXValues[j]);
        prodC[i] *= v;
        sumC[i]  += v;
      }
    }
  }
  isComputed = true;
  return 0;
}

void HermiteInterpolator::Clear()
{
  if (theXValues != NULL)
  {
    delete[] theXValues;
    theXValues = NULL;
  }

  if (theYValues != NULL)
  {
    delete[] theYValues;
    theYValues = NULL;
  }

  if (thedYValues != NULL)
  {
    delete[] thedYValues;
    thedYValues = NULL;
  }

  if (prodC != NULL)
  {
    delete[] prodC;
    prodC = NULL;
  }

  if (sumC != NULL)
  {
    delete[] sumC;
    prodC = NULL;
  }
  isComputed = false;
  theNPointsAvailable = 0;
}
}
