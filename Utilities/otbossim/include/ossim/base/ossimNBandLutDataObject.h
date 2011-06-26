//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LICENSE: LGPL See top level license file.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimNBandLutDataObject.h 19732 2011-06-06 22:24:54Z dburken $
#ifndef ossimNBandLutDataObject_HEADER
#define ossimNBandLutDataObject_HEADER 1

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimFilename.h>
#include <iostream>
#include <map>

/**
 * class ossimNBandLutDataObject
 *
 * Sample CADRG LUT taken from an arbitrary CADRG image entry.  This is a keywordlist
 * support by the loadstate of this class.
 *
 * <pre>
 * entry0:  25 39 116 
 * entry1:  245 242 242 
 * entry10:  92 89 115 
 * entry100:  74 55 85 
 * entry101:  233 165 53 
 * entry102:  75 2 4 
 * entry103:  162 118 42 
 * entry104:  233 159 130 
 * entry105:  189 129 40 
 * entry106:  154 109 66 
 * entry107:  145 97 85 
 * entry108:  121 86 31 
 * entry109:  238 151 118 
 * entry11:  52 31 76 
 * entry110:  239 167 42 
 * entry111:  236 137 100 
 * entry112:  179 112 88 
 * entry113:  92 37 33 
 * entry114:  121 66 24 
 * entry115:  120 133 151 
 * entry116:  76 92 192 
 * entry117:  36 50 117 
 * entry118:  57 75 191 
 * entry119:  125 45 50 
 * entry12:  235 217 204 
 * entry120:  102 65 81 
 * entry121:  172 148 199 
 * entry122:  204 145 95 
 * entry123:  81 19 5 
 * entry124:  91 4 1 
 * entry125:  109 54 31 
 * entry126:  137 114 118 
 * entry127:  230 150 52 
 * entry128:  82 38 87 
 * entry129:  190 120 76 
 * entry13:  244 218 172 
 * entry130:  170 109 45 
 * entry131:  95 20 22 
 * entry132:  150 80 61 
 * entry133:  225 122 82 
 * entry134:  122 37 16 
 * entry135:  146 88 49 
 * entry136:  122 94 122 
 * entry137:  230 135 63 
 * entry138:  137 73 75 
 * entry139:  119 46 29 
 * entry14:  189 167 141 
 * entry140:  190 142 128 
 * entry141:  242 216 187 
 * entry142:  176 125 95 
 * entry143:  193 117 143 
 * entry144:  19 0 0 
 * entry145:  12 1 42 
 * entry146:  24 4 26 
 * entry147:  38 40 52 
 * entry148:  8 24 175 
 * entry149:  40 23 45 
 * entry15:  245 213 142 
 * entry150:  39 11 58 
 * entry151:  10 0 22 
 * entry152:  236 148 41 
 * entry153:  101 41 11 
 * entry154:  230 113 67 
 * entry155:  148 44 46 
 * entry156:  225 144 133 
 * entry157:  115 2 13 
 * entry158:  159 79 49 
 * entry159:  164 95 33 
 * entry16:  246 241 205 
 * entry160:  97 6 22 
 * entry161:  175 90 133 
 * entry162:  112 23 20 
 * entry163:  105 22 6 
 * entry164:  148 59 33 
 * entry165:  215 118 104 
 * entry166:  211 122 41 
 * entry167:  233 129 34 
 * entry168:  153 73 25 
 * entry169:  208 102 39 
 * entry17:  240 215 89 
 * entry170:  206 101 133 
 * entry171:  229 113 96 
 * entry172:  127 16 25 
 * entry173:  224 104 28 
 * entry174:  171 76 74 
 * entry175:  221 94 78 
 * entry176:  229 86 94 
 * entry177:  213 80 57 
 * entry178:  161 44 36 
 * entry179:  155 54 19 
 * entry18:  115 92 157 
 * entry180:  130 25 5 
 * entry181:  150 29 24 
 * entry182:  198 54 34 
 * entry183:  224 61 70 
 * entry184:  213 82 37 
 * entry185:  186 28 31 
 * entry186:  200 64 15 
 * entry187:  110 2 2 
 * entry188:  195 38 12 
 * entry189:  151 6 8 
 * entry19:  160 163 37 
 * entry190:  193 13 10 
 * entry191:  205 1 3 
 * entry192:  44 62 193 
 * entry193:  35 14 16 
 * entry194:  15 30 118 
 * entry195:  28 47 189 
 * entry196:  25 3 80 
 * entry197:  7 16 118 
 * entry198:  33 10 6 
 * entry199:  39 24 95 
 * entry2:  216 212 237 
 * entry20:  38 29 21 
 * entry200:  82 58 159 
 * entry201:  55 29 142 
 * entry202:  33 4 23 
 * entry203:  17 14 36 
 * entry204:  58 26 41 
 * entry205:  11 13 14 
 * entry206:  74 87 85 
 * entry207:  50 60 41 
 * entry208:  119 123 98 
 * entry209:  193 187 172 
 * entry21:  208 202 156 
 * entry210:  124 126 128 
 * entry211:  47 68 84 
 * entry212:  110 119 60 
 * entry213:  63 145 121 
 * entry214:  154 166 147 
 * entry215:  107 156 104 
 * entry22:  242 202 152 
 * entry23:  168 148 140 
 * entry24:  31 22 25 
 * entry25:  245 205 114 
 * entry26:  245 191 137 
 * entry27:  175 161 172 
 * entry28:  17 6 11 
 * entry29:  172 170 137 
 * entry3:  247 230 113 
 * entry30:  51 27 20 
 * entry31:  239 210 73 
 * entry32:  185 176 129 
 * entry33:  35 2 13 
 * entry34:  60 52 51 
 * entry35:  101 78 70 
 * entry36:  141 124 101 
 * entry37:  206 184 107 
 * entry38:  244 174 106 
 * entry39:  82 98 136 
 * entry4:  246 231 132 
 * entry40:  190 191 239 
 * entry41:  241 188 161 
 * entry42:  67 51 38 
 * entry43:  244 184 122 
 * entry44:  211 201 142 
 * entry45:  61 27 16 
 * entry46:  182 182 82 
 * entry47:  243 188 87 
 * entry48:  86 83 60 
 * entry49:  61 40 43 
 * entry5:  204 198 166 
 * entry50:  146 141 93 
 * entry51:  44 4 4 
 * entry52:  231 210 51 
 * entry53:  85 74 39 
 * entry54:  197 169 127 
 * entry55:  182 151 60 
 * entry56:  121 98 73 
 * entry57:  243 182 145 
 * entry58:  58 18 21 
 * entry59:  241 157 91 
 * entry6:  2 0 2 
 * entry60:  59 4 1 
 * entry61:  155 162 223 
 * entry62:  154 133 80 
 * entry63:  73 21 16 
 * entry64:  195 183 116 
 * entry65:  76 51 29 
 * entry66:  234 181 63 
 * entry67:  58 59 98 
 * entry68:  42 22 79 
 * entry69:  134 143 219 
 * entry7:  204 204 239 
 * entry70:  106 117 205 
 * entry71:  48 65 117 
 * entry72:  63 54 117 
 * entry73:  125 93 62 
 * entry74:  137 138 41 
 * entry75:  104 68 59 
 * entry76:  113 99 41 
 * entry77:  76 30 11 
 * entry78:  239 184 48 
 * entry79:  161 129 98 
 * entry8:  31 4 66 
 * entry80:  241 170 126 
 * entry81:  145 109 105 
 * entry82:  207 163 116 
 * entry83:  79 41 40 
 * entry84:  175 138 41 
 * entry85:  10 0 56 
 * entry86:  180 151 77 
 * entry87:  72 12 24 
 * entry88:  242 154 105 
 * entry89:  91 26 15 
 * entry9:  246 238 162 
 * entry90:  160 118 53 
 * entry91:  228 166 141 
 * entry92:  231 160 69 
 * entry93:  61 2 11 
 * entry94:  132 83 60 
 * entry95:  86 46 24 
 * entry96:  243 143 86 
 * entry97:  108 60 46 
 * entry98:  109 76 38 
 * entry99:  96 86 84 
 * null_value:  -1
 * number_bands:  3
 * number_entries:  216
 * scalar_type:  ossim_uint8
 * type:  ossimNBandLutDataObject
 *
 *
 * NOTES:
 * 
 * - The null_value keyword above is the index in the lut that correspnds to an invalid pixel value.
 * - Each entry will have number_bands.  SO if number of bands is 3 then there will be 3 values
 *   Where the first value is for band 1, ... etc.
 * - scalar_type is a hint as to the scalar type of the entries.  Is the resulting lut for 8 bit, 16 bit , etc data.
 * - You should always use the LUT_ENTRY_TYPE when getting values from the lut.  This value could change to an
 *   ossim_float32 precision and if you use the LUT_ENTRY_TYPE then your code will not have to change.
 *   
 * </pre>
 */
class OSSIM_DLL ossimNBandLutDataObject : public ossimObject
{
public:
   
   typedef ossim_int32 LUT_ENTRY_TYPE;
   
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimNBandLutDataObject& lut);
   /*!
    * Will allocate 256 values for the color table
    */
   ossimNBandLutDataObject(ossim_uint32 numberOfEntries=0,
                           ossim_uint32 numberOfBands=0,
                           ossimScalarType bandScalarType = OSSIM_UINT8,
                           ossim_int32 nullPixelIndex=-1);
   ossimNBandLutDataObject(const ossimNBandLutDataObject& lut);
   ossimObject* dup()const
   {
      return new ossimNBandLutDataObject(*this);
   }
   void create(ossim_uint32 numberOfEntries,
               ossim_uint32 numberOfBands=3);
   const LUT_ENTRY_TYPE* operator[](ossim_uint32 idx)const
   {
      if(theLut)
      {
         return &theLut[idx*theNumberOfBands];
      }
      
      return 0;
   }
   LUT_ENTRY_TYPE* operator[](ossim_uint32 idx)
   {
      if(theLut)
      {
         return &theLut[idx*theNumberOfBands];
      }
      
      return 0;
   }
   const LUT_ENTRY_TYPE* operator[](ossim_int32 idx)const
   {
      if(theLut)
      {
         return &theLut[idx*theNumberOfBands];
      }
      
      return 0;
   }
   LUT_ENTRY_TYPE* operator[](ossim_int32 idx)
   {
      if(theLut)
      {
         return &theLut[idx*theNumberOfBands];
      }
      
      return 0;
   }
   const LUT_ENTRY_TYPE* operator[](double normalizedIndex)const
   {
      int idx = int(normalizedIndex*theNumberOfEntries);
      if (idx < 0)
      {
         idx = 0;
      }
      return (*this)[idx];
   }

   /**
    * @param normalizedIndex  If a double precision value is passed in then it is
    *                         assumed to be in normalized space where the value ranges from 0.0 to 1.0
    * @return the starting address of the band values.
    */ 
   LUT_ENTRY_TYPE* operator[](double normalizedIndex)
   {
      ossim_uint32 idx = int(normalizedIndex*(theNumberOfEntries-1));
      if (idx >= theNumberOfEntries)
      {
         idx = theNumberOfEntries-1;
      }
      return (*this)[idx];
   }
   
   bool hasNullPixelIndex()const
   {
      return (theNullPixelIndex >= 0);
   }
   /**
    * Will return the min max value for a given band
    *
    * @param band band number to query.  This is 0 based so the first band
    *        is indicated by the value of 0.
    * @param minValue is the minimum value returned for the passed in band
    * @param maxValue is the maximum value returned for the passed in band
    */
   void getMinMax(ossim_uint32 band,
                  LUT_ENTRY_TYPE& minValue,
                  LUT_ENTRY_TYPE& maxValue);

   /**
    * @return First index with alpha of 0 or -1 if not found or there is no alpha.
    *
    * Currently only works on 4 band data.  May consider adding an alpha channel
    * class data member in the future.
    */
   ossim_int32 getFirstNullAlphaIndex() const;
   
   /**
    * Allows you to specify which entry is to be designated as an invalid
    * entry.  Note: setting this to -1 indicates no null entry is set.
    *
    * @param idx is the entry number that will be used for the null value
    */
   void setNullPixelIndex(ossim_int32 idx){theNullPixelIndex = idx;}

   /**
    * @return Return either a positive value indicating the entry designated for
    *         a null pixel value or a -1 to say no null pixel has been designated
    *         yet.
    */ 
   ossim_int32 getNullPixelIndex()const{return theNullPixelIndex;}

   /**
    * @return Return the number of bands this lut is indexed for.
    */ 
   ossim_uint32 getNumberOfBands()const{return theNumberOfBands;}

   /**
    * @return Return the number of entries in the lut.
    */ 
   ossim_uint32 getNumberOfEntries()const{return theNumberOfEntries;}

   /**
    * This method gives you a way to find an entry index closest to the passed in
    * band values.
    * 
    * @param values Is an array of band values.  This array must be equal to the number
    *        of bands for the lut.
    * @return Returns the nearest index to the passed in value.  
    */ 
   ossim_uint32 findIndex(ossim_int32* values) const;

   /**
    * @brief This method gives you a way to find an entry index closest to the
    * passed in band values.
    * 
    * @param values Is an array of band values.
    * @param size Size of values array.  Note if size is less than the LUT
    * number of bands, the trailing bands get dropped.  This allows finding
    * an index dropping the alpha channel.
    * 
    * @return Returns the nearest index to the passed in value.  
    */ 
   ossim_uint32 findIndex(ossim_int32* values, ossim_uint32 size) const;

   /**
    * Will do a simple memset to '\0' all 
    */ 
   void clearLut();
   /**
    * does a deep copy from the passed in lut to the object.
    *
    * @param lut The lut to copy.
    */ 
   const ossimNBandLutDataObject& operator =(const ossimNBandLutDataObject& lut);

   /**
    * Determines if the state of this lut is identical to the passed in lut
    *
    * @param lut Checks equality of the this object to the lut parameter.
    * @return True if equal and false otherwise
    */ 
   bool operator ==(const ossimNBandLutDataObject& lut)const;

   /**
    * Reads in an external lut file.  The external file is in a keywordlist format specified by the saveState.
    */ 
   bool open(const ossimFilename& lutFile);

   /**
    * <pre>
    * 
    * Keywords:
    *  
    * entry0:
    * entry1:
    * :
    * entry<number_entries-1>:
    * null_value:  -1
    * number_bands:  3
    * number_entries:  216
    * scalar_type:  ossim_uint8
    * type:  ossimNBandLutDataObject
    *
    * Where entry<idx> is repeasted for each entry number where idx start from 0 and goes to number_entries-1
    * each entry must have number_bands values.
    * </pre>
    *
    * @param kwl The keyword list to store the keywords to.  Each keyword could be prefixed by an optional
    *            prefix parameter.
    * @param prefix The optional prefix parameter used to prefix each keyword.
    * 
    */ 
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;

   std::vector<ossimString> getEntryLabels(ossim_uint32 band);

   void setEntryLables(ossim_uint32 band, std::vector<ossimString> entryLabels);

   
   /**
    * Loads the state of the object by reading in the keywords listed in the save state.
    *
    * @param kwl The keywordlist to extract the classes keywords from to restore its state.
    * @param prefix  Optional prefix value used to prefix each keyword.
    */ 
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
protected:
   virtual ~ossimNBandLutDataObject();

   LUT_ENTRY_TYPE *theLut;
   ossim_uint32    theNumberOfEntries;
   ossim_uint32    theNumberOfBands;
   ossimScalarType theBandScalarType;
   ossim_int32     theNullPixelIndex;
   std::map<ossim_uint32, std::vector<ossimString> >  m_entryLabels;
   
TYPE_DATA
};

#endif
