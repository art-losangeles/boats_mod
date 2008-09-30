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
	track.h \
	trackwidget.h \
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
	track.cpp \
	trackwidget.cpp \
	undocommands.cpp \
	xmlsituationreader.cpp \
	xmlsituationwriter.cpp

CONFIG = qt debug warn_on

MOC_DIR = .moc/

OBJECTS_DIR = .obj/

TEMPLATE = app

TARGET = boats

