package dk.hisimon.mmlgb;

public class ParserException extends Exception {
	public ParserException(String message) {
		super(message);
	}

	public ParserException(String message, Lexer.Token token) {
		super(String.format("On line %d column %d: %s", token.line, token.pos, message));
	}
}
