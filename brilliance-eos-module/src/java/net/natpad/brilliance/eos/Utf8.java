package net.natpad.brilliance.eos;

public class Utf8 {

	public static int getUtf8ByteCount(int codePoint) {
	    return codePoint <= 0x7F? 1
	         : codePoint <= 0x7FF? 2
	         : codePoint <= 0xFFFF? 3
	         : codePoint <= 0x1FFFFF? 4
	         : codePoint <= 0x3FFFFFF? 5
	         : 6;
	}
	
	public static int utf8offset(String text, int offset) {
		int out = 0;
		int result = 0;
		for(int i=0; i<text.length(); i++) {
			char charAt = text.charAt(i);
			out += getUtf8ByteCount(charAt);
			if (offset<out) {
				break;
			}
			result++;
		}
		return result;
	}
	
	
	public static String grepIndent(String text) {
		StringBuilder s = new StringBuilder();
		for(int idx=0; idx<text.length(); idx++) {
			char ch = text.charAt(idx);
			if ((ch==' ') || (ch=='\t')) {
				s.append(ch);
			} else {
				break;
			}
		}
		return s.toString();
	}
}
