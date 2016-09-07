find_package( Qt4 REQUIRED QtCore QtGui QtOpenGL QtXml )
mark_as_advanced( QT_QMAKE_EXECUTABLE )
set( QT_USE_QTXML 1 )
include( ${QT_USE_FILE} )
