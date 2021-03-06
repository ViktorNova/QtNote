#ifndef SHORTCUTSMANAGER_H
#define SHORTCUTSMANAGER_H

#include <QObject>
#include <QHash>
#include <QKeySequence>

class QAction;

namespace QtNote {

class GlobalShortcutsInterface;

class ShortcutsManager : public QObject
{
	Q_OBJECT

public:

	class ShortcutInfo
	{
	public:
		QString option;
		QString name;
		QKeySequence key;
	};

	static const char *SKNoteFromSelection;

	explicit ShortcutsManager(GlobalShortcutsInterface *gs, QObject *parent = 0);
	//const QMap<QString, QString> &optionsMap() const;
	//QAction *shortcut(const QLatin1String &option);
	QKeySequence key(const QString &option) const;
	bool setKey(const QString &option, const QKeySequence &seq);
	QList<ShortcutInfo> all() const;
	QString friendlyName(const QString &option) const;

	bool registerGlobal(const char *option, QObject *receiver, const char *slot);
	
signals:
	
public slots:

private:
	GlobalShortcutsInterface *gs;
};

} // namespace QtNote

#endif // SHORTCUTSMANAGER_H
