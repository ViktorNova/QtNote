/*
QtNote - Simple note-taking application
Copyright (C) 2010 Ili'nykh Sergey

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Contacts:
E-Mail: rion4ik@gmail.com XMPP: rion@jabber.ru
*/

#include <QtGui/QApplication>
#include <QMessageBox>
#include <QLocale>
#include <QTranslator>
#include <QFileInfo>
#include <QSettings>
#include <QLibraryInfo>
#include "mainwidget.h"
#include "notemanager.h"
#ifdef TOMBOY
#include "tomboystorage.h"
#endif
#include "ptfstorage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


	Q_INIT_RESOURCE(main);

	QCoreApplication::setOrganizationName("R-Soft");
	QCoreApplication::setApplicationName("QtNote");

	QApplication::setQuitOnLastWindowClosed(false);

	// loading localization
	QString locale = QLocale::system().name();
	QTranslator translator;
	if (!translator.load(QString("langs/qtnote_") + locale)) {
#ifdef Q_OS_UNIX
		if (!translator.load(QString(TRANSLATIONS_DIR) + "/qtnote_" + locale))
#endif
			qDebug("failed to load translation");
	}
	a.installTranslator(&translator);
	QTranslator qtTranslator;
#ifdef Q_OS_UNIX
	if (!qtTranslator.load(QLibraryInfo::location(QLibraryInfo::TranslationsPath)
		+ "/qt_" + locale))
#endif
		qtTranslator.load(QString("langs/qt_") + locale);
	a.installTranslator(&qtTranslator);


	// itialzation of notes storages
	QList<NoteStorage*> storages;
	QStringList priorities = QSettings().value("storage.priority")
							 .toStringList();
	QStringList prioritiesR;
	while (priorities.count()) {
		prioritiesR.append(priorities.takeLast());
	}

#ifdef TOMBOY
	storages.append(new TomboyStorage(&a));
#endif
	storages.append(new PTFStorage(&a));

	while (storages.count()) {
		NoteStorage *storage = storages.takeFirst();
		if (storage->isAccessible()) {
			int priority = prioritiesR.indexOf(storage->systemName());
			NoteManager::instance()->registerStorage(storage,
													 priority >= 0? priority:0);
		} else {
			delete storage;
		}
	}
	if (!NoteManager::instance()->loadAll()) {
		QMessageBox::critical(0, "QtNote", QObject::tr("no one of note "
							  "storages is accessible. can't continue.."));
		return 1;
	}

	Widget w;
    return a.exec();
}