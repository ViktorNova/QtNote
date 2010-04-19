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

	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
		QMessageBox::critical(0, QObject::tr("QNote"),
							  QObject::tr("I couldn't detect any system tray "
										   "on this system."));
		return 1;
	}
	QApplication::setQuitOnLastWindowClosed(false);

	QCoreApplication::setApplicationName("QtNote");

	QList<NoteStorage*> storages;
#ifdef TOMBOY
	storages.append(new TomboyStorage(&a));
#endif
	storages.append(new PTFStorage(&a));

	while (storages.count()) {
		NoteStorage *storage = storages.takeFirst();
		if (storage->isAccessible()) {
			NoteManager::instance()->registerStorage(storage, true);
		} else {
			delete storage;
		}
	}
	if (!NoteManager::instance()->loadAll()) {
		qWarning("no one of note storages reported success. can't continue..");
		return 1;
	}

	Widget w;
    return a.exec();
}
