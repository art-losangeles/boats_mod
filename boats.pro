HEADERS =   boat.h \
	model/boatmodel.h \
	model/trackmodel.h \
	trackwidget.h \
	model/situationmodel.h \
	situationscene.h \
	mainwindow.h \
	undocommands.h

SOURCES =   boat.cpp \
	main.cpp \
	model/boatmodel.cpp \
	model/trackmodel.cpp \
	trackwidget.cpp \
	model/situationmodel.cpp \
	situationscene.cpp \
	mainwindow.cpp \
	undocommands.cpp

CONFIG = qt debug warn_on

TEMPLATE = app

TARGET = boats

