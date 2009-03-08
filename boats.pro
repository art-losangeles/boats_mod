TEMPLATE = app
TARGET = boats

CONFIG += qt warn_on

INCLUDEPATH += model graphicsview itemviews

HEADERS = \
	model/boatmodel.h \
	model/markmodel.h \
	model/positionmodel.h \
	model/situationmodel.h \
	model/trackmodel.h \
	graphicsview/boat.h \
	graphicsview/boatanimation.h \
	graphicsview/mark.h \
	graphicsview/situationscene.h \
	graphicsview/situationview.h \
	graphicsview/track.h \
	itemviews/colorpickerwidget.h \
	itemviews/trackdelegate.h \
	itemviews/tracktablemodel.h \
	boats.h \
	commontypes.h \
	mainwindow.h \
	situationwidget.h \
	trace.h \
	undocommands.h \
	xmlsituationreader.h \
	xmlsituationwriter.h

SOURCES = \
	model/boatmodel.cpp \
	model/markmodel.cpp \
	model/positionmodel.cpp \
	model/situationmodel.cpp \
	model/trackmodel.cpp \
	graphicsview/boat.cpp \
	graphicsview/boatanimation.cpp \
	graphicsview/mark.cpp \
	graphicsview/situationscene.cpp \
	graphicsview/situationview.cpp \
	graphicsview/track.cpp \
	itemviews/colorpickerwidget.cpp \
	itemviews/trackdelegate.cpp \
	itemviews/tracktablemodel.cpp \
	boats.cpp \
	main.cpp \
	mainwindow.cpp \
	situationwidget.cpp \
	trace.cpp \
	undocommands.cpp \
	xmlsituationreader.cpp \
	xmlsituationwriter.cpp

RESOURCES = boats.qrc

TRANSLATIONS = locale/boats_fr.ts

MOC_DIR = .moc/

OBJECTS_DIR = .obj/

win32 {
	RC_FILE = boats.rc
}
