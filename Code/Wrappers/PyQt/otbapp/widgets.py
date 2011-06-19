# -*- coding: utf-8 -*-

from PyQt4 import QtCore, QtGui
import otbApplication

class QParameterBase(QtGui.QWidget):
    def __init__(self, model):
        super(QParameterBase, self).__init__()
        self._model = model

    def CreateWidget(self):
        self.DoCreateWidget()
    
    def UpdateGUI(self):
        self.DoUpdateGUI()

    def ParameterChanged(self):
        pass
        
    def GetModel(self):
        return self._model

class QParameterLabel(QParameterBase):
    def __init__(self, model, paramKey):
        super(QParameterLabel, self).__init__(model)
        
        self.label = QtGui.QLabel()
        app = self.GetModel().GetApplication()
        self.label.setText( app.GetParameterName(paramKey))
        self.label.setToolTip( app.GetParameterDescription(paramKey) )
        print self.label.toolTip()
        self.labelLayout = QtGui.QVBoxLayout()
        self.labelLayout.setSpacing(0)
        self.labelLayout.setContentsMargins(0,0,0,0)
        self.labelLayout.addWidget(self.label)
        print self.labelLayout.spacing()
        self.setLayout(self.labelLayout)
        self.setWindowTitle('box layout')

class QParameterEmpty(QParameterBase):
    def __init__(self, model, paramKey):
        super(QParameterEmpty, self).__init__(model)
        self._paramKey = paramKey

    def DoCreateWidget(self):
        app = self.GetModel().GetApplication()
        h = QtGui.QHBoxLayout()
        h.setSpacing(0)
        h.setContentsMargins(0,0,0,0)
        spin = QtGui.QCheckBox()
        spin.setToolTip(app.GetParameterDescription(self._paramKey))
        self.connect(spin, QtCore.SIGNAL('stateChanged(int)'), self.SetValue)
        h.addWidget(spin)
        h.addStretch()
        self.setLayout(h)
        
    def SetValue(self, val):
        app = self.GetModel().GetApplication()
        if val:
            app.EnableParameter(self._paramKey)
        else:
            app.DisableParameter(self._paramKey)


class QParameterInt(QParameterBase):
    def __init__(self, model, paramKey):
        super(QParameterInt, self).__init__(model)
        self._paramKey = paramKey

    def DoCreateWidget(self):
        app = self.GetModel().GetApplication()
        
        h = QtGui.QHBoxLayout()
        h.setSpacing(0)
        h.setContentsMargins(0,0,0,0)
        
        spin = QtGui.QSpinBox()
        spin.setToolTip(app.GetParameterDescription(self._paramKey))
        
        self.connect(spin, QtCore.SIGNAL('valueChanged(int)'), self.SetValue)
        
        h.addWidget(spin)
        h.addStretch()
        
        self.setLayout(h)
        
    def SetValue(self, val):
        print val
        app = self.GetModel().GetApplication()
        app.SetParameterInt(self._paramKey, val)


class QParameterChoice(QParameterBase):
    def __init__(self, model, paramKey):
        super(QParameterChoice, self).__init__(model)
        self._paramKey = paramKey
    
    def DoCreateWidget(self):
        print 'QParameterChoice DoCreateWidget'
        app = self.GetModel().GetApplication()
        
        layout = QtGui.QVBoxLayout()
        
        
        combo = QtGui.QComboBox()
        combo.setToolTip(app.GetParameterDescription(self._paramKey))
        
        stack = QtGui.QStackedWidget()
        
        allparams = app.GetParametersKeys()
        print 'allparams = ' + str(allparams)
        subgroups = set()
        for paramKey in allparams:
            if paramKey.startswith(self._paramKey):
                choiceSubParam = paramKey.partition(self._paramKey + '.')[2].partition('.')[0]
                if choiceSubParam:
                    subgroups.add(choiceSubParam.partition('.')[0])
        print str(subgroups)
        
        for subgroup in subgroups:
            combo.addItem(subgroup, subgroup)
            widget = QParameterGroup(self.GetModel(), self._paramKey + '.' + subgroup)
            if widget:
                widget.CreateWidget()
                stack.addWidget(widget)
            

        layout.addWidget(combo)
        layout.addWidget(stack)
        self.setLayout(layout)
        
    def SetValue(self, val):
        app = self.GetModel().GetApplication()
        app.SetParameterInt(self._paramKey, val)

class QParameterFloat(QParameterBase):
    def __init__(self, model, paramKey):
        super(QParameterFloat, self).__init__(model)
        self._paramKey = paramKey
        
    def DoCreateWidget(self):
        app = self.GetModel().GetApplication()
        
        h = QtGui.QHBoxLayout()
        h.setSpacing(0)
        h.setContentsMargins(0,0,0,0)
        
        spin = QtGui.QDoubleSpinBox()
        spin.setDecimals(5)
        spin.setToolTip(app.GetParameterDescription(self._paramKey))
        
        self.connect(spin, QtCore.SIGNAL('valueChanged(double)'), self.SetValue)
        
        h.addWidget(spin)
        h.addStretch()
        
        self.setLayout(h)
        
    def SetValue(self, val):
        print val
        app = self.GetModel().GetApplication()
        app.SetParameterFloat(self._paramKey, val)


class QParameterString(QParameterBase):
    def __init__(self, model, paramKey):
        super(QParameterString, self).__init__(model)
        self._paramKey = paramKey
        
    def DoCreateWidget(self):
        app = self.GetModel().GetApplication()
        
        h = QtGui.QHBoxLayout()
        h.setSpacing(0)
        h.setContentsMargins(0,0,0,0)
        
        self._line = QtGui.QLineEdit()
        self._line.setToolTip(app.GetParameterDescription(self._paramKey))
        self.connect(self._line, QtCore.SIGNAL('textChanged(const QString&)'), self.SetValue)
        
        h.addWidget(self._line)
        self.setLayout(h)
        
    def SetValue(self, val):
        print val
        app = self.GetModel().GetApplication()
        app.SetParameterString(self._paramKey, str(val))


class QParameterInputFilename(QParameterBase):
    def __init__(self, model, paramKey):
        super(QParameterInputFilename, self).__init__(model)
        self._paramKey = paramKey
        
    def DoCreateWidget(self):
        app = self.GetModel().GetApplication()
        
        h = QtGui.QHBoxLayout()
        h.setSpacing(0)
        h.setContentsMargins(0,0,0,0)
        
        self._line = QtGui.QLineEdit()
        self._line.setToolTip(app.GetParameterDescription(self._paramKey))
        self.connect(self._line, QtCore.SIGNAL('textChanged(const QString&)'), self.SetFilename)
        
        self._button = QtGui.QPushButton()
        self._button.setText('...')
        self._button.setToolTip('Select file...')
        self._button.setMaximumWidth(self._button.width())
        self.connect(self._button, QtCore.SIGNAL('clicked()'), self.SelectFile)
        
        h.addWidget(self._line)
        h.addWidget(self._button)
        
        self.setLayout(h)
        
    def SetFilename(self, val):
        print val
        app = self.GetModel().GetApplication()
        
        app.SetParameterString(self._paramKey, str(val))

    def SelectFile(self):
        dialog = QtGui.QFileDialog()
        dialog.setConfirmOverwrite(True)
        dialog.setFileMode(QtGui.QFileDialog.ExistingFile)
        dialog.setNameFilter('Raster files (*)')
        if dialog.exec_() :
            self._line.setText(dialog.selectedFiles()[0])

class QParameterDirectory(QParameterBase):
    def __init__(self, model, paramKey):
        super(QParameterDirectory, self).__init__(model)
        self._paramKey = paramKey
        
    def DoCreateWidget(self):
        app = self.GetModel().GetApplication()
        
        h = QtGui.QHBoxLayout()
        h.setSpacing(0)
        h.setContentsMargins(0,0,0,0)
        
        self._line = QtGui.QLineEdit()
        self._line.setToolTip(app.GetParameterDescription(self._paramKey))
        self.connect(self._line, QtCore.SIGNAL('textChanged(const QString&)'), self.SetDirname)
        
        self._button = QtGui.QPushButton()
        self._button.setText('...')
        self._button.setToolTip('Select directory...')
        self._button.setMaximumWidth(self._button.width())
        self.connect(self._button, QtCore.SIGNAL('clicked()'), self.SelectDir)
        
        h.addWidget(self._line)
        h.addWidget(self._button)
        
        self.setLayout(h)
        
    def SetDirname(self, val):
        print val
        app = self.GetModel().GetApplication()
        app.SetParameterString(self._paramKey, str(val))

    def SelectDir(self):
        dialog = QtGui.QFileDialog()
        dialog.setConfirmOverwrite(True)
        dialog.setFileMode(QtGui.QFileDialog.Directory)
        dialog.setOption(QtGui.QFileDialog.ShowDirsOnly, True)
        dialog.setNameFilter('Raster files (*)')
        if dialog.exec_() :
            self._line.setText(dialog.selectedFiles()[0])

class QParameterOutputImage(QParameterBase):
    def __init__(self, model, paramKey):
        super(QParameterOutputImage, self).__init__(model)
        self._paramKey = paramKey
        
    def DoCreateWidget(self):
        app = self.GetModel().GetApplication()
        
        h = QtGui.QHBoxLayout()
        h.setSpacing(0)
        h.setContentsMargins(0,0,0,0)
        
        self._line = QtGui.QLineEdit()
        self._line.setToolTip(app.GetParameterDescription(self._paramKey))
        self.connect(self._line, QtCore.SIGNAL('textChanged(const QString&)'), self.SetFilename)
        
        self._button = QtGui.QPushButton()
        self._button.setText('...')
        self._button.setToolTip('Select output file...')
        self._button.setMaximumWidth(self._button.width())
        self.connect(self._button, QtCore.SIGNAL('clicked()'), self.SelectFile)
        
        h.addWidget(self._line)
        h.addWidget(self._button)
        
        self.setLayout(h)
        
    def SetFilename(self, val):
        print val
        app = self.GetModel().GetApplication()
        app.SetParameterString(self._paramKey, str(val))

    def SelectFile(self):
        dialog = QtGui.QFileDialog()
        dialog.setConfirmOverwrite(True)
        dialog.setFileMode(QtGui.QFileDialog.AnyFile)
        dialog.setNameFilter('Raster files (*)')
        if dialog.exec_() :
            self._line.setText(dialog.selectedFiles()[0])

class QParameterGroup(QParameterBase):
    def __init__(self, model, paramKey = None):
        super(QParameterGroup, self).__init__(model)
        self._paramKey = paramKey
        if paramKey:
            print 'QParameterGroup ' + paramKey

    def DoUpdateGUI(self):
        pass

    def DoCreateWidget(self):
        print "QParameterGroup DoCreateWidget"
        grid = QtGui.QGridLayout()
        grid.setSpacing(1)
        grid.setContentsMargins(0,0,0,0)

        defaultWidget = QParameterInt
        paramTypeToWidget = {
            otbApplication.ParameterType_Choice : QParameterInt,
            otbApplication.ParameterType_Empty : QParameterEmpty,
            otbApplication.ParameterType_Int : QParameterInt,
            otbApplication.ParameterType_Float : QParameterFloat,
            otbApplication.ParameterType_String : QParameterString,
            otbApplication.ParameterType_Filename : QParameterInputFilename,
            otbApplication.ParameterType_Directory : QParameterDirectory,
            otbApplication.ParameterType_Choice : QParameterChoice,
            otbApplication.ParameterType_InputImage : QParameterInputFilename,
            otbApplication.ParameterType_InputComplexImage : QParameterInputFilename,
            otbApplication.ParameterType_InputVectorData : QParameterInputFilename,
            otbApplication.ParameterType_OutputImage : QParameterOutputImage,
            otbApplication.ParameterType_OutputVectorData : QParameterOutputImage, # todo
            otbApplication.ParameterType_Radius : QParameterInt,
            otbApplication.ParameterType_Group : QParameterGroup  
        }

        app = self.GetModel().GetApplication()
        if self._paramKey:
            keys = [key for key in app.GetParametersKeys(True) if key and key.startswith(self._paramKey)]
        else:
            keys = app.GetParametersKeys(False)
            
        i = 0
        for key in keys:
            widgetClass = paramTypeToWidget[app.GetParameterType(key)]
            print "widgetClass " + str(widgetClass)
            grid.addWidget( QParameterLabel(self._model, key), i, 0 )
            w = widgetClass(self._model, key)
            w.CreateWidget()
            grid.addWidget(w, i, 1 )
            i = i+1
        
        self.setLayout(grid)
