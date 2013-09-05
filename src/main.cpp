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

#include <QApplication>
#include <QtSingleApplication>
#include <QDataStream>
#include <QBuffer>
#include <iostream>

#include "qtnote.h"

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++) {
		QLatin1String v(argv[i]);
		if (v == "-h" || v == "--help") {
			std::cout << "QtNote - note taking application\n\n"
					  << " -n [type] - Create new note from 'type'. 'selection' type is the only supported.\n\n";
			return 0;
		}
	}

	QtSingleApplication a(argc, argv);
	if (a.isRunning()) {
		QStringList args = a.arguments();
		if (args.size() > 1) {
			args.pop_front();
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::ReadWrite);
			stream << args;
			a.sendMessage(dynamic_cast<QBuffer*>(stream.device())->data());
		}
		return 0;
	}

	Q_INIT_RESOURCE(main);

	//QCoreApplication::setOrganizationName("R-Soft"); // get rid of useless dirs
	QCoreApplication::setApplicationName("QtNote");

	QApplication::setQuitOnLastWindowClosed(false);

	QtNote qtnote;
	if (qtnote.isOperable()) {
		return a.exec();
	}
	return 1;
}
