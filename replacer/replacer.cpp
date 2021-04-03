#include "replacer.h"

#include <QtDebug>
#include <QFile>
#include <QXmlStreamReader>

#include <stdexcept>

Replacer::Replacer(const QString &schemaFile) : mIsOk(true)
{
    try {
        readSchemaFile(schemaFile);
    }  catch (const std::runtime_error& e) {
        mIsOk = false;
        qCritical() << e.what() << "(" << schemaFile << ")";
    }
}

void Replacer::performReplacement(const QString &inputPath, const QString &outputPath) const
{
    QFile inFile(inputPath);
    if( ! inFile.open( QFile::ReadOnly )  )
    {
        qCritical() << "Cannot open input path:" << inputPath;
        return;
    }

    QFile outFile(outputPath);
    if( ! outFile.open( QFile::WriteOnly )  )
    {
        qCritical() << "Cannot open output path:" << outputPath;
        return;
    }

    if( mMode == Replacer::Text )
    {
        performReplacementText(inFile, outFile);
    }
    else if( mMode == Replacer::Xml )
    {
        performReplacementXml(inFile, outFile);
    }
}

void Replacer::readSchemaFile(const QString &schemaFile)
{
    QFile file(schemaFile);
    if( file.open( QFile::ReadOnly ) )
    {
        QXmlStreamReader in(&file);

        in.readNextStartElement();
        if( in.name() != "schema" )
        {
            throw std::runtime_error( "Expected 'models' as a root element" );
        }
        if( in.attributes().value("mode") == "xml" )
        {
            mMode = Replacer::Xml;
        }
        else
        {
            mMode = Replacer::Text;
        }

        while(!in.atEnd())
        {
            in.readNext();

            if( in.isStartElement() )
            {
                QString name = in.name().toString();
                QXmlStreamAttributes attr = in.attributes();

                if ( name == "replacements" )
                {
                    mSchema.readReplacements( in.readElementText() );
                }
                else if ( name == "element" )
                {
                    const QString elementName = in.attributes().value("name").toString();
                    QHash<QString,QString> attributes;
                    while( ! ( name == "element" && in.isEndElement() ) )
                    {
                        /// read any attributes
                        in.readNext();
                        if( in.isStartElement() && in.name() == "attribute" )
                        {
                            const QString attr = in.attributes().value("name").toString();
                            const QString value = in.readElementText();
                            attributes.insert( attr, value );
                        }
                    }
                    mElements.insert( elementName, attributes );
                }
            }
        }
    }
}

void Replacer::performReplacementText(QFile &inFile, QFile &outFile) const
{
    QTextStream in( &inFile );
    in.setCodec("UTF-8");

    QTextStream out( &outFile );
    out.setCodec("UTF-8");

    while( !in.atEnd() )
    {
        QString line = in.readLine();
        out << mSchema.performReplacements( line ) << Qt::endl;
    }

    inFile.close();
    outFile.close();
}

void Replacer::performReplacementXml(QFile &inFile, QFile &outFile) const
{
    QXmlStreamReader in(&inFile);
    QXmlStreamWriter out(&outFile);

    while(!in.atEnd())
    {
        in.readNext();
        if( in.isStartElement() && matchesElement(in) )
        {
            out.writeCurrentToken( in ); /// the start element
            const QString elementText = in.readElementText();
            out.writeCharacters( mSchema.performReplacements(elementText) );
            out.writeCurrentToken( in ); /// the end element
        }
        else
        {
            out.writeCurrentToken( in );
        }
    }
}

bool Replacer::matchesElement(QXmlStreamReader &in) const
{
    const QString elementName = in.name().toString();
    if( mElements.contains( elementName ) )
    {
        QHashIterator<QString,QString> i( mElements.value(elementName) );
        while( i.hasNext() )
        {
            i.next();
            if( ! in.attributes().hasAttribute( i.key() ) || in.attributes().value( i.key() ).toString() != i.value() )
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool Replacer::isOk() const
{
    return mIsOk;
}
