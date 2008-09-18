HEADERS =   boat.h \
	model/positionmodel.h \
	model/boatmodel.h \
	model/trackmodel.h \
	model/markmodel.h \
	mark.h \
	track.h \
	trackwidget.h \
	model/situationmodel.h \
	situationscene.h \
	mainwindow.h \
	undocommands.h \
	xmlsituationreader.h \
	xmlsituationwriter.h

SOURCES =   boat.cpp \
	main.cpp \
	model/positionmodel.cpp \
	model/boatmodel.cpp \
	model/trackmodel.cpp \
	model/markmodel.cpp \
	mark.cpp \
	track.cpp \
	trackwidget.cpp \
	model/situationmodel.cpp \
	situationscene.cpp \
	mainwindow.cpp \
	undocommands.cpp \
	xmlsituationreader.cpp \
	xmlsituationwriter.cpp

CONFIG = qt debug warn_on

MOC_DIR = .moc/

OBJECTS_DIR = .obj/

TEMPLATE = app

TARGET = boats

