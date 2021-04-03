#include "replacement.h"

#include <QtDebug>

Replacement::Replacement() : mEnabled(true)
{
}

Replacement::Replacement(const QRegularExpression &before, const QString &after, const QString &note) : mBefore(before), mAfter(after), mNote(note), mEnabled(true)
{
    /// make sure this is always set correctly
    mBefore.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::CaseInsensitiveOption);
}

Replacement::Replacement(const Replacement &other)
    : mBefore(other.before()),
      mAfter(other.after()),
      mNote(other.note()),
      mEnabled(other.mEnabled)
{
}

QRegularExpression Replacement::before() const
{
    return mBefore;
}

void Replacement::setBefore(const QRegularExpression &before)
{
    mBefore = before;
    /// make sure this is always set correctly
    mBefore.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::CaseInsensitiveOption);
}

void Replacement::setBefore(const QString &before)
{
    mBefore.setPattern( before );
    /// make sure this is always set correctly
    mBefore.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption | QRegularExpression::CaseInsensitiveOption);
}

QString Replacement::after() const
{
    return mAfter;
}

void Replacement::setAfter(const QString &after)
{
    mAfter = after;
}

QString Replacement::note() const
{
    return mNote;
}

void Replacement::setNote(const QString &note)
{
    mNote = note;
}

void Replacement::serialize(QXmlStreamWriter *out) const
{
    out->writeStartElement("replacement");

    out->writeTextElement("before",mBefore.pattern());
    out->writeTextElement("after", mAfter);
    out->writeTextElement("note", mNote);
    out->writeTextElement("enabled", mEnabled ? "true" : "false" );

    out->writeEndElement(); // replacement
}

bool Replacement::enabled() const
{
    return mEnabled;
}

void Replacement::setEnabled(bool enabled)
{
    mEnabled = enabled;
}

QDebug operator<<(QDebug dbg, const Replacement &key)
{
    dbg.nospace() << "Replacement(Before: " << key.before() << ", After: " << key.after() << ")";
    return dbg.maybeSpace();
}
