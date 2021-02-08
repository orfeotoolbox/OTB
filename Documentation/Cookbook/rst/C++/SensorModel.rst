Sensor model
============

The class ``otb::SensorTransformBase`` is the base class for sensor
models. It inherits from ``otb::Transform``, which inherits from
``itk::Transform``. It is templated over the data type, and input and
output dimentions. All sensor model classes should inherit from it,
and implement the methods:

- ``SetMetadataModel`` that takes a ``boost::any`` object representing the model;

- ``IsValidSensorModel`` that returns true if the model is correctly set;

- ``TransformPoint`` that process the transformation.

.. image:: /Art/C++/UmlSensorModel.png

RPC sensor model
----------------

The structure ``otb::Projection::RPCParam`` is used to store the
parameters of the RPC model. It is based on `GDAL's RPC structure
<https://gdal.org/development/rfc/rfc22_rpc.html>`_.

The RPC model is stored in the ``otb::ImageMetadata`` object, using
the key ``MDGeom::RPC``. The classes ``otb::RPCTransformBase``,
``otb::RPCForwardTransform`` and ``otb::RPCInverseTransform`` are used
to perform forward and inverse transformation using this model.

The abstract class ``otb::RPCTransformBase`` contains the
implementation of the SetMetadataModel method, which receives the RPC
description from the ``otb::ImageMetadata`` and instantiates an
``otb::GDALRPCTransformer``.

The classes ``otb::RPCForwardTransform`` and
``otb::RPCInverseTransform`` each implement a version of the
``TransformPoint`` method which uses the ``otb::GDALRPCTransformer``.
