#include "replacementgroup.h"

ReplacementGroup::ReplacementGroup()
{

}

ReplacementGroup::ReplacementGroup(const ReplacementGroup &other)
{
    for(int i=0; i<other.mReplacements.count(); i++)
    {
        mReplacements.append( new Replacement( *(other.mReplacements.at(i)) ) );
    }
}

QList<Replacement*> *ReplacementGroup::replacements()
{
    return &mReplacements;
}

const QList<Replacement*> *ReplacementGroup::replacements() const
{
    return &mReplacements;
}

void ReplacementGroup::serialize(QXmlStreamWriter *out) const
{
    out->writeStartElement("group");

    for(int i=0; i<mReplacements.count(); i++)
    {
        mReplacements.at(i)->serialize(out);
    }

    out->writeEndElement(); // group
}

int ReplacementGroup::indexFromReplacement(const Replacement *replacement)
{
    for(int i=0; i<mReplacements.count(); i++)
    {
        if( mReplacements.at(i) == replacement )
            return i;
    }
    return -1;
}

int ReplacementGroup::count() const
{
    return mReplacements.count();
}
