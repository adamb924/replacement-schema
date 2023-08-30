#ifndef REPLACEMENTGROUP_H
#define REPLACEMENTGROUP_H

#include "replacement.h"

#include "replacement-schema_global.h"

class REPLACEMENTSCHEMA_EXPORT ReplacementGroup
{
public:
    ReplacementGroup();
    ReplacementGroup( const ReplacementGroup & other );

    QList<Replacement*> *replacements();
    const QList<Replacement*> *replacements() const;

    void serialize(QXmlStreamWriter *out) const;

    int indexFromReplacement( const Replacement * replacement );

    int count() const;

private:
    QList<Replacement*> mReplacements;
};

#endif // REPLACEMENTGROUP_H
