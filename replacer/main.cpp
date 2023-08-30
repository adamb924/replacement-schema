#include <QCoreApplication>

#include <QCommandLineParser>
#include <QtDebug>

#include "replacer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("Replacer");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Program to perform systematica replacements on files.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("schema", QCoreApplication::translate("main", "Schema file."));
    parser.addPositionalArgument("input", QCoreApplication::translate("main", "Input file."));
    parser.addPositionalArgument("output", QCoreApplication::translate("main", "Output file."));
    QCommandLineOption xmlOption(QStringList() << "x" << "xml" << "Process file as XML instead of as plain text.");
    parser.addOption( xmlOption );

    parser.process(a);

    const QStringList args = parser.positionalArguments();
    if( args.count() < 3 )
    {
        parser.showHelp();
    }
    const QString schema = args.at(0);
    const QString input = args.at(1);
    const QString output = args.at(2);
    const bool xml = parser.isSet(xmlOption);

    /// TODO for text files, it's stupid to make a schema file
    /// also, <schema> should be called <settings> or something like tthat

    Replacer replacer(schema);
    replacer.setMode( xml ? Replacer::Xml : Replacer::Text );

    qInfo() << replacer.schema()->summary();
    if( replacer.isOk() )
    {
        replacer.performReplacement( input, output );
        qInfo() << "Transformed" << input << "to" << output << "using" << schema;
    }
    else
    {
        qCritical() << "Replacer is not okay.";
    }

    return 0;
}
