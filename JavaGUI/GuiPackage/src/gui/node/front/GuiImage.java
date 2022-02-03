package gui.node.front;

import gui.GuiFrame;
import gui.node.base.GuiElement;
import gui.node.base.GuiOwner;
import gui.render.RenderEngine;
import gui.render.RenderImage;
import gui.render.RenderingContext;

public class GuiImage extends GuiElement {
	
	private RenderImage image;

	public GuiImage(GuiOwner owner, GuiFrame size) {
		super(owner, size);
	}
	
	@Override
	public void Render(RenderingContext renContext) {
		if (image != null) {
			RenderEngine renderer = renContext.renderer;
			renderer.DrawImage(image, renContext.IntLeft(), renContext.IntTop(),
					renContext.IntRight(), renContext.IntBottom());
		}
	}
	
	public void SetImage(RenderImage image) {
		this.image = image;
	}

}
