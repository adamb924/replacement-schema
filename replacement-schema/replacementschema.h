#ifndef REPLACEMENTSCHEMA_H
#define REPLACEMENTSCHEMA_H

#include <QList>

class ReplacementGroup;
#include "replacement.h"
#include "replacementpass.h"
#include "replacementreport.h"

#include "replacement-schema_global.h"

class REPLACEMENTSCHEMA_EXPORT ReplacementSchema
{
public:
    ReplacementSchema();
    ~ReplacementSchema();

    enum Level { PassObject, GroupObject, ReplacementObject, NoObject };

    QList<ReplacementPass*> *passes();
    const QList<ReplacementPass*> *passes() const;

    int indexFromPass( const ReplacementPass * pass );
    int passFromReplacement(const Replacement * replacement );
    int passFromReplacementGroup(ReplacementGroup *group ) const;

    void serialize(const QString & filename);
    void readReplacements(const QString & filename);

    QString performReplacements( const QString & original ) const;
    ReplacementReport *generateReport( const QString & original );

    bool changed() const;
    void setChanged(bool changed);

    //! This function should only be called on ReplacementGroups
    void moveReplacement(int oldPass, int oldReplacementGroup, int newPass, int newReplacementGroup);

    ReplacementSchema::Level getLevelFromPointer(void *p) const;

    QString summary() const;

private:
    QList<ReplacementPass*> mPasses;
    bool mChanged;
};

#endif // REPLACEMENTSCHEMA_H
