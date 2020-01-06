package net.natpad.json;

import java.io.IOException;
import java.io.Reader;

import net.natpad.brilliance.eos.Utf8;

public class JsonScanner {

	private final Reader reader;
	
	private int nextCodepoint;
	private Location left;
	private Location right;
	
	public JsonScanner(Reader reader) throws IOException {
		this.reader = reader;
		
		left = new Location();
		nextCodepoint = reader.read();
		right = left.next(nextCodepoint);
	}
	
	private void advance() throws IOException {
		left = right;
		nextCodepoint = reader.read();
		right = left.next(nextCodepoint);
	}


	public JsonToken next() throws IOException {
		JsonToken result = nextImpl();
//		log("token="+result);
		return result;
	}

	public JsonToken nextImpl() throws IOException {
		JsonToken result = null;
		while(result ==null) {
		switch(nextCodepoint) {
			case -1 : {
				result = new JsonToken(JsonTokenType.EOT);
			} break;
			case '"' : {
				result = scanQuotedString();
			} break;
			case ':' : 
				result = createPlane(JsonTokenType.COLON);
				break;
			case ',' :
				result = createPlane(JsonTokenType.COMMA);
				break;
			case '{' : 
				result = createPlane(JsonTokenType.LEFT_BRACE);
				break;
			case '}' : 
				result = createPlane(JsonTokenType.RIGHT_BRACE);
				break;
			case '[' : 
				result = createPlane(JsonTokenType.LEFT_BRACKET);
				break;
			case ']' : 
				result = createPlane(JsonTokenType.RIGHT_BRACKET);
				break;
			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' :
			case '-' :
				result = scanNumber();
				break;
			default :
				if (Character.isAlphabetic(nextCodepoint)) {
					result = scanLiteral();
				} else {
					advance();
				}
				break;
		
		}
		}
		return result;
	}

	private JsonToken scanNumber() throws IOException {
		Location tokenLeft = left;
		StringBuilder buf = new StringBuilder();
		scanInteger(buf);
		scanFraction(buf);
		scanExponent(buf);
		return new JsonToken(JsonTokenType.NUMBER, tokenLeft, left);
	}

	private void scanExponent(StringBuilder buf) throws IOException {
		if (nextCodepoint == 'e' || nextCodepoint == 'E') {
			advance();
			if (nextCodepoint == '-' || nextCodepoint == '+') {
				advance();
			}
			scanDigits(buf);
		}
		
	}

	private void scanFraction(StringBuilder buf) throws IOException {
		if (nextCodepoint == '.') {
			advance();
			scanDigits(buf);
		}
	}

	private void scanInteger(StringBuilder buf) throws IOException {
		if (nextCodepoint == '-') {
			advance();
		}
		
		if (nextCodepoint == '0') {
			advance();
		} else {
			scanDigits(buf);
		}
	}

	
	
	private void scanDigits(StringBuilder buf) throws IOException {
		while(true) {
			if (Character.isDigit(nextCodepoint)) {
				advance();
			} else {
				break;
			}
		}
	}

	private JsonToken scanLiteral() throws IOException {
		Location tokenLeft = left;
		StringBuilder buf = new StringBuilder();
		while(true) {
			if (Character.isAlphabetic(nextCodepoint) || nextCodepoint=='_' || Character.isDigit(nextCodepoint)) {
				buf.appendCodePoint(nextCodepoint);
				advance();
			} else {
				break;
			}
		}
		String literal = buf.toString();
		if ("true".equalsIgnoreCase(literal)) {
			return new JsonToken(JsonTokenType.TRUE, tokenLeft, left);
		} else if ("false".equalsIgnoreCase(literal)) {
			return new JsonToken(JsonTokenType.FALSE, tokenLeft, left);
		} else if ("null".equalsIgnoreCase(literal)) {
			return new JsonToken(JsonTokenType.NULL, tokenLeft, left);
		}
		return null;
	}

	private JsonToken createPlane(JsonTokenType type) throws IOException {
		JsonToken result = new JsonToken(type, left, right);
		advance();
		return result;
	}

	private JsonToken scanQuotedString() throws IOException {
		Location tokenLeft = left;
		int marker = nextCodepoint;
		boolean escapeActive = false;
		advance();
		StringBuilder buf = new StringBuilder();
		while(true) {
			if (nextCodepoint == marker && !escapeActive) {
				advance();
				break;
			} else if ((nextCodepoint == -1) || (right.getRow()!=tokenLeft.getRow())) {
				return new JsonToken(JsonTokenType.QUOTED_STRING, tokenLeft, left);
			} else if (nextCodepoint == '\\') {
				escapeActive = !escapeActive;
			} else {
				escapeActive = false;
			}
			
			buf.appendCodePoint(nextCodepoint);
			advance();
		}
		
		return new JsonToken(JsonTokenType.QUOTED_STRING, tokenLeft, left, buf.toString());
	}
	
	


	public static class Location {
		private final int column;
		private final int row;
		private final int utf8offset;
		
		public Location() {
			this.column = 0;
			this.row = 0;
			this.utf8offset = 0;
		}
		
		public Location(int column, int row, int utf8Offset) {
			this.column = column;
			this.row = row;
			this.utf8offset = utf8Offset;
		}
		
		public Location next(int codepoint) {
			if (codepoint==0 || codepoint==0xa || codepoint==0xd) {
				return nextLine();
			}
			return new Location(column+1, row, utf8offset+Utf8.getUtf8ByteCount(codepoint));
			
		}
		
		public Location nextLine() {
			return new Location(0, row+1, 0);
		}
		
		public int getColumn() {
			return column;
		}
		
		public int getRow() {
			return row;
		}
		
		public int getUtf8offset() {
			return utf8offset;
		}

		@Override
		public String toString() {
			return "Location [column=" + column + ", row=" + row + ", utf8offset=" + utf8offset + "]";
		}
	}
	
}
