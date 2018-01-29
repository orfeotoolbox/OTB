
// File: index.xml


%feature("docstring") Application "

This class represent an application.  

This class is an abstract class that is used to set a framework for the
different applications that will derived from it. It defines the basic actions
that one needs to create or use an application, mainly through parameter
manipulation. See the Parameter class documentation for more information.  

C++ includes: otbWrapperApplication.h
";

%feature("docstring") GetParameterUInt16Image "
";

%feature("docstring") IsParameterEnabled "

Return the enable state of an optional parameter.  
";

%feature("docstring") SetNthParameterInputImageList "

Set the nth image of an InputImageList parameter.  

Set the nth image of an InputImageList parameter as an ImageBase pointer instead
of reading from file. Useful to connect pipelines between different application
instances.  

Parameters
----------
* `parameter` :  
    The parameter key  
* `id` :  
    Position at which to set the ImageBase pointer  
* `img` :  
    The ImageBase * of the image to add  

Exceptions
----------
* `itk::Exception` :  
    if parameter is not found or not an InputImageList parameter or if id is out
    of bounds  
";

%feature("docstring") ClearParameterInputImageList "

Clear all images from an InputImageList parameter.  

Parameters
----------
* `parameter` :  
    The parameter key  

Exceptions
----------
* `itk::Exception` :  
    if parameter is not found or not an InputImageList parameter  
";

%feature("docstring") SetMinimumParameterFloatValue "

Set a minimum float value  

This method must be used in the DoInit when setting the minimum *value* allowed
for the *parameter*  

Can be called for type `ParameterType_Float`  
";

%feature("docstring") GetParameterInt32Image "
";

%feature("docstring") GetDocName "

Doc element accessors.  
";

%feature("docstring") SetDocLink "

documentation link  
";

%feature("docstring") GetParameterList "

Get the internal application parameters  

WARNING: this method may disappear from the API  
";

%feature("docstring") GetParameterAsString "

Get the parameter as a std::string  

Can be called for types :  

*   `ParameterType_String`  
*   `ParameterType_InputFilename`  
*   `ParameterType_OutputFilename`  
*   `ParameterType_Directory`  
*   `ParameterType_InputImage`  
*   `ParameterType_ComplexInputImage`  
*   `ParameterType_InputVectorData`  
*   `ParameterType_OutputImage`  
*   `ParameterType_OutputVectorData`  
*   `ParameterType_ListView`  
*   `ParameterType_Choice`  
*   `ParameterType_OutputProcessXML`  
*   `ParameterType_InputProcessXML`  
*   `ParameterType_StringList` (get parameter string list)  
*   `ParameterType_InputImageList` (get parameter string list)  
*   `ParameterType_InputVectorDataList` (get parameter string list)  
*   `ParameterType_InputFilenameList` (get parameter string list)  
*   `ParameterType_Int`  
*   `ParameterType_Radius`  
*   `ParameterType_RAM`  
";

%feature("docstring") IsApplicationReady "

Checks if the application is ready to be executed.  

It checks that there is no parameter missing.  
";

%feature("docstring") SetParameterComplexOutputImagePixelType "

Set the complex pixel type in which the image will be saved.  

Can be called for type `ParameterType_ComplexOutputImage`  
";

%feature("docstring") SetDocAuthors "

Doc element accessors.  
";

%feature("docstring") GetProgressDescription "
";

%feature("docstring") GetParameterFloatImage "
";

%feature("docstring") GetLastExecutionTiming "
";

%feature("docstring") SetLogger "

Set the logger instance of the application as *logger* (to use with caution)  
";

%feature("docstring") SetParameterComplexOutputImage "

Set a complex output image value.  

Can be called for type `ParameterType_ComplexOutputImage`  
";

%feature("docstring") GetParameterStringList "

Get a string list parameter value.  

Can be called for types :  
    
*   `ParameterType_StringList` 
*   `ParameterType_ListView` (get the filename list)  
*   `ParameterType_InputImageList` (get the filename list)  
*   `ParameterType_InputVectorDataList` (get the filename list)  
*   `ParameterType_InputFilenameList` (get the filename list)  
";

%feature("docstring") Execute "

Run the application.  

For pipeline ready application, this only wire and configure the pipeline, and
provides the output image or vector data parameters as pointers.  

In other cases, the application must handle the I/O (intermediary results for
example)  

Returns 0 on success, or a non-null integer on error  
";

%feature("docstring") GetDocTags "

Get the application doc tag  
";

%feature("docstring") SetDocTags "

Set the application doc tag to *val*  
";

%feature("docstring") SetListViewSingleSelectionMode "

Set single selection mode on *status*.  

Can be called for type `ParameterType_ListView`  
";

%feature("docstring") SetParameterList "

Set the parameter list of the application as *paramGroup*  
";

%feature("docstring") SetHaveOutXML "

Set the parameter xml flag  
";

%feature("docstring") GetHtmlExample "

Generate the first command line example (HTML style)  
";

%feature("docstring") GetParameterDoubleVectorImage "
";

%feature("docstring") EnableParameter "

Enable the use of an optional parameter.  
";

%feature("docstring") SetParameterInt "

Set an integer value.  

Can be called for types :  

*   `ParameterType_Int`  
*   `ParameterType_Float`  
*   `ParameterType_Radius`  
*   `ParameterType_Choice`  
";

%feature("docstring") SetDefaultOutputPixelType "

Set a default pixel type for an output image parameter  

Parameters
----------
* `parameter` :  
    Name of the output image parameter  
* `type` :  
    Default pixel type  
";

%feature("docstring") SetNthParameterStringList "

Set the nth value of a parameter list as a string.  

Can be called for parameter type `ParameterType_InputImageList` .  

Parameters
----------
* `parameter` :  
    The parameter key  
* `id` :  
    Position at which to set the ImageBase pointer  
* `str` :  
    The filename of the image to add  

Exceptions
----------
* `itk::Exception` :  
    if parameter is not found or not an InputImageList parameter or if id is out
    of bounds  
";

%feature("docstring") SetMaximumParameterIntValue "

Set a maximum int value.  

This method must be used in the DoInit when setting the maximum *value* allowed
for the *parameter*  

Can be called for type `ParameterType_Int`  
";

%feature("docstring") SetMaximumParameterFloatValue "

Set a maximum float value  

This method must be used in the DoInit when setting the maximum *value* allowed
for the *parameter*  

Can be called for type `ParameterType_Float`  
";

%feature("docstring") GetCLExample "

Generate the first command line example  
";

%feature("docstring") GetParameterEmpty "

Get active flag of parameter with key paramKey  
";

%feature("docstring") SetParameterDescription "

Set the description of a parameter.  
";

%feature("docstring") GetParameterFloatVectorImage "
";

%feature("docstring") GetParameterDescription "

Returns the description of a parameter.  
";

%feature("docstring") GetNumberOfElementsInParameterInputImageList "

Get the number of images in an InputImageList parameter.  

Parameters
----------
* `parameter` :  
    The parameter key  

Returns
-------
The number of images  

Exceptions
----------
* `itk::Exception` :  
    if parameter is not found or not an InputImageList parameter  
";

%feature("docstring") GetParameterOutputImage "

Get the output image parameter  

Get the output image parameter as an ImageBase * instead of writing to disk.
Useful to connect pipelines between different application instances.  

Parameters
----------
* `parameter` :  
    The parameter key  

Returns
-------
The ImageBase * to the output image  

Exceptions
----------
* `itk::Exception` :  
    if parameter is not found or not an OutputImageParameter  
";

%feature("docstring") GetParameterVectorDataList "

Get a vector data list as a pointer of `VectorDataListType`.  

Can be called for type `ParameterType_InputVectorDatalist`  
";

%feature("docstring") SetDocLimitations "

Doc element accessors.  
";

%feature("docstring") GetDocSeeAlso "

Doc element accessors.  
";

%feature("docstring") SetParameterOutputImage "

Set an output image value.  

Can be called for type `ParameterType_OutputImage`  
";

%feature("docstring") SetDefaultParameterInt "

Set a default integer value.  

This method must be used in the DoInit when setting a `value` by default for the
parameter.  

Can be called for types :  

*   `ParameterType_Int`  
*   `ParameterType_Float`  
*   `ParameterType_Radius`  
*   `ParameterType_RAM`  
";

%feature("docstring") SetParameterUserValue "

Set UserValue flag of parameter with key paramKey  
";

%feature("docstring") HasValue "

Returns true if the parameter has an associated value.  

This value can be an automatically computed value, default value, or a value set
externally by user  
";

%feature("docstring") GetHaveOutXML "

Get the parameter xml flag  
";

%feature("docstring") GetParameterUInt8VectorImage "
";

%feature("docstring") SetHaveInXML "

Set the parameter xml flag  
";

%feature("docstring") GetParametersKeys "

Get the list of all parameters as a vector of their key.  

The *recursive* allows you to get all parameters inside group prameter (default
value is )  
";

%feature("docstring") AddParameterStringList "

Add a value to a parameter list as a string.  

Can be called for parameter type `ParameterType_InputImageList` .  

Parameters
----------
* `parameter` :  
    The parameter key  
* `str` :  
    The filename of the image to add  

Exceptions
----------
* `itk::Exception` :  
    if parameter is not found or not an InputImageList parameter  
";

%feature("docstring") SetDocName "

Doc element accessors.  
";

%feature("docstring") GetDocLimitations "

Doc element accessors.  
";

%feature("docstring") HasUserValue "

Returns true if the parameter has an associated value provided externally (not
automatically computed by the application).  
";

%feature("docstring") AddImageToParameterInputImageList "

Add an image to an InputImageList parameter.  

Add an image to an InputImageList parameter as an ImageBase pointer instead of
reading from file. Useful to connect pipelines between different application
instances.  

Parameters
----------
* `parameter` :  
    The parameter key  
* `img` :  
    The ImageBase * of the image to add  

Exceptions
----------
* `itk::Exception` :  
    if parameter is not found or not an InputImageList parameter  
";

%feature("docstring") GetProgressSource "
";

%feature("docstring") GetParameterInt "

Get an integer from a *parameter*.  

Can be called for types :  

*   `ParameterType_Int`  
*   `ParameterType_Float`  
*   `ParameterType_Radius`  
*   `ParameterType_Choice`  
*   `ParameterType_RAM`  
";

%feature("docstring") SetMinimumParameterIntValue "

Set a minimum int value.  

This method must be used in the DoInit when setting the minimum *value* allowed
for the *parameter*  

Can be called for type `ParameterType_Int`  
";

%feature("docstring") GetParameterString "

Get a string parameter value.  

Can be called for types :  

*   `ParameterType_String`  
*   `ParameterType_InputFilename`  
*   `ParameterType_OutputFilename`  
*   `ParameterType_Directory`  
*   `ParameterType_InputImage` (get the filename)  
*   `ParameterType_ComplexInputImage` (get the filename)  
*   `ParameterType_InputVectorData` (get the filename)  
*   `ParameterType_OutputImage` (get the filename)  
*   `ParameterType_OutputVectorData` (get the filename)  
*   `ParameterType_ListView` (get the string list)  
*   `ParameterType_Choice` (get the string list)  
";

%feature("docstring") GetChoiceNames "

Returns the list of choices (name) of the ListView parameter or the Choice
parameter.  
";

%feature("docstring") GetParameterInt16Image "
";

%feature("docstring") SetOfficialDocLink "
";

%feature("docstring") GetParameterImageList "

Get an image list value as a pointer of `FloatVectorImageListType`.  

Can be called for type `ParameterType_InputImageList`  
";

%feature("docstring") GetParameterUserLevel "

Return the user level of access to a parameter  
";

%feature("docstring") GetParameterName "

Returns the name of a parameter.  
";

%feature("docstring") GetNameOfClass "

RTTI support  
";

%feature("docstring") GetDocExample "

Get the application doc example  
";

%feature("docstring") ExecuteAndWriteOutput "

Run the application, then writes all the output to disk if they have an
associated filename.  

This is a helper function for wrappers without pipeline support.  

Returns 0 on success, or a non-null integer on error  
";

%feature("docstring") GetParameterRole "

Get the role of the parameter  
";

%feature("docstring") GetOutputParametersSumUp "

Return all enable parameters which role is `Role_Output`  

Return all parameters which role is `Role_Output` and that are enable in a
vector of pairs that contains the parameter key (first) and its value (second).  
";

%feature("docstring") SetExampleComment "

Set the *i* th example comment to *comm*  
";

%feature("docstring") GetExampleComment "

Get the comment of the *id* th example of the application doc example  
";

%feature("docstring") IsParameterMissing "

Checks if a parameter 'key' is missing.  

A parameter is missing when all the following conditions are true :  

*   the parameter has no value  
*   the parameter is mandatory  
*   the parameter has Role_Input  
*   the parameter is not a group  
*   the parameter ancestors are mandatory or enabled.  
";

%feature("docstring") GetParameterDoubleImage "
";

%feature("docstring") GetParameterComplexImage "

Get a complex image value as a pointer of `ComplexFloatVectorImageType`.  

Can be called for type `ParameterType_ComplexInputImage`  
";

%feature("docstring") GetNumberOfExamples "

Get the number of example in the application doc example  
";

%feature("docstring") otbGetParameterComplexImageMacro "
";

%feature("docstring") otbGetParameterComplexImageMacro "
";

%feature("docstring") otbGetParameterComplexImageMacro "
";

%feature("docstring") otbGetParameterComplexImageMacro "
";

%feature("docstring") GetParameterByKey "

Get the internal application parameter specified.  

If the follow flag is on, the function returns the target of proxy parameters,
default is on.  

WARNING: this method may disappear from the API  
";

%feature("docstring") GetParameterByKey "

Get the internal application parameter specified.  

WARNING: this method may disappear from the API  
";

%feature("docstring") GetParameterComplexOutputImage "

Get the complex output image parameter.  

Get the complex output image parameter as an ImageBase * instead of writing to
disk. Useful to connect pipelines between different application instances.  

Parameters
----------
* `parameter` :  
    The parameter key  

Returns
-------
The ImageBase * pointer to the output image  

Exceptions
----------
* `itk::Exception` :  
    if parameter is not found or not an ComplexOutputImageParameter  
";

%feature("docstring") AddExample "

Add an example to the application doc example.  

    Return the id of the added example  
";

%feature("docstring") GetParameterUInt8Image "
";

%feature("docstring") SetDefaultParameterFloat "

Set a default floating value.  

This method must used in the DoInit when setting a `value` by default for the
parameter.  

Can be called for type `ParameterType_Float`  
";

%feature("docstring") GetParameterUInt32Image "
";

%feature("docstring") ClearValue "

If a user value was provided clear it and update the other parameters  
";

%feature("docstring") GetParameterUInt32VectorImage "
";

%feature("docstring") GetParameterImage "

Get an image value as a pointer of `FloatVectorImageType`.  

Can be called for type `ParameterType_InputImage`  
";

%feature("docstring") SetParameterComplexInputImage "

Set the input complex image parameter.  

Set the input complex image parameter as an ImageBase * instead of filename.
Useful to connect pipelines between different application instances.  

Parameters
----------
* `parameter` :  
    The parameter key  
* `inputImage` :  
    ImageBase pointer to use as input  

Exceptions
----------
* `itk::Exception` :  
    if parameter is not found or not an ComplexInputImageParameter  
";

%feature("docstring") GetParameterOutputImagePixelType "

Get the pixel type in which the image will be saved.  

Can be called for type `ParameterType_OutputImage`  
";

%feature("docstring") SetName "

Set the application name  
";

%feature("docstring") GetDocLongDescription "

Doc element accessors.  
";

%feature("docstring") GetParameterInt32VectorImage "
";

%feature("docstring") GetParameterFloat "

Get a floating from a *parameter*.  

Can be called for type `ParameterType_Float`  
";

%feature("docstring") SetParameterStringList "

Set a string value.  

Can be called for types :  

*   `ParameterType_StringList`  
*   `ParameterType_InputFilenameList`  
*   `ParameterType_ListView` (set selected names)  
*   `ParameterType_InputVectorDataList` (set filenames)  
*   `ParameterType_InputImageList` (set filenames)  
";

%feature("docstring") SetDefaultOutputComplexPixelType "

Set a default complex pixel type for an output complex image parameter  

Parameters
----------
* `parameter` :  
    Name of the output complex image parameter  
* `type` :  
    Default complex pixel type  
";

%feature("docstring") GetLogger "

Return the logger of the application  
";

%feature("docstring") Init "

Initialize the application, instantiating the parameter list  
";

%feature("docstring") SetDocLongDescription "

Doc element accessors.  
";

%feature("docstring") SetParameterEmpty "

Set Active flag to *value* and UserValue to *hasUserValueFlag* .  
";

%feature("docstring") SetParameterString "

Set a string value.  

Can be called for types :  

*   `ParameterType_Choice`  
*   `ParameterType_ListView`  
*   `ParameterType_String`  
*   `ParameterType_InputFilename`  
*   `ParameterType_OutputFilename`  
*   `ParameterType_Directory`  
*   `ParameterType_RAM`  
*   `ParameterType_Int` (conversion)  
*   `ParameterType_Float` (conversion)  
*   `ParameterType_Radius` (conversion)  
*   `ParameterType_InputImage` (set filename)  
*   `ParameterType_ComplexInputImage` (set filename)  
*   `ParameterType_InputVectorData` (set filename)  
*   `ParameterType_OutputImage` (set filename)  
*   `ParameterType_ComplexOutputImage` (set filename)  
*   `ParameterType_OutputVectorData` (set filename)  
*   `ParameterType_OutputProcessXML` (set filename)  
*   `ParameterType_InputProcessXML` (set filename)  
";

%feature("docstring") SetParameterOutputVectorData "

Set an output vector data value.  

Can be called for type `ParameterType_OutputVectorData`  
";

%feature("docstring") SetParameterInputImage "

Set the input image parameter  

Set the input image parameter as an ImageBase * instead of filename. Useful to
connect pipelines between different application instances.  

Parameters
----------
* `parameter` :  
    The parameter key  
* `inputImage` :  
    ImageBase pointer to use as input  

Exceptions
----------
* `itk::Exception` :  
    if parameter is not found or not an InputImageParameter  
";

%feature("docstring") GetParameterUInt8RGBAImage "
";

%feature("docstring") GetParameterInt16VectorImage "
";

%feature("docstring") GetExampleParameterValue "

Get the parameter value of the *paramId* th parameter of the *id* th example of
the application doc example  
";

%feature("docstring") UpdateParameters "

Update the value of parameters for which no user value has been provided  
";

%feature("docstring") SetParameterFloat "

Set a floating value.  

Can be called for type `ParameterType_Float`  
";

%feature("docstring") GetDescription "

Get the application description  
";

%feature("docstring") IsMandatory "

Return true if the specified parameter is mandatory.  
";

%feature("docstring") SetParameterOutputImagePixelType "

Set the pixel type in which the image will be saved.  

Can be called for type `ParameterType_OutputImage`  
";

%feature("docstring") GetParameterType "

Get the parameter type  
";

%feature("docstring") GetParameterUInt16VectorImage "
";

%feature("docstring") GetParameterVectorData "

Get a vector data as a pointer of `VectorDataType`.  

Can be called for type `ParameterType_InputVectorData`  
";

%feature("docstring") SetDescription "

Set the application description  
";

%feature("docstring") GetParameterComplexOutputImagePixelType "

Get the pixel type in which the complex image will be saved.  

Can be called for type `ParameterType_ComplexOutputImage`  
";

%feature("docstring") AddDocTag "

Add a *tag* to the application doc tag  
";

%feature("docstring") GetName "

Get the application name  
";

%feature("docstring") GetDocAuthors "

Doc element accessors.  
";

%feature("docstring") GetChoiceKeys "

Returns the list of choices (key) of the ListView parameter or the Choice
parameter.  
";

%feature("docstring") SetDocExampleParameterValue "

Set the parameter *key* value of the *id* th example of the application doc
example to *val*  
";

%feature("docstring") HasAutomaticValue "

Return true if the specified parameter was set automatically in the application.  
";

%feature("docstring") DisableParameter "

Disable the use of an optional parameter.  
";

%feature("docstring") GetDocLink "

Get the doc link  
";

%feature("docstring") GetParameterUInt8RGBImage "
";

%feature("docstring") SetDocSeeAlso "

Doc element accessors.  
";

%feature("docstring") GetExampleParameterKey "

Get the parameter key of the *paramId* th parameter of the *id* th example of
the application doc example  
";

%feature("docstring") ForceInXMLParseFlag "

If need to force readxml more than once in application  
";

%feature("docstring") GetHaveInXML "

Get the parameter xml flag  
";

%feature("docstring") IsInitialized "

Check if the application has been initialized  
";

%feature("docstring") GetExampleNumberOfParameters "

Get the number of parameter of the *id* th example of the application doc
example  
";

