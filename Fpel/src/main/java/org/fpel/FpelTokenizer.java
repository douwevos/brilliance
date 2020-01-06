package org.fpel;


public class FpelTokenizer {

    private final StringBuilder buf = new StringBuilder();
    private final String fpelText;
    private int start, offset;
    private int ch;
    
    public FpelTokenizer(String fpelText) {
        this.fpelText = fpelText;
        start = 0;
        offset = 0;
        nextChar();
    }
 
    private int nextChar() {
        if (offset<fpelText.length()) {
            ch = fpelText.charAt(offset++);
        } else {
            ch = -1;
        }
        return ch;
    }
    
    
    public FpelToken nextToken() {
        FpelToken result = null;
        while(result==null) {
            start = offset-1;
            switch(ch) {
                case -1 :
                    result = new FpelToken(FpelTokenType.EOT);
                    break;
                case '.' :
                    result = new FpelToken(FpelTokenType.DOT, null, start, offset);
                    nextChar();
                    break;
                case ':' :
                    result = new FpelToken(FpelTokenType.COLON, null, start, offset);
                    nextChar();
                    break;
                case '?' :
                    result = new FpelToken(FpelTokenType.CONDITIONAL, null, start, offset);
                    nextChar();
                    break;
                case ',' :
                    result = new FpelToken(FpelTokenType.COMMA, null, start, offset);
                    nextChar();
                    break;
                case '(' :
                    result = new FpelToken(FpelTokenType.LEFT_PAREN, null, start, offset);
                    nextChar();
                    break;
                case ')' :
                    result = new FpelToken(FpelTokenType.RIGHT_PAREN, null, start, offset);
                    nextChar();
                    break;
                case '[' :
                    result = new FpelToken(FpelTokenType.LEFT_SQ_BRACK, null, start, offset);
                    nextChar();
                    break;
                case ']' :
                    result = new FpelToken(FpelTokenType.RIGHT_SQ_BRACK, null, start, offset);
                    nextChar();
                    break;
                case '=' :
                	nextChar();
                	if (ch == '=') {
                    	nextChar();
                		result = new FpelToken(FpelTokenType.EQ, null, start, offset);
                	}
                    break;

                case '\'' :
                    result = parseStringLiteral();
                    break;

                default :
                    if (Character.isUnicodeIdentifierStart(ch)) {
                        result = parseIdentifier();
                    } else if (Character.isDigit(ch)) {
                        result = parseNumner();
                    } else {
                        nextChar();
                    }
                    break;
            }
        }
        System.out.println(" "+ result);
        return result;
    }

    private FpelToken parseStringLiteral() {
        buf.setLength(0);
        char markChar = (char) ch;
        nextChar();
        boolean isEscape = false;
        while(ch>=0) {
            if (isEscape) {
                switch(ch) {
                    case '\\' : buf.append('\\'); break;
                    case 'n' : buf.append('\n'); break;
                    case 't' : buf.append('\t'); break;
                    case 'r' : buf.append('\r'); break;
                    case 'f' : buf.append('\f'); break;
                    case '0' : buf.append('\0'); break;
                    case '\'' : buf.append('\''); break;
                    case '\"' : buf.append('\"'); break;
                    default :  buf.append('\\').append((char) ch); break;
                }
                isEscape=false;
            } else {
                if (ch==markChar) {
                    nextChar();
                    break;
                } else if (ch=='\\') {
                    isEscape=true;
                } else {
                    buf.append((char) ch);
                }
            }
            nextChar();
        }

        return new FpelToken(FpelTokenType.STRING, buf.toString(), start, offset);
    }

    private FpelToken parseNumner() {
        buf.setLength(0);
        buf.append((char) ch);
        nextChar();
        while(Character.isDigit(ch)) {
            buf.append((char) ch);
            nextChar();
        }
        return new FpelToken(FpelTokenType.INTEGER, Integer.parseInt(buf.toString()), start, offset);
    }

    private FpelToken parseIdentifier() {
        buf.setLength(0);
        buf.append((char) ch);
        nextChar();
        while(Character.isUnicodeIdentifierPart(ch)) {
            buf.append((char) ch);
            nextChar();
        }
        int end = offset-1; 
        String idName = buf.toString();
        if ("null".equals(idName)) {
            return new FpelToken(FpelTokenType.NULL, null, start, end);
        } else if ("eq".equals(idName)) {
            return new FpelToken(FpelTokenType.EQ, null, start, end);
        } else if ("ne".equals(idName)) {
            return new FpelToken(FpelTokenType.NE, null, start, end);
        } else if ("lt".equals(idName)) {
            return new FpelToken(FpelTokenType.LT, null, start, end);
        } else if ("gt".equals(idName)) {
            return new FpelToken(FpelTokenType.GT, null, start, end);
        } else if ("le".equals(idName)) {
            return new FpelToken(FpelTokenType.LE, null, start, end);
        } else if ("ge".equals(idName)) {
            return new FpelToken(FpelTokenType.GE, null, start, end);
        }
        return new FpelToken(FpelTokenType.IDENTIFIER, idName, start, end);
    }
}
