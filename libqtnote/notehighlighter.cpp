#include <QSet>

#include "notehighlighter.h"
#include "noteedit.h"

namespace QtNote {

NoteHighlighter::NoteHighlighter(NoteEdit *nde) :
	QSyntaxHighlighter(nde)
{
}

void NoteHighlighter::highlightBlock(const QString &text)
{
	formats.clear();
	QMutableListIterator<QWeakPointer<HighlighterExtension>> it(extensions);
	while (it.hasNext()) {
		HighlighterExtension::Ptr ext = it.next().toStrongRef();
		if (ext) {
			ext->highlight(this, text);
		} else {
			it.remove();
		}
	}

	QSet<int> bps;

	for (int i = 0; i < formats.count(); i++) {
		bps.insert(formats[i].start);
		bps.insert(formats[i].start + formats[i].count);
	}

	QList<int> bpl = bps.toList();
	qSort(bpl);
	for (int i = 0; i < bpl.count() - 1; i++) {
		int count = bpl[i + 1] - bpl[i];
		QTextCharFormat format;
		bool merged = false;
		foreach (auto &f, formats) {
			if (f.start <= bpl[i] && (f.start + f.count) >= bpl[i + 1]) {
				format.merge(f.format);
				merged = true;
			}
		}
		if (merged) {
			setFormat(bpl[i], count, format);
		}
	}
}

void NoteHighlighter::addFormat(int start, int count, const QTextCharFormat &format)
{
	formats.append({start, count, format});
}

} // namespace QtNote
