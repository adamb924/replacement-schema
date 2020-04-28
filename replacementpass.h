#ifndef REPLACEMENTPASS_H
#define REPLACEMENTPASS_H

#include <QList>
#include <QXmlStreamWriter>

#include "replacementgroup.h"

class ReplacementPass
{
public:
    ReplacementPass();
    ReplacementPass(const QString & name);
    ~ReplacementPass();

    QString name() const;
    void setName(const QString &name);

    int groupFromReplacement( const Replacement * replacement );

    void serialize(QXmlStreamWriter *out) const;

    QList<ReplacementGroup *> *replacementGroups();

private:
    QString mName;
    QList<ReplacementGroup*> mReplacementGroups;
};

#endif // REPLACEMENTPASS_H
