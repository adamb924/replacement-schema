#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include <QRegularExpression>
#include <QList>
#include <QXmlStreamWriter>

#include "replacement-schema_global.h"

class REPLACEMENTSCHEMA_EXPORT Replacement
{
public:
    Replacement();
    Replacement(const QRegularExpression & before, const QString & after, const QString & note = QString() );
    Replacement( const Replacement & other );

    QRegularExpression before() const;
    void setBefore(const QRegularExpression &before);
    void setBefore(const QString &before);

    QString after() const;
    void setAfter(const QString &after);

    QString note() const;
    void setNote(const QString &note);

    void serialize(QXmlStreamWriter *out) const;

    bool enabled() const;
    void setEnabled(bool enabled);

private:
    QRegularExpression mBefore;
    QString mAfter, mNote;
    bool mEnabled;
};

QDebug operator<<(QDebug dbg, const Replacement &key);

#endif // REPLACEMENT_H
