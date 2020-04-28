#ifndef REPLACEMENTGROUP_H
#define REPLACEMENTGROUP_H

#include "replacement.h"

class ReplacementGroup
{
public:
    ReplacementGroup();
    ReplacementGroup( const ReplacementGroup & other );

    QList<Replacement*> *replacements();
    const QList<Replacement*> *replacements() const;

    void serialize(QXmlStreamWriter *out) const;

    int indexFromReplacement( const Replacement * replacement );

private:
    QList<Replacement*> mReplacements;
};

#endif // REPLACEMENTGROUP_H
