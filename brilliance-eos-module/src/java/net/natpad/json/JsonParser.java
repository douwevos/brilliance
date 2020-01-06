package net.natpad.json;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import net.natpad.json.JsonArray.JsonArrayBuilder;
import net.natpad.json.JsonError.Level;
import net.natpad.json.JsonError.Type;
import net.natpad.json.JsonField.JsonFieldBuilder;
import net.natpad.json.JsonObject.JsonObjectBuilder;

public class JsonParser {

	private final JsonScanner scanner;
	
	private int currentIndex;
	private JsonToken current;
	private JsonToken[] lookahead = new JsonToken[4];
	private List<JsonToken> tokenList = new ArrayList<JsonToken>();
	
	public JsonParser(JsonScanner scanner) throws IOException {
		this.scanner = scanner;
		currentIndex = 0;
		current = scanToken();
		for(int idx=0; idx<lookahead.length; idx++) {
			lookahead[idx] = scanToken();
		}
	}

	
	public JsonToken scanToken() throws IOException {
		JsonToken result = scanner.next();
		if (result.getType()!=JsonTokenType.EOT) {
			tokenList.add(result);
		}
		return result;
	}
	
	private void next() throws IOException {
		current = lookahead[0];
		currentIndex++;
		System.arraycopy(lookahead, 1, lookahead, 0, lookahead.length-1);
		lookahead[lookahead.length-1] = scanToken();
//		log("next="+current);
	}

	public List<JsonToken> getTokenList() {
		return tokenList;
	}
	
	public JsonValue parse() throws IOException {
		return parseValue();
	}

	
	public JsonValue parseValue() throws IOException {
		switch(current.getType()) {
			case EOT :
				return null;
			case LEFT_BRACE : 
				return parseObject();
			case LEFT_BRACKET :
				return parseArray();
			case TRUE : 
			case FALSE : 
			case NULL : 
			case NUMBER : 
			case QUOTED_STRING : {
				JsonSimpleValue result = new JsonSimpleValue(current);
				next();
				return result;
			}
			
			default : 
				JsonError error = new JsonError(Type.SCANNER, Level.ERROR, "Unexpected symbol "+current.getType());
				current.addError(error);
		}
		return null;
	}


	private JsonArray parseArray() throws IOException {
		
		JsonArrayBuilder builder = JsonArray.builder();
		builder.append(current);
		
		if (!expect(JsonTokenType.LEFT_BRACKET, "Expected a left Bracket")) {
			// TODO error
		}
		if (current.getType() == JsonTokenType.RIGHT_BRACKET) {
			builder.append(current);
			next();
			return builder.build();
		}

		List<JsonValue> values = new ArrayList<JsonValue>();
		JsonValue firstValue = parseValue();
		
		values.add(firstValue);
		builder.append(firstValue);
		while(true) {
			if (current.getType() == JsonTokenType.RIGHT_BRACKET) {
				builder.append(current);
				next();
				break;
			}
			if (current.getType() != JsonTokenType.COMMA) {
				// TODO error
				break;
			}
			builder.append(current);
			next();
			JsonValue nextValue = parseValue();
			builder.append(nextValue);
		}
		
		return builder.build();
	}


	private JsonObject parseObject() throws IOException {
		JsonObjectBuilder builder = JsonObject.builder();
		builder.append(current);
		if (!expect(JsonTokenType.LEFT_BRACE, "Expected a left brace")) {
			// TODO error
		}
		
		if (current.getType() == JsonTokenType.RIGHT_BRACE) {
			builder.append(current);
			next();
			return builder.build();
		}
		JsonField firstField = parseField();
		builder.append(firstField);
		while(true) {
			if (current.getType() == JsonTokenType.RIGHT_BRACE) {
				builder.append(current);
				next();
				break;
			}
			if (current.getType() != JsonTokenType.COMMA) {
				builder.append(current);
				JsonError error = new JsonError(Type.SCANNER, Level.ERROR, "expected a comma");
				current.addError(error);
				break;
			}
			next();
			JsonField nextField = parseField();
			builder.append(nextField);
		}
		return builder.build();

	}

	
	private JsonField parseField() throws IOException {
		JsonFieldBuilder builder = JsonField.builder();
		JsonToken key = current;
		builder.setKey(key);
		if (!expect(JsonTokenType.QUOTED_STRING, "Expected a quoted string")) {
			// TODO error
		}

		if (!expect(JsonTokenType.COLON, "Expected a colon")) {
			// TODO error
		}
		JsonValue value = parseValue();
		builder.setValue(value);
		return builder.build();

		
	}

	private boolean expect(JsonTokenType type, String errorMessage) throws IOException {
		JsonTokenType typeCurrent = current.getType();
		boolean isSame = typeCurrent == type;
		if (!isSame) {
			JsonError error = new JsonError(Type.SCANNER, Level.ERROR, "expected "+type+" got "+typeCurrent);
			current.addError(error );
		}
		next();
		return isSame;
	}

	
}
