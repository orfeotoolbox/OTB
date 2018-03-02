This module contains a new dimensionality reduction framework for the Orfeo Toolbox.

The framework is based on Machine learning models and a dimensionality reduction algorithm 
can be trained and used using the model class, in the same fashion as Machine Learning Models
for Classification (supervised or unspervised) and Regression.

The algorithms featured in the module are (27/06/2017) :
 - autoencoders and multi layer autoencoders, with several regularization options
 - PCA 
 - SOM 
 
 Autoencoders and PCA models are using Shark ML library, while SOM model is based on the SOM classes of the OTB.
 
 More specifically, the module contains :
 
 - Autoencoder models, PCA models and SOM models, with methods for training, serialization and prediction (i.e. reduction)
 - A Dimensionality Reduction Model Factory and a factories for each model, which allow the user to create objects of the model classes
 - A (OTB ImageToImage) filter that can be used to perform dimensionality reduction on an image. This filter supports threading and streaming
 - An application for training the models according to a shapefile
 - An application for using a trained model on a shapefile
 - An application for using a trained model on an image (using the filter)
 
			/!\ Work In Progress /!\
 

