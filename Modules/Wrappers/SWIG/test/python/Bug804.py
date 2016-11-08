def test(otb, argv):
	app = otb.Registry.CreateApplication('TrainImagesClassifier')
	app.SetParameterFloat('classifier.libsvm.opt',True)
	print( app.GetParameterValue('classifier.libsvm.opt') )
