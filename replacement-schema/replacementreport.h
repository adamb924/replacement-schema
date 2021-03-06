#ifndef REPLACEMENTREPORT_H
#define REPLACEMENTREPORT_H

#include <QList>
#include <QPair>

class Replacement;

#include "replacement-schema_global.h"

class REPLACEMENTSCHEMA_EXPORT ReplacementEvent {
public:
    ReplacementEvent(const QString & passName, const Replacement * replacement, const QString & result );

    QString passName() const;

    const Replacement *replacement() const;

    QString result() const;

private:
    QString mPassName;
    const Replacement * mReplacement;
    QString mResult;
};

class REPLACEMENTSCHEMA_EXPORT ReplacementReport
{
public:
    ReplacementReport();

    const QList<ReplacementEvent> *replacementEvents() const;

    void addEvent(ReplacementEvent event);

private:
    QList<ReplacementEvent> mReplacementEvents;
};

#endif // REPLACEMENTREPORT_H
