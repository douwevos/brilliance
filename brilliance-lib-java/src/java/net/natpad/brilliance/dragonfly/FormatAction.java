package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.dragonfly.prime.PFormatProgram;
import net.natpad.brilliance.glibc.GObject;

public class FormatAction extends GObject {

	public FormatAction(int in_left_offset, int in_left_row, int in_right_offset, int in_right_row, int out_left_offset, int out_left_row, int out_right_offset,
			int out_right_row) {
		super(PFormatProgram.createAction(in_left_offset, in_left_row, in_right_offset, in_right_row, out_left_offset, out_left_row, out_right_offset,
				out_right_row));
	}

}
