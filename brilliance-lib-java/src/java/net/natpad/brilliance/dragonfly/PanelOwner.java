package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.glibc.GObject;
import net.natpad.brilliance.leafhopper.Frame;
import net.natpad.brilliance.leafhopper.IPanelOwner;
import net.natpad.brilliance.leafhopper.Panel;
import net.natpad.brilliance.worm.WorkerService;

public class PanelOwner extends GObject implements IPanelOwner {

	public PanelOwner(Frame frame, WorkerService workerService) {
		super(Dragonfly.createPanelOwner(frame, workerService));
	}
	
	@Override
	public void addPanel(Panel panel) {
	}
	
}
