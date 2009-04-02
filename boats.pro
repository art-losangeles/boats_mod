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
	boatapplication.h \
	boats.h \
	commontypes.h \
	mainwindow.h \
	situationprint.h \
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
	boatapplication.cpp \
	boats.cpp \
	main.cpp \
	mainwindow.cpp \
	situationprint.cpp \
	situationwidget.cpp \
	trace.cpp \
	undocommands.cpp \
	xmlsituationreader.cpp \
	xmlsituationwriter.cpp

RESOURCES = boats.qrc

contains(GIF_EXPORT,1) {
	DEFINES += GIF_EXPORT
	HEADERS += gifwriter.h
	SOURCES += gifwriter.cpp
	LIBS += -lgif
}

TRANSLATIONS = locale/boats_fr.ts

isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

updateqm.input = TRANSLATIONS
updateqm.output = locale/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN}
updateqm.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += updateqm

PRE_TARGETDEPS += compiler_updateqm_make_all

MOC_DIR = .moc/

OBJECTS_DIR = .obj/

win32 {
	RC_FILE = boats.rc
}
