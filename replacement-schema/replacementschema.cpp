#include "replacementschema.h"

#include <QXmlStreamWriter>
#include <QtDebug>
#include <QFile>

ReplacementSchema::ReplacementSchema() : mChanged(false)
{

}

ReplacementSchema::~ReplacementSchema()
{
    qDeleteAll(mPasses);
}

QList<ReplacementPass*> *ReplacementSchema::passes()
{
    return &mPasses;
}

const QList<ReplacementPass*> *ReplacementSchema::passes() const
{
    return &mPasses;
}

int ReplacementSchema::indexFromPass(const ReplacementPass *pass)
{
    for(int i=0; i<mPasses.count(); i++)
    {
        if( mPasses.at(i) == pass )
            return i;
    }
    return -1;
}

int ReplacementSchema::passFromReplacement(const Replacement *replacement)
{
    for(int i=0; i<mPasses.count(); i++)
    {
        if( mPasses.at(i)->groupFromReplacement(replacement) != -1 )
            return i;
    }
    return -1;
}

int ReplacementSchema::passFromReplacementGroup(ReplacementGroup *group) const
{
    for(int i=0; i<mPasses.count(); i++)
    {
        int index = mPasses.at(i)->replacementGroups()->indexOf(group);
        if( index != -1 )
            return i;
    }
    return -1;
}

void ReplacementSchema::serialize(const QString &filename)
{
    QFile file(filename);
    if( file.open( QFile::WriteOnly ) )
    {
        QXmlStreamWriter out(&file);
        out.setCodec("UTF-8");
        out.setAutoFormatting(true);

        out.writeStartDocument();
        out.writeStartElement("schema");

        for(int i=0; i<mPasses.count(); i++)
        {
            mPasses.at(i)->serialize(&out);
        }

        out.writeEndElement(); // schema

        /// because it's just been written
        mChanged = false;
    }
    else
    {
        qDebug() << "Could not open: " << filename;
    }
}

void ReplacementSchema::readReplacements(const QString &filename)
{
    QFile file(filename);
    if( file.open( QFile::ReadOnly ) )
    {
        QXmlStreamReader in(&file);

        while(!in.atEnd())
        {
            in.readNext();

            if( in.tokenType() == QXmlStreamReader::StartElement )
            {
                QString name = in.name().toString();
                if( name == "pass" )
                {
                    mPasses.append( new ReplacementPass );
                    if( in.attributes().hasAttribute("name") )
                    {
                        mPasses.last()->setName( in.attributes().value("name").toString() );
                    }
                }
                else if ( name == "group" )
                {
                    mPasses.last()->replacementGroups()->append( new ReplacementGroup );
                }
                else if ( name == "replacement" )
                {
                    mPasses.last()->replacementGroups()->last()->replacements()->append( new Replacement );
                }
                else if ( name == "before" )
                {
                    mPasses.last()->replacementGroups()->last()->replacements()->last()->setBefore( in.readElementText() );
                }
                else if ( name == "after" )
                {
                    mPasses.last()->replacementGroups()->last()->replacements()->last()->setAfter( in.readElementText() );
                }
                else if ( name == "note" )
                {
                    mPasses.last()->replacementGroups()->last()->replacements()->last()->setNote( in.readElementText() );
                }
                else if ( name == "enabled" )
                {
                    mPasses.last()->replacementGroups()->last()->replacements()->last()->setEnabled( in.readElementText() == "true" );
                }
            }
        }

        /// it's just been read
        mChanged = false;
    }
    else
    {
        qDebug() << "Could not open: " << filename;
    }
}


QString ReplacementSchema::performReplacements(const QString &original) const
{
    QString result = original;
    for(int i=0; i < mPasses.count(); i++)
    {
        for(int j=0; j < mPasses.at(i)->replacementGroups()->count(); j++)
        {
            for(int k=0; k < mPasses.at(i)->replacementGroups()->at(j)->replacements()->count(); k++)
            {
                Replacement * r = mPasses.at(i)->replacementGroups()->at(j)->replacements()->at(k);
                if( r->enabled() )
                {
                    result = result.replace( r->before(), r->after() );
                }
            }
        }
    }
    return result;
}

ReplacementReport* ReplacementSchema::generateReport(const QString &original)
{
    ReplacementReport *report = new ReplacementReport;
    report->addEvent( ReplacementEvent( "<initial>", nullptr, original ) );

    QString result = original;
    QString previous = result;
    for(int i=0; i < mPasses.count(); i++)
    {
        for(int j=0; j < mPasses.at(i)->replacementGroups()->count(); j++)
        {
            for(int k=0; k < mPasses.at(i)->replacementGroups()->at(j)->replacements()->count(); k++)
            {
                Replacement * r = mPasses.at(i)->replacementGroups()->at(j)->replacements()->at(k);
                if( r->enabled() )
                {
                    result = result.replace( r->before(), r->after() );
                    if( result != previous )
                    {
                        report->addEvent( ReplacementEvent( mPasses.at(i)->name(), r, result ) );
                    }
                    previous = result;
                }
            }
        }
    }
    return report;
}


bool ReplacementSchema::changed() const
{
    return mChanged;
}

void ReplacementSchema::setChanged(bool changed)
{
    mChanged = changed;
}

void ReplacementSchema::moveReplacement(int oldPass, int oldReplacementGroup, int newPass, int newReplacementGroup)
{
    qDebug() << oldPass << oldReplacementGroup << newPass << newReplacementGroup;

    mPasses[newPass]->replacementGroups()->insert( newReplacementGroup, mPasses.at(oldPass)->replacementGroups()->at(oldReplacementGroup)  );
//    int replacementToRemove = oldReplacement;
//    if( oldPass == newPass && oldReplacement > newReplacement )
//    {
//        replacementToRemove++;
//    }
    //    mPasses[oldPass]->replacements()->removeAt(replacementToRemove);
}

ReplacementSchema::Level ReplacementSchema::getLevelFromPointer(void *p) const
{
    if( mPasses.contains( static_cast<ReplacementPass*>(p) ) )
    {
        return ReplacementSchema::PassObject;
    }
    else
    {
        for(int i=0; i < mPasses.count(); i++)
        {
            if( mPasses.at(i)->replacementGroups()->contains( static_cast<ReplacementGroup*>(p) ) )
            {
                return ReplacementSchema::GroupObject;
            }
            for(int j=0; j<mPasses.at(i)->replacementGroups()->count(); j++ )
            {
                if( mPasses.at(i)->replacementGroups()->at(j)->replacements()->contains( static_cast<Replacement*>(p) ) )
                {
                    return ReplacementSchema::ReplacementObject;
                }
            }
        }
    }
    return ReplacementSchema::NoObject;
}
