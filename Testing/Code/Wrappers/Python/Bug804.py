import otbApplication
app = otbApplication.Registry_CreateApplication('TrainImagesClassifier')
app.SetParameterFloat('classifier.libsvm.opt',True)
print app.GetParameterValue('classifier.libsvm.opt')


