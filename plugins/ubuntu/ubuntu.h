#ifndef UBUNTUPLUGIN_H
#define UBUNTUPLUGIN_H

#include <QObject>

#include "qtnoteplugininterface.h"
#include "trayinterface.h"

class KAction;

namespace QtNote {

class UbuntuPlugin : public QObject, public QtNotePluginInterface, public TrayInterface
{
	Q_OBJECT
#if QT_VERSION >= 0x050000
	Q_PLUGIN_METADATA(IID "com.rion-soft.QtNote.Ubuntu")
#endif
	Q_INTERFACES(QtNote::QtNotePluginInterface QtNote::TrayInterface)
public:
	explicit UbuntuPlugin(QObject *parent = 0);

	virtual PluginMetadata metadata();
	bool init(Main *qtnote);

	TrayImpl* initTray(Main *qtnote);
};

} // namespace QtNote

#endif // UBUNTUPLUGIN_H
