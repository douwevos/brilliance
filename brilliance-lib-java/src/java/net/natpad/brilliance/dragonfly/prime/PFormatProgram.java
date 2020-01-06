package net.natpad.brilliance.dragonfly.prime;

import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.dragonfly.FormatAction;
import net.natpad.brilliance.dragonfly.FormatProgram;

public class PFormatProgram {

	static {
        System.loadLibrary("brilliancejni");
    }

	public static native long create();

	public static native void construct(FormatProgram formatProgram);

	public static native void add(FormatProgram formatProgram, FormatAction action);

	public static native void apply(FormatProgram formatProgram, Document document);

	public static native long createAction(int in_left_offset, int in_left_row, int in_right_offset, int in_right_row,
			int out_left_offset, int out_left_row, int out_right_offset, int out_right_row);

}
