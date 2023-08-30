#include "replacementpass.h"

ReplacementPass::ReplacementPass()
{

}

ReplacementPass::ReplacementPass(const QString &name) : mName(name)
{

}

ReplacementPass::~ReplacementPass()
{
    qDeleteAll(mReplacementGroups);
}

QString ReplacementPass::name() const
{
    return mName;
}

void ReplacementPass::setName(const QString &name)
{
    mName = name;
}

int ReplacementPass::groupFromReplacement(const Replacement *replacement)
{
    for(int i=0; i<mReplacementGroups.count(); i++)
    {
        if( mReplacementGroups.at(i)->indexFromReplacement(replacement) != -1 )
            return i;
    }
    return -1;
}

void ReplacementPass::serialize(QXmlStreamWriter *out) const
{
    out->writeStartElement("pass");
    out->writeAttribute("name", mName);

    for(int i=0; i<mReplacementGroups.count(); i++)
    {
        mReplacementGroups.at(i)->serialize(out);
    }

    out->writeEndElement(); // pass
}

QList<ReplacementGroup *> *ReplacementPass::replacementGroups()
{
    return &mReplacementGroups;
}

int ReplacementPass::groupCount() const
{
    return mReplacementGroups.count();
}

int ReplacementPass::replacementCount() const
{
    int ct = 0;
    for(int i=0; i<mReplacementGroups.count(); i++)
    {
        ct += mReplacementGroups.at(i)->count();
    }
    return ct;
}

