TEMPLATE = app
TARGET = boats

CONFIG += qt warn_on

HEADERS = \
	model/boatmodel.h \
	model/markmodel.h \
	model/positionmodel.h \
	model/situationmodel.h \
	model/trackmodel.h \
	boat.h \
	boatanimation.h \
	mainwindow.h \
	mark.h \
	situationscene.h \
	situationwidget.h \
	trace.h \
	track.h \
	undocommands.h \
	xmlsituationreader.h \
	xmlsituationwriter.h

SOURCES = \
	model/boatmodel.cpp \
	model/markmodel.cpp \
	model/positionmodel.cpp \
	model/situationmodel.cpp \
	model/trackmodel.cpp \
	boat.cpp \
	boatanimation.cpp \
	main.cpp \
	mainwindow.cpp \
	mark.cpp \
	situationscene.cpp \
	situationwidget.cpp \
	trace.cpp \
	track.cpp \
	undocommands.cpp \
	xmlsituationreader.cpp \
	xmlsituationwriter.cpp

RESOURCES = boats.qrc

MOC_DIR = .moc/

OBJECTS_DIR = .obj/
