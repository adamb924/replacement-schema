#include "replacer.h"

#include <QtDebug>
#include <QFile>
#include <QXmlStreamReader>

#include <stdexcept>

Replacer::Replacer(const QString &schemaFile) : mIsOk(true)
{
    try {
        mSchema.readReplacements(schemaFile);
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
        const QString message = "Cannot open input path: " + inputPath;
        throw std::runtime_error( message.toStdString() );
    }

    QFile outFile(outputPath);
    if( ! outFile.open( QFile::WriteOnly )  )
    {
        const QString message = "Cannot open output path: " + outputPath;
        throw std::runtime_error( message.toStdString() );
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

const ReplacementSchema *Replacer::schema() const
{
    return &mSchema;
}

bool Replacer::isOk() const
{
    return mIsOk;
}
