# -*- coding: utf-8 -*-

from PyQt4 import QtCore, QtGui
import otbApplication
from widgets import QParameterGroup

class QParameterModel(QtCore.QObject):
    def __init__(self, app):
        super(QParameterModel, self).__init__()
        self._app = app
        print self._app.GetName()
        print self._app.GetDescription()
        
    def GetApplication(self):
        return self._app
    
    def NotifyUpdate(self):
        self._app.UpdateParameters()

    def Execute(self):
        self._app.ExecuteAndWriteOutput()
        # todo : write output

class QParameterView(QtGui.QMainWindow):
    def __init__(self, app, parent = None):
        super(QParameterView, self).__init__(parent)
        self._app = app
        self._model = QParameterModel(app)

    def CreateGui(self):
        mainLayout = QtGui.QVBoxLayout()
        mainLayout.addWidget(self.CreateHeader())
        mainLayout.addWidget(self.CreateInputWidgets())
        mainLayout.addWidget(self.CreateFooter())
        
        group = QtGui.QGroupBox()
        group.setLayout(mainLayout)
        
        scroll = QtGui.QScrollArea()
        scroll.setWidget(group)
        scroll.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOn)
        scroll.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOn)
        
        scrollLayout = QtGui.QVBoxLayout()
        scrollLayout.addWidget(scroll)
        
        self.setCentralWidget(scroll)
        self.setWindowTitle(self._app.GetName())
        
        self.resize(group.sizeHint() + scroll.verticalScrollBar().size())
        
    def CreateHeader(self):
        h = QtGui.QHBoxLayout()
        g = QtGui.QGroupBox()
        g.setStyleSheet("border: 1px solid gray")
        g.setFixedHeight(50)
        g.setContentsMargins(0,0,0,0)
        h.setContentsMargins(5,5,5,5)
        
        description = QtGui.QLabel()
        description.setStyleSheet("border-style: none")
        description.setText(self._app.GetDescription())
        
        h.addWidget(description)
        g.setLayout(h)
        
        return g
        
    def CreateInputWidgets(self):
        group = QParameterGroup(self._model)
        group.CreateWidget()
        return group

    def CreateFooter(self):
        h = QtGui.QHBoxLayout()
        g = QtGui.QGroupBox()

        g.setFixedHeight(40)
        g.setContentsMargins(0,0,0,0)
        h.setContentsMargins(5,5,5,5)
        
        self._execButton = QtGui.QPushButton(g)
        self._execButton.setDefault(True)
        self._execButton.setText('Execute')
        self.connect(self._execButton, QtCore.SIGNAL('clicked()'), self._model.Execute)
    
        self._quitButton = QtGui.QPushButton(g)
        self._quitButton.setDefault(True)
        self._quitButton.setText('Quit')
        self.connect(self._quitButton, QtCore.SIGNAL('clicked()'), self.close)
 
        h.addStretch()
        h.addWidget(self._execButton)
        h.addWidget(self._quitButton)
        g.setLayout(h)
        
        return g