set(DOCUMENTATION "Anomalies are composed of a set of isolated pixels with
anomalous signatures (when compared to the image background) which represents a very
small piece of the full image, and they only occur in the image with low
probabilities. This module contains classes related to well-known approaches for anomaly
detection which have shown success for multispectral and hyperspectral images
(for example Local-RX detector).")

otb_module(OTBAnomalyDetection 
  DEPENDS
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
