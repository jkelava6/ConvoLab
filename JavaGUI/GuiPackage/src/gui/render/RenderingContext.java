package gui.render;

import gui.GuiFrame;

public class RenderingContext {
	
	public RenderEngine renderer;
	public GuiFrame frame;
	
	private RenderingContext(RenderEngine renderer) {
		this.renderer = renderer;
	}
	
	public RenderingContext(RenderEngine renderer, float width, float height) {
		this.renderer = renderer;
		frame = new GuiFrame(width, height);
	}
	
	public void Resize(GuiFrame frame) {
		this.frame = frame;
	}
	
	public RenderingContext SubFrame(GuiFrame frame) {
		RenderingContext child = new RenderingContext(renderer);
		child.frame = this.frame.SubFrame(frame);
		
		return child;
	}
	
	public int IntLeft() {
		return (int)frame.loX;
	}
	
	public int IntTop() {
		return (int)frame.loY;
	}
	
	public int IntBottom() {
		return (int)frame.hiY;
	}
	
	public int IntRight() {
		return (int)frame.hiX;
	}
	
	public int IntWidth() {
		return IntRight() - IntLeft();
	}
	
	public int IntHeight() {
		return IntBottom() - IntTop();
	}

}
