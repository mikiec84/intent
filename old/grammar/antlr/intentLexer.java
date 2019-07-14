// Generated from intent.g4 by ANTLR 4.1
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class intentLexer extends Lexer {
	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, COMMA=2, PLUS=3, MINUS=4, TIMES=5, DIVIDE=6, MOD=7, POWER=8, AND=9, 
		OR=10, BIT_AND=11, BIT_OR=12, BIT_XOR=13, IS_EQUAL=14, IS_GT=15, IS_GTE=16, 
		IS_LT=17, IS_LTE=18, EQUALS=19, HASH=20, PIPE=21, LINE_BREAK=22, COLON=23, 
		LPAREN=24, RPAREN=25, WORD=26, MARK=27, DOT=28, DOT_MAYBE=29, NOT=30, 
		IF=31, LOOP=32;
	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	public static final String[] tokenNames = {
		"<INVALID>",
		"'?'", "','", "'+'", "'-'", "'*'", "'/'", "'%'", "'**'", "'&&'", "'||'", 
		"'&'", "BIT_OR", "'^'", "'=='", "'>'", "'>='", "'<'", "'<='", "'='", "'#'", 
		"PIPE", "'\n'", "':'", "'('", "')'", "WORD", "MARK", "'.'", "'.?'", "'not'", 
		"'if'", "'loop'"
	};
	public static final String[] ruleNames = {
		"T__0", "COMMA", "PLUS", "MINUS", "TIMES", "DIVIDE", "MOD", "POWER", "AND", 
		"OR", "BIT_AND", "BIT_OR", "BIT_XOR", "IS_EQUAL", "IS_GT", "IS_GTE", "IS_LT", 
		"IS_LTE", "EQUALS", "HASH", "PIPE", "LINE_BREAK", "COLON", "LPAREN", "RPAREN", 
		"WORD", "MARK", "DOT", "DOT_MAYBE", "NOT", "IF", "LOOP"
	};


	public intentLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "intent.g4"; }

	@Override
	public String[] getTokenNames() { return tokenNames; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\3\uacf5\uee8c\u4f5d\u8b0d\u4a45\u78bd\u1b2f\u3378\2\"\u0097\b\1\4\2\t"+
		"\2\4\3\t\3\4\4\t\4\4\5\t\5\4\6\t\6\4\7\t\7\4\b\t\b\4\t\t\t\4\n\t\n\4\13"+
		"\t\13\4\f\t\f\4\r\t\r\4\16\t\16\4\17\t\17\4\20\t\20\4\21\t\21\4\22\t\22"+
		"\4\23\t\23\4\24\t\24\4\25\t\25\4\26\t\26\4\27\t\27\4\30\t\30\4\31\t\31"+
		"\4\32\t\32\4\33\t\33\4\34\t\34\4\35\t\35\4\36\t\36\4\37\t\37\4 \t \4!"+
		"\t!\3\2\3\2\3\3\3\3\3\4\3\4\3\5\3\5\3\6\3\6\3\7\3\7\3\b\3\b\3\t\3\t\3"+
		"\t\3\n\3\n\3\n\3\13\3\13\3\13\3\f\3\f\3\r\3\r\3\16\3\16\3\17\3\17\3\17"+
		"\3\20\3\20\3\21\3\21\3\21\3\22\3\22\3\23\3\23\3\23\3\24\3\24\3\25\3\25"+
		"\3\26\3\26\3\27\3\27\3\30\3\30\3\31\3\31\3\32\3\32\3\33\6\33}\n\33\r\33"+
		"\16\33~\3\34\3\34\6\34\u0083\n\34\r\34\16\34\u0084\3\35\3\35\3\36\3\36"+
		"\3\36\3\37\3\37\3\37\3\37\3 \3 \3 \3!\3!\3!\3!\3!\2\"\3\3\1\5\4\1\7\5"+
		"\1\t\6\1\13\7\1\r\b\1\17\t\1\21\n\1\23\13\1\25\f\1\27\r\1\31\16\1\33\17"+
		"\1\35\20\1\37\21\1!\22\1#\23\1%\24\1\'\25\1)\26\1+\27\1-\30\1/\31\1\61"+
		"\32\1\63\33\1\65\34\1\67\35\19\36\1;\37\1= \1?!\1A\"\1\3\2\5\6\2))//a"+
		"ac|\4\2--//\4\2aac|\u0098\2\3\3\2\2\2\2\5\3\2\2\2\2\7\3\2\2\2\2\t\3\2"+
		"\2\2\2\13\3\2\2\2\2\r\3\2\2\2\2\17\3\2\2\2\2\21\3\2\2\2\2\23\3\2\2\2\2"+
		"\25\3\2\2\2\2\27\3\2\2\2\2\31\3\2\2\2\2\33\3\2\2\2\2\35\3\2\2\2\2\37\3"+
		"\2\2\2\2!\3\2\2\2\2#\3\2\2\2\2%\3\2\2\2\2\'\3\2\2\2\2)\3\2\2\2\2+\3\2"+
		"\2\2\2-\3\2\2\2\2/\3\2\2\2\2\61\3\2\2\2\2\63\3\2\2\2\2\65\3\2\2\2\2\67"+
		"\3\2\2\2\29\3\2\2\2\2;\3\2\2\2\2=\3\2\2\2\2?\3\2\2\2\2A\3\2\2\2\3C\3\2"+
		"\2\2\5E\3\2\2\2\7G\3\2\2\2\tI\3\2\2\2\13K\3\2\2\2\rM\3\2\2\2\17O\3\2\2"+
		"\2\21Q\3\2\2\2\23T\3\2\2\2\25W\3\2\2\2\27Z\3\2\2\2\31\\\3\2\2\2\33^\3"+
		"\2\2\2\35`\3\2\2\2\37c\3\2\2\2!e\3\2\2\2#h\3\2\2\2%j\3\2\2\2\'m\3\2\2"+
		"\2)o\3\2\2\2+q\3\2\2\2-s\3\2\2\2/u\3\2\2\2\61w\3\2\2\2\63y\3\2\2\2\65"+
		"|\3\2\2\2\67\u0080\3\2\2\29\u0086\3\2\2\2;\u0088\3\2\2\2=\u008b\3\2\2"+
		"\2?\u008f\3\2\2\2A\u0092\3\2\2\2CD\7A\2\2D\4\3\2\2\2EF\7.\2\2F\6\3\2\2"+
		"\2GH\7-\2\2H\b\3\2\2\2IJ\7/\2\2J\n\3\2\2\2KL\7,\2\2L\f\3\2\2\2MN\7\61"+
		"\2\2N\16\3\2\2\2OP\7\'\2\2P\20\3\2\2\2QR\7,\2\2RS\7,\2\2S\22\3\2\2\2T"+
		"U\7(\2\2UV\7(\2\2V\24\3\2\2\2WX\7~\2\2XY\7~\2\2Y\26\3\2\2\2Z[\7(\2\2["+
		"\30\3\2\2\2\\]\7~\2\2]\32\3\2\2\2^_\7`\2\2_\34\3\2\2\2`a\7?\2\2ab\7?\2"+
		"\2b\36\3\2\2\2cd\7@\2\2d \3\2\2\2ef\7@\2\2fg\7?\2\2g\"\3\2\2\2hi\7>\2"+
		"\2i$\3\2\2\2jk\7>\2\2kl\7?\2\2l&\3\2\2\2mn\7?\2\2n(\3\2\2\2op\7%\2\2p"+
		"*\3\2\2\2qr\7~\2\2r,\3\2\2\2st\7\f\2\2t.\3\2\2\2uv\7<\2\2v\60\3\2\2\2"+
		"wx\7*\2\2x\62\3\2\2\2yz\7+\2\2z\64\3\2\2\2{}\t\2\2\2|{\3\2\2\2}~\3\2\2"+
		"\2~|\3\2\2\2~\177\3\2\2\2\177\66\3\2\2\2\u0080\u0082\t\3\2\2\u0081\u0083"+
		"\t\4\2\2\u0082\u0081\3\2\2\2\u0083\u0084\3\2\2\2\u0084\u0082\3\2\2\2\u0084"+
		"\u0085\3\2\2\2\u00858\3\2\2\2\u0086\u0087\7\60\2\2\u0087:\3\2\2\2\u0088"+
		"\u0089\7\60\2\2\u0089\u008a\7A\2\2\u008a<\3\2\2\2\u008b\u008c\7p\2\2\u008c"+
		"\u008d\7q\2\2\u008d\u008e\7v\2\2\u008e>\3\2\2\2\u008f\u0090\7k\2\2\u0090"+
		"\u0091\7h\2\2\u0091@\3\2\2\2\u0092\u0093\7n\2\2\u0093\u0094\7q\2\2\u0094"+
		"\u0095\7q\2\2\u0095\u0096\7r\2\2\u0096B\3\2\2\2\5\2~\u0084";
	public static final ATN _ATN =
		ATNSimulator.deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}