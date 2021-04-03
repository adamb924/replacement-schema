#include <QCoreApplication>

#include <QCommandLineParser>

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

    parser.process(a);

    const QStringList args = parser.positionalArguments();
    if( args.count() < 3 )
    {
        parser.showHelp();
    }
    const QString schema = args.at(0);
    const QString input = args.at(1);
    const QString output = args.at(2);

    Replacer replacer(schema);
    if( replacer.isOk() )
    {
        replacer.performReplacement( input, output );
    }

    return 0;
}
