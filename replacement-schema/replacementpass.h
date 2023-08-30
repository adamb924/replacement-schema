#ifndef REPLACEMENTPASS_H
#define REPLACEMENTPASS_H

#include <QList>
#include <QXmlStreamWriter>

#include "replacementgroup.h"

#include "replacement-schema_global.h"

class REPLACEMENTSCHEMA_EXPORT ReplacementPass
{
public:
    ReplacementPass();
    explicit ReplacementPass(const QString & name);
    ~ReplacementPass();

    QString name() const;
    void setName(const QString &name);

    int groupFromReplacement( const Replacement * replacement );

    void serialize(QXmlStreamWriter *out) const;

    QList<ReplacementGroup *> *replacementGroups();

    int groupCount() const;
    int replacementCount() const;

private:
    QString mName;
    QList<ReplacementGroup*> mReplacementGroups;
};

#endif // REPLACEMENTPASS_H
