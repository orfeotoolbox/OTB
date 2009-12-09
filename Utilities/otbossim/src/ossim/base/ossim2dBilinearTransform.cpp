#include <ossim/base/ossim2dBilinearTransform.h>
#include <ossim/base/ossimLeastSquaresBilin.h>
#include <ossim/base/ossimKeywordlist.h>
#include <iostream>
RTTI_DEF1(ossim2dBilinearTransform, "ossim2dBilinearTransform", ossim2dTo2dTransform);
ossim2dBilinearTransform::ossim2dBilinearTransform()
{
   std::fill(m_coefficientsXTerm, m_coefficientsXTerm+4, 0.0);
   std::fill(m_coefficientsYTerm, m_coefficientsYTerm+4, 0.0);
   std::fill(m_inverseCoefficientsXTerm, m_inverseCoefficientsXTerm+4, 0.0);
   std::fill(m_inverseCoefficientsYTerm, m_inverseCoefficientsYTerm+4, 0.0);
   
   // setup identity
   //
   m_coefficientsXTerm[1] = 1.0;
   m_coefficientsYTerm[2] = 1.0;
   m_inverseCoefficientsXTerm[1] = 1.0;
   m_inverseCoefficientsYTerm[2] = 1.0;
}

ossim2dBilinearTransform::ossim2dBilinearTransform(const ossim2dBilinearTransform& src)
:ossim2dTo2dTransform(src)
{
   std::copy(src.m_coefficientsXTerm, src.m_coefficientsXTerm+4, m_coefficientsXTerm);
   std::copy(src.m_coefficientsYTerm, src.m_coefficientsYTerm+4, m_coefficientsYTerm);
   std::copy(src.m_inverseCoefficientsXTerm, src.m_inverseCoefficientsXTerm+4, m_inverseCoefficientsXTerm);
   std::copy(src.m_inverseCoefficientsYTerm, src.m_inverseCoefficientsYTerm+4, m_inverseCoefficientsYTerm);
}

void ossim2dBilinearTransform::setFromPoints(const ossimDpt& in1, const ossimDpt& in2, const ossimDpt& in3, const ossimDpt& in4,
                                             const ossimDpt& out1, const ossimDpt& out2, const ossimDpt& out3, const ossimDpt& out4)
{
   ossimDpt input[4];
   ossimDpt output[4];
   
   input[0] = in1;
   input[1] = in2;
   input[2] = in3;
   input[3] = in4;
   
   output[0] = out1;
   output[1] = out2;
   output[2] = out3;
   output[3] = out4;
   
   setFromPoints(&input[0], &output[0], 4);
}

void ossim2dBilinearTransform::setFromPoints(const ossimDpt* input,
                                             const ossimDpt* output, 
                                             ossim_uint32 arraySize)
{
   ossimLeastSquaresBilin inx;
   ossimLeastSquaresBilin inversex;
   ossimLeastSquaresBilin iny;
   ossimLeastSquaresBilin inversey;
   ossim_uint32 idx = 0;
   for(idx=0; idx < arraySize; ++idx)
   {
      inx.addSample(input[idx].x, input[idx].y, output[idx].x);
      inversex.addSample(output[idx].x, output[idx].y, input[idx].x);
      iny.addSample(input[idx].x, input[idx].y, output[idx].y);
      inversey.addSample(output[idx].x, output[idx].y, input[idx].y);
   }
   inx.solveLS();
   inversex.solveLS();
   iny.solveLS();
   inversey.solveLS();
   inx.getLSParms(m_coefficientsXTerm[0], m_coefficientsXTerm[1], m_coefficientsXTerm[2], m_coefficientsXTerm[3]);
   iny.getLSParms(m_coefficientsYTerm[0], m_coefficientsYTerm[1], m_coefficientsYTerm[2], m_coefficientsYTerm[3]);
   inversex.getLSParms(m_inverseCoefficientsXTerm[0], m_inverseCoefficientsXTerm[1], m_inverseCoefficientsXTerm[2], m_inverseCoefficientsXTerm[3]);
   inversey.getLSParms(m_inverseCoefficientsYTerm[0], m_inverseCoefficientsYTerm[1], m_inverseCoefficientsYTerm[2], m_inverseCoefficientsYTerm[3]);
}

bool ossim2dBilinearTransform::saveState(ossimKeywordlist& kwl,
                                       const char* prefix)const
{
   ossimString xterm = (ossimString::toString(m_coefficientsXTerm[0])
                        +" "+ossimString::toString(m_coefficientsXTerm[1])
                        +" "+ossimString::toString(m_coefficientsXTerm[2])
                        +" "+ossimString::toString(m_coefficientsXTerm[3]));
   ossimString yterm = (ossimString::toString(m_coefficientsYTerm[0])
                        +" "+ossimString::toString(m_coefficientsYTerm[1])
                        +" "+ossimString::toString(m_coefficientsYTerm[2])
                        +" "+ossimString::toString(m_coefficientsYTerm[3]));
   ossimString inverse_xterm = (ossimString::toString(m_inverseCoefficientsXTerm[0])
                        +" "+ossimString::toString(m_inverseCoefficientsXTerm[1])
                        +" "+ossimString::toString(m_inverseCoefficientsXTerm[2])
                        +" "+ossimString::toString(m_inverseCoefficientsXTerm[3]));
   ossimString inverse_yterm = (ossimString::toString(m_inverseCoefficientsYTerm[0])
                        +" "+ossimString::toString(m_inverseCoefficientsYTerm[1])
                        +" "+ossimString::toString(m_inverseCoefficientsYTerm[2])
                        +" "+ossimString::toString(m_inverseCoefficientsYTerm[3]));
   
   kwl.add(prefix, "xterm", xterm, true);
   kwl.add(prefix, "yterm", yterm, true);
   kwl.add(prefix, "inverse_xterm", inverse_xterm, true);
   kwl.add(prefix, "inverse_yterm", inverse_yterm, true);
   return ossim2dTo2dTransform::saveState(kwl, prefix);
}

bool ossim2dBilinearTransform::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
   bool result = true;
   ossimString xterm = kwl.find(prefix, "xterm");
   ossimString yterm = kwl.find(prefix, "yterm");
   ossimString inverse_xterm = kwl.find(prefix, "inverse_xterm");
   ossimString inverse_yterm = kwl.find(prefix, "inverse_yterm");
   
   std::vector<ossimString> values;
   xterm.split(values, " ");
   if(values.size() == 4)
   {
      m_coefficientsXTerm[0] = values[0].toDouble();
      m_coefficientsXTerm[1] = values[1].toDouble();
      m_coefficientsXTerm[2] = values[2].toDouble();
      m_coefficientsXTerm[3] = values[3].toDouble();
   }
   else 
   {
      result = false;
   }
   values.clear();
   yterm.split(values, " ");
   if(values.size() == 4)
   {
      m_coefficientsYTerm[0] = values[0].toDouble();
      m_coefficientsYTerm[1] = values[1].toDouble();
      m_coefficientsYTerm[2] = values[2].toDouble();
      m_coefficientsYTerm[3] = values[3].toDouble();
   }
   else 
   {
      result = false;
   }
   values.clear();
   inverse_xterm.split(values, " ");
   if(values.size() == 4)
   {
      m_inverseCoefficientsXTerm[0] = values[0].toDouble();
      m_inverseCoefficientsXTerm[1] = values[1].toDouble();
      m_inverseCoefficientsXTerm[2] = values[2].toDouble();
      m_inverseCoefficientsXTerm[3] = values[3].toDouble();
   }
   else 
   {
      result = false;
   }
   values.clear();
   inverse_yterm.split(values, " ");
   if(values.size() == 4)
   {
      m_inverseCoefficientsYTerm[0] = values[0].toDouble();
      m_inverseCoefficientsYTerm[1] = values[1].toDouble();
      m_inverseCoefficientsYTerm[2] = values[2].toDouble();
      m_inverseCoefficientsYTerm[3] = values[3].toDouble();
   }
   else 
   {
      result = false;
   }

   if(result)
   {
      result = ossim2dTo2dTransform::loadState(kwl, prefix);
   }
   
   return result;
}
