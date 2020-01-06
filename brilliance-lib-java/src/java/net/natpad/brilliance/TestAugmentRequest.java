//package net.natpad.brilliance;
//
//import java.util.ArrayList;
//import java.util.List;
//
//import org.eclipse.jdt.core.compiler.InvalidInputException;
//import org.eclipse.jdt.internal.compiler.parser.Scanner;
//import org.eclipse.jdt.internal.compiler.parser.TerminalTokens;
//
//import net.natpad.brilliance.caterpillar.StringWo;
//import net.natpad.brilliance.chameleon.Document;
//import net.natpad.brilliance.chameleon.Revision;
//import net.natpad.brilliance.chameleon.RevisionReader;
//import net.natpad.brilliance.dragonfly.AugmentRequest;
//import net.natpad.brilliance.dragonfly.KeywordPrinter;
//
//public class TestAugmentRequest extends AugmentRequest {
//
//	public TestAugmentRequest(Document document, Revision revision, StringWo markupSlotKey) {
//		super(document, revision, markupSlotKey);
//	}
//	
//	
//	
//	@Override
//	public boolean runAugment(Revision revision, KeywordPrinter keywordPrinter, KeywordPrinter lineTagPrinter) {
//		
//		RevisionReader revisionReader = new RevisionReader(revision);
//		
//		List<Integer> lineStarts = new ArrayList<>();
//		lineStarts.add(0);
//		
//		StringBuilder buf = new StringBuilder();
//		int offset = 0;
//		while(true) {
//			int utf8Char = revisionReader.scanUtf8Char();
//			if (utf8Char==-1) {
//				break;
//			}
//			buf.appendCodePoint(utf8Char);
//			
//			log(""+utf8Char);
//			if (utf8Char==10) {
//				lineStarts.add(buf.length());
//			}
//			offset++;
//		}
//
//		lineStarts.add(buf.length()+1);
//
//		
//		Scanner scanner = new Scanner();
//		scanner.setSource(buf.toString().toCharArray());
//		Position start = new Position();
//		Position end = new Position();
//		for(int idx=0; idx<100000; idx++) {
//			try {
//				int nextToken = scanner.getNextToken();
//				if (nextToken == TerminalTokens.TokenNameEOF) {
//					break;
//				}
//				log("nexttoken="+nextToken+", "+scanner.startPosition+"..."+scanner.currentPosition);
//				row(lineStarts, scanner.startPosition, start);
//				row(lineStarts, scanner.currentPosition, end);
//				log("nexttoken="+nextToken+", "+start+", "+end);
////				scanner.currentPosition
//				if (!(start.row==-1 || end.row==-1)) {
//					keywordPrinter.printFgColor(start.row, start.column, end.row, end.column, mapTokenToColor(nextToken));
//				}
//			} catch (InvalidInputException e) {
//			}
//		}
//		
//		log("shoudl run augment request");
//		
////		keywordPrinter.printFgColor(0, 5, 0, 10, 4);
////		keywordPrinter.printFgColor(0, 13, 0, 15, 7);
////		keywordPrinter.printFgColor(0, 18, 0, 23, 9);
//		return true;
//	}
//
//	public int mapTokenToColor(int symbolId) {
//		switch(symbolId) {
//		case TerminalTokens.TokenNameCOLON : return 3;
//		case TerminalTokens.TokenNameDOT : return 3;
//		case TerminalTokens.TokenNameCOMMA : return 3;
//		case TerminalTokens.TokenNameSEMICOLON: return 3;
//
//		case TerminalTokens.TokenNameEQUAL: return 1;
//
//		/* conditions */
//		case TerminalTokens.TokenNameEQUAL_EQUAL : return 1;
//		case TerminalTokens.TokenNameNOT_EQUAL: return 1;
//		case TerminalTokens.TokenNameLESS : return 1;
//		case TerminalTokens.TokenNameGREATER : return 1;
//		case TerminalTokens.TokenNameLESS_EQUAL : return 1;
//		case TerminalTokens.TokenNameGREATER_EQUAL : return 1;
//
//		/* literals */
//		case TerminalTokens.TokenNameStringLiteral : return 5;
////		case TerminalTokens.TokenNameOCTAL : return 4;
////		case TerminalTokens.TokenNameDECIMAL : return 4;
////		case TerminalTokens.TokenNameSTRING : return 5;
////		case TerminalTokens.TokenNameHEX : return 4;
//		case TerminalTokens.TokenNameIdentifier : return 1;
//
//		/* unary operators */
//		case TerminalTokens.TokenNameQUESTION : return 7;
//		case TerminalTokens.TokenNamePLUS_PLUS : return 7;
//		case TerminalTokens.TokenNameMINUS_MINUS : return 7;
//		case TerminalTokens.TokenNameNOT : return 7;
//		case TerminalTokens.TokenNameTWIDDLE : return 7;
//
//		case TerminalTokens.TokenNameMULTIPLY : return 7;
//		case TerminalTokens.TokenNameDIVIDE : return 7;
//		case TerminalTokens.TokenNameREMAINDER : return 7;
//		case TerminalTokens.TokenNamePLUS : return 7;
//		case TerminalTokens.TokenNameMINUS : return 7;
//
//		case TerminalTokens.TokenNameOR : return 6;
//		case TerminalTokens.TokenNameAND : return 6;
//		case TerminalTokens.TokenNameOR_OR : return 6;
//		case TerminalTokens.TokenNameAND_AND : return 6;
//		case TerminalTokens.TokenNameXOR : return 6;
//
//
//
//		case TerminalTokens.TokenNameLBRACE : return 9;
//		case TerminalTokens.TokenNameRBRACE : return 9;
//		case TerminalTokens.TokenNameLPAREN : return 9;
//		case TerminalTokens.TokenNameRPAREN : return 9;
//		case TerminalTokens.TokenNameLBRACKET : return 4;
//		case TerminalTokens.TokenNameRBRACKET : return 4;
//
//
//		case TerminalTokens.TokenNameMULTIPLY_EQUAL : return 2;
//		case TerminalTokens.TokenNameDIVIDE_EQUAL : return 2;
//		case TerminalTokens.TokenNameREMAINDER_EQUAL : return 2;
//		case TerminalTokens.TokenNamePLUS_EQUAL : return 2;
//		case TerminalTokens.TokenNameMINUS_EQUAL : return 1;
//		case TerminalTokens.TokenNameLEFT_SHIFT_EQUAL : return 2;
//		case TerminalTokens.TokenNameRIGHT_SHIFT_EQUAL : return 2;
//		case TerminalTokens.TokenNameAND_EQUAL : return 2;
//		case TerminalTokens.TokenNameXOR_EQUAL : return 2;
//		case TerminalTokens.TokenNameOR_EQUAL : return 2;
//
//
//
//		case TerminalTokens.TokenNameUNSIGNED_RIGHT_SHIFT_EQUAL : return 1;
//		case TerminalTokens.TokenNameCOMMENT_LINE : return 15;
//		case TerminalTokens.TokenNameCOMMENT_BLOCK : return 3;
//
//		case TerminalTokens.TokenNamepackage : return 8;
//		case TerminalTokens.TokenNameimport : return 8;
//		case TerminalTokens.TokenNameimplements : return 8;
//		case TerminalTokens.TokenNameinstanceof : return 8;
//		case TerminalTokens.TokenNameextends : return 8;
//
//		case TerminalTokens.TokenNamepublic : return 4;
//		case TerminalTokens.TokenNameprivate : return 4;
//		case TerminalTokens.TokenNameprotected : return 4;
//		case TerminalTokens.TokenNamestatic : return 4;
//		case TerminalTokens.TokenNameabstract : return 5;
//		case TerminalTokens.TokenNametransient : return 5;
//		case TerminalTokens.TokenNamefinal : return 5;
//		case TerminalTokens.TokenNamevolatile : return 5;
//		case TerminalTokens.TokenNamenative : return 5;
//
//		case TerminalTokens.TokenNamedo : return 9;
//		case TerminalTokens.TokenNametry : return 9;
//		case TerminalTokens.TokenNamethrow : return 9;
//		case TerminalTokens.TokenNamethrows : return 9;
//		case TerminalTokens.TokenNamefinally : return 9;
//		case TerminalTokens.TokenNamebreak : return 9;
//		case TerminalTokens.TokenNameelse : return 9;
////		case TerminalTokens.TokenNameINNER : return 9;
//		case TerminalTokens.TokenNamecase : return 9;
////		case TerminalTokens.TokenNameGOTO : return 9;
//		case TerminalTokens.TokenNamereturn : return 9;
//		case TerminalTokens.TokenNamecatch : return 9;
//		case TerminalTokens.TokenNamenew : return 9;
//		case TerminalTokens.TokenNamefor : return 9;
//		case TerminalTokens.TokenNameswitch : return 9;
//		case TerminalTokens.TokenNamecontinue : return 9;
//		case TerminalTokens.TokenNamewhile : return 9;
//		case TerminalTokens.TokenNamesynchronized : return 9;
//		case TerminalTokens.TokenNamedefault : return 9;
//		case TerminalTokens.TokenNameif : return 9;
//
//		case TerminalTokens.TokenNameboolean : return 12;
//		case TerminalTokens.TokenNamedouble : return 12;
//		case TerminalTokens.TokenNamebyte : return 12;
//		case TerminalTokens.TokenNameint : return 12;
//		case TerminalTokens.TokenNamevoid : return 12;
//		case TerminalTokens.TokenNamefloat : return 12;
//		case TerminalTokens.TokenNamelong : return 12;
//		case TerminalTokens.TokenNameshort : return 12;
//		case TerminalTokens.TokenNamechar : return 1;
//
////		case TerminalTokens.TokenNameREST : return 15;
////		case TerminalTokens.TokenNameVAR : return 15;
////		case TerminalTokens.TokenNameCAST : return 15;
////		case TerminalTokens.TokenNameFUTURE : return 15;
////		case TerminalTokens.TokenNameCONST : return 15;
////		case TerminalTokens.TokenNameGENERIC : return 15;
//
//		case TerminalTokens.TokenNameinterface : return 10;
//		case TerminalTokens.TokenNameclass : return 10;
//
//		case TerminalTokens.TokenNamesuper : return 8;
////		case TerminalTokens.TokenNameNULL : return 8;
//		case TerminalTokens.TokenNamethis : return 8;
//		}
//		
//		return 1;
//	}
//	
//	public void row(List<Integer> starts, int offset, Position position) {
//		for(int idx=0; idx<starts.size(); idx++) {
//			Integer u = starts.get(idx);
//			if (offset<u) {
//				position.row = idx-1;
//				position.column = offset-starts.get(idx-1);
//				return;
//			}
//		}
//		position.row = -1;
//	}
//
//	static class Position {
//		public int row;
//		public int column;
//		
//		@Override
//		public String toString() {
//			return "pos[col="+column+", row="+row+"]";
//		}
//	}
//	
//}
