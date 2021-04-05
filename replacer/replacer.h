#ifndef REPLACER_H
#define REPLACER_H

class QString;
class QFile;

#include "replacementschema.h"

class Replacer
{
public:
    enum Mode { Text, Xml };

    Replacer(const QString & schemaFile);

    void performReplacement(const QString & inputPath, const QString & outputPath) const;

    bool isOk() const;

private:
    void readSchemaFile(const QString & schemaFile);

    void performReplacementText(QFile &inFile, QFile &outFile) const;
    void performReplacementXml(QFile &inFile, QFile &outFile) const;

    bool matchesElement(QXmlStreamReader & in) const;

    Mode mMode;
    ReplacementSchema mSchema;
    bool mIsOk;
    QMultiHash<QString, QHash<QString,QString > > mElements;
};

#endif // REPLACER_H
