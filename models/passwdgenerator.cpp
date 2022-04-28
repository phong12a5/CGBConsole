#include "passwdgenerator.h"

#include <QVector>
#include <QRandomGenerator>

#include <vector>



static QVector<QChar> lowercase  { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
static QVector<QChar> uppercase  { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
                                   'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                                   'W', 'X', 'Y', 'Z'};
static QVector<QChar> digits  { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
static QVector<QChar> specialchars  { '&', '~', '#', '-', '_', '$', '%', '*', '!', '?' };

PasswdGenerator::PasswdGenerator()
{
    setTypes({PasswdGenerator::Lowercase,PasswdGenerator::Uppercase,PasswdGenerator::Digits});
}

QString PasswdGenerator::generate(int length) const
{
    QString passwd(length);

    QVector<QChar> symbols;

    if (m_types.testFlag(Lowercase))
        symbols += lowercase;

    if (m_types.testFlag(Uppercase))
        symbols += uppercase;

    if (m_types.testFlag(Digits))
        symbols += digits;


    if (m_types.testFlag(Symbols))
        symbols += specialchars;


    if (m_types.testFlag(Custom))
        symbols += m_customChars;

    if (symbols.isEmpty())
        return QString();

    QRandomGenerator *rng = QRandomGenerator::global();

    for (int i = 0 ; i < length ; ++i) {
        int index = rng->bounded(symbols.length());
        passwd[i] = symbols[index];
    }

    return passwd;
}

PasswdGenerator::SymbolTypes PasswdGenerator::types() const
{
    return m_types;
}

void PasswdGenerator::setTypes(const SymbolTypes &types)
{
    m_types = types;
}

void PasswdGenerator::setCustomChars(const QVector<QChar> &customChars)
{
    m_customChars = customChars;
}

void PasswdGenerator::setCustomChars(const QString &customChars)
{
    m_customChars.clear();

    for (auto c: customChars)
        m_customChars.append(c);
}
