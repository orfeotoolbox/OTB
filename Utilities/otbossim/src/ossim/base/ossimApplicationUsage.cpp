//-------------------------------------------------------------------------
//
// This code was taken from Open Scene Graph and incorporated from into
// OSSIM.
//
//-------------------------------------------------------------------------
// $Id: ossimApplicationUsage.cpp 19682 2011-05-31 14:21:20Z dburken $

#include <iostream>
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/base/ossimCommon.h>

ossimApplicationUsage::ossimApplicationUsage(const ossimString& commandLineUsage):
    theCommandLineUsage(commandLineUsage)
{
}

ossimApplicationUsage* ossimApplicationUsage::instance()
{
    static ossimApplicationUsage theApplicationUsage;
    return &theApplicationUsage;
}
void ossimApplicationUsage::setApplicationName(const ossimString& name)
{
   theApplicationName = name;
}

const ossimString& ossimApplicationUsage::getApplicationName() const
{
   return theApplicationName;
}
void ossimApplicationUsage::setDescription(const ossimString& desc)
{
   theDescription = desc;
}

const ossimString& ossimApplicationUsage::getDescription() const
{
   return theDescription;
}

void ossimApplicationUsage::addUsageExplanation(Type type,const ossimString& option,const ossimString& explanation)
{
    switch(type)
    {
        case(OSSIM_COMMAND_LINE_OPTION):
            addCommandLineOption(option,explanation);
            break;
        case(OSSIM_ENVIRONMENTAL_VARIABLE):
            addEnvironmentalVariable(option,explanation);
            break;
    }
}
void ossimApplicationUsage::setCommandLineUsage(const ossimString& explanation)
{
   theCommandLineUsage=explanation;
}

const ossimString& ossimApplicationUsage::getCommandLineUsage() const
{
   return theCommandLineUsage;
}

void ossimApplicationUsage::addCommandLineOption(const ossimString& option,const ossimString& explanation)
{
    theCommandLineOptions[option]=explanation;
}

const ossimApplicationUsage::UsageMap& ossimApplicationUsage::getCommandLineOptions() const
{
   return theCommandLineOptions;
}

void ossimApplicationUsage::addEnvironmentalVariable(const ossimString& option,const ossimString& explanation)
{
    theEnvironmentalVariables[option]=explanation;
}
const ossimApplicationUsage::UsageMap& ossimApplicationUsage::getEnvironmentalVariables() const
{
   return theEnvironmentalVariables;
}

void ossimApplicationUsage::getFormatedString(ossimString& str, const UsageMap& um,unsigned int widthOfOutput)
{

    unsigned int maxNumCharsInOptions = 0;
    ossimApplicationUsage::UsageMap::const_iterator citr;
    for(citr=um.begin();
        citr!=um.end();
        ++citr)
    {
        maxNumCharsInOptions = ossim::max(maxNumCharsInOptions,(unsigned int)citr->first.length());
    }
    
    unsigned int fullWidth = widthOfOutput;
    unsigned int optionPos = 2;
    unsigned int explanationPos = 2+maxNumCharsInOptions+2;
    unsigned int explanationWidth = fullWidth-explanationPos;

    ossimString line;
    
    for(citr=um.begin();
        citr!=um.end();
        ++citr)
    {
        line.assign(fullWidth,' ');
        line.replace(optionPos,citr->first.length(),citr->first);
        
        const ossimString& explanation = citr->second;
        std::string::size_type pos = 0;
        std::string::size_type offset = 0;
        bool firstInLine = true;
        while (pos<explanation.length())
        {
            if (firstInLine) offset = 0;
                    
            // skip any leading white space.
            while (pos<explanation.length() && *(explanation.begin()+pos)==' ')
            {
                if (firstInLine) ++offset;
                ++pos;
            }
            
            firstInLine = false;
        
            std::string::size_type width = ossim::min((ossim_int64)(explanation.length()-pos),
                                                      (ossim_int64)(explanationWidth-offset));
            std::string::size_type slashn_pos = explanation.find('\n',pos);
            
            unsigned int extraSkip = 0;
            bool concatinated = false;
            if (slashn_pos!=std::string::npos)
            {
                if (slashn_pos<pos+width)
                {
                    width = slashn_pos-pos;
                    ++extraSkip;
                    firstInLine = true;
                }
                else if (slashn_pos==pos+width) 
                {
                    ++extraSkip;
                    firstInLine = true;
                }
            }
            
            if (pos+width<explanation.length())
            {
                // now reduce width until we get a space or a return
                // so that we ensure that whole words are printed.
                while (width>0 && 
                       *(explanation.begin()+(pos+width))!=' ' && 
                       *(explanation.begin()+(pos+width))!='\n') --width;
                       
                if (width==0)
                {
                    // word must be longer than a whole line so will need
                    // to concatinate it.
                    width = explanationWidth-1;
                    concatinated = true;
                }
            }

            line.replace(explanationPos+offset,explanationWidth, explanation, pos, width);

            if (concatinated) { str += line; str += "-\n"; }
            else { str += line; str += "\n"; }
            
            // move to the next line of output.
            line.assign(fullWidth,' ');
            
            pos += width+extraSkip;

            
        }
                
    }
}

void ossimApplicationUsage::write(std::ostream& output, const ossimApplicationUsage::UsageMap& um,unsigned int widthOfOutput)
{
    ossimString str;
    getFormatedString(str, um, widthOfOutput);
    output << str << std::endl;
}

void ossimApplicationUsage::write(std::ostream& output, unsigned int type, unsigned int widthOfOutput)
{

    output << "Usage: "<<getCommandLineUsage()<<std::endl;
    if(theDescription.size()>0)
    {
       // TODO: take into account "widthOfOutput"
       output << "\nDescription:\n" << theDescription.c_str() << "\n"
              << std::endl;
    }
    
    bool needspace = false;
    if ((type&OSSIM_COMMAND_LINE_OPTION) && !getCommandLineOptions().empty())
    {
        if (needspace) output << std::endl;
        output << "Options:"<<std::endl;
        write(output,getCommandLineOptions(),widthOfOutput);
        needspace = true;
    }
    
    if ((type&OSSIM_ENVIRONMENTAL_VARIABLE) && !getEnvironmentalVariables().empty())
    {
        if (needspace) output << std::endl;
        output << "Environmental Variables:"<<std::endl;
        write(output,getEnvironmentalVariables(),widthOfOutput);
        needspace = true;
    }

}

