import otbApplication as otb

# TO RUN WITH PYTHON3
# try:
#    unicode = unicode
# except NameError:
#    # 'unicode' is undefined, must be Python 3
#    str = str
#    unicode = str
#    bytes = bytes
#    basestring = (str,bytes)
# else:
#    # 'unicode' exists, must be Python 2
#    str = str
#    unicode = unicode
#    bytes = str
#    basestring = basestring
  

# This test checks that UpdateOutputInformation() has been called on output image pointers when connecting pipeline in-memory
# This code snippet has been used to reproduce bug #1498

if __name__ == '__main__':
    inimage = sys.argv[2]
    outimage = sys.argv[3]

    ext1 = otb.Registry.CreateApplication("ExtractROI")
    ext2 = otb.Registry.CreateApplication("ExtractROI")

    ext1.SetParameterString('in',inimage)
    ext1.Execute()
    
    ext2.SetParameterInputImage('in',ext2.GetParameterOutputImage('out'))
    ext2.SetParameterStringList('cl',['Channel1'])
    ext2.SetParameterString("out",outimage)
    ext2.ExecuteAndWriteOutput()
