#include "replacementreport.h"

ReplacementEvent::ReplacementEvent(const QString &passName, const Replacement *replacement, const QString &result) :
    mPassName(passName),
    mReplacement(replacement),
    mResult(result)
{

}

QString ReplacementEvent::passName() const
{
    return mPassName;
}

const Replacement *ReplacementEvent::replacement() const
{
    return mReplacement;
}

QString ReplacementEvent::result() const
{
    return mResult;
}

ReplacementReport::ReplacementReport()
{

}

const QList<ReplacementEvent> *ReplacementReport::replacementEvents() const
{
    return &mReplacementEvents;
}

void ReplacementReport::addEvent(ReplacementEvent event)
{
    mReplacementEvents << event;
}
