#include "replacer.h"

#include <QtDebug>
#include <QFile>
#include <QXmlStreamReader>

#include <stdexcept>

Replacer::Replacer(const QString &schemaFile) : mMode(Replacer::Text), mIsOk(true)
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
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    in.setEncoding( QStringConverter::Utf8 );
#else
    in.setCodec("UTF-8");
#endif

    QTextStream out( &outFile );
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    out.setEncoding( QStringConverter::Utf8 );
#else
    out.setCodec("UTF-8");
#endif

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
        if( in.isCharacters() )
        {
            out.writeCharacters( mSchema.performReplacements( in.text().toString() ) );
        }
        else
        {
            out.writeCurrentToken(in);
        }
    }
}

void Replacer::setMode(Mode newMode)
{
    mMode = newMode;
}

const ReplacementSchema *Replacer::schema() const
{
    return &mSchema;
}

bool Replacer::isOk() const
{
    return mIsOk;
}
