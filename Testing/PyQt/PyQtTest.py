# -*- coding: utf-8 -*-

import sys
import otbApplication as otb_core
import otbapp as otb_gui
from PyQt4 import QtGui

if __name__ == "__main__":
    qapp = QtGui.QApplication(sys.argv)

    app = otb_core.Registry.CreateApplication("TestApplication")

    widget = otb_gui.QParameterView(app)
    widget.CreateGui()
    widget.show()

    sys.exit(qapp.exec_())
