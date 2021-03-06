#include "parser.h"

int parseStageFromFile( QString str, Stage& s ) {

    QStringList tokens = tokenizeByChar(str, QString(" "));

    if( tokens.size() > 3 )		// catches commands that are too long
        return PARSE_ERROR;

    if( parseType(tokens.at(0), s) != PARSE_SUCCESS )	return PARSE_ERROR;
    if( parseParam(tokens.at(1), s) != PARSE_SUCCESS ) return PARSE_ERROR;
    if( s.getType() == STAGE_FILL )
        if( parseParam(tokens.at(2), s) != PARSE_SUCCESS ) return PARSE_ERROR;

    return PARSE_SUCCESS;
}

int parseType( QString typeStr, Stage& s ) {

    if		( typeStr.compare("fill", Qt::CaseInsensitive) == 0 )  s.setType(STAGE_FILL);
    else if ( typeStr.compare("wash", Qt::CaseInsensitive) == 0 )  s.setType(STAGE_WASH);
    else if ( typeStr.compare("dry", Qt::CaseInsensitive) == 0 )   s.setType(STAGE_DRY);
    else return PARSE_ERROR;

    return PARSE_SUCCESS;
}

int parseParam( QString paramStr, Stage& s ) {

    QRegExp rxNumeric("\\d*");
    QStringList paramTokens = tokenizeByChar(paramStr, QString(":"));

    if( paramTokens.size() > 2 )		// no more than 2 key:value pairs
        return PARSE_ERROR;
    else if ( !rxNumeric.exactMatch(paramTokens.at(1)) )	// literal after : must be an int
        return PARSE_ERROR;

    if ( paramTokens.at(0).compare("p", Qt::CaseInsensitive) == 0 && s.getType() == STAGE_FILL )
        s.setPump( paramTokens.at(1).toInt() );
    else if ( paramTokens.at(0).compare("t", Qt::CaseInsensitive) == 0 )
        s.setDur( paramTokens.at(1).toInt() );
    else
        return PARSE_ERROR;

    return PARSE_SUCCESS;
}

QStringList tokenizeByChar( QString str, QString delimiter) {
    QRegExp rx("(\\" + delimiter + ")");
    QStringList tokens = str.split(rx);

    for(int i=0; i<tokens.size(); i++) {
        if( tokens.at(i).isEmpty() ) {
        tokens.removeAt(i);
        }
    }

    return tokens;
}
