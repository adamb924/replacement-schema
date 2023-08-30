#ifndef REPLACER_H
#define REPLACER_H

class QString;
class QFile;

#include "replacementschema.h"

class Replacer
{
public:
    enum Mode { Text, Xml };

    explicit Replacer(const QString & schemaFile);

    void performReplacement(const QString & inputPath, const QString & outputPath) const;

    bool isOk() const;

    const ReplacementSchema *schema() const;

    void setMode(Mode newMode);

private:
    void performReplacementText(QFile &inFile, QFile &outFile) const;
    void performReplacementXml(QFile &inFile, QFile &outFile) const;

    Mode mMode;
    ReplacementSchema mSchema;
    bool mIsOk;
};

#endif // REPLACER_H
