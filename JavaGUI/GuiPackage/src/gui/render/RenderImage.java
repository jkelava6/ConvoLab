package gui.render;

import gui.GuiFrame;

public abstract class RenderImage {
	
	protected GuiFrame area;
	
	public RenderImage(byte[] imageString, GuiFrame area) {
		this.area = area;
	}
	
	public GuiFrame GetArea() {
		return area;
	}
	
	public void SetArea(GuiFrame area) {
		this.area = area;
	}

}
