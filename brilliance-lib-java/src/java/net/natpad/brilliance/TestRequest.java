package net.natpad.brilliance;

import net.natpad.brilliance.worm.Request;

public class TestRequest extends Request implements Logger {

	
	@Override
	public void runRequest() {
		log("remotly requested");
	}
	
}
