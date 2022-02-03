package gui.node.front;

import gui.GuiFrame;
import gui.node.base.GuiElement;
import gui.node.base.GuiOwner;
import gui.render.RenderColor;
import gui.render.RenderEngine;
import gui.render.RenderingContext;

public class GuiOutlinedBackground extends GuiElement {
	
	public static RenderColor DEFAULT_FILL = RenderColor.COL_LIGHT_GRAY;
	public static RenderColor DEFAULT_OUTLINE = RenderColor.COL_BLACK;
	private RenderColor outline;
	
	public GuiOutlinedBackground(GuiOwner owner, GuiFrame size) {
		super(owner, size);
		baseColor = DEFAULT_FILL;
		outline = DEFAULT_OUTLINE;
	}
	
	@Override
	public void Render(RenderingContext renContext) {
		super.Render(renContext);
		
		RenderEngine renderer = renContext.renderer;

		if (outline != null) {
			renderer.SetColor(outline);
			renderer.DrawBox(renContext.IntLeft(), renContext.IntTop(), renContext.IntWidth(), renContext.IntHeight());
		}
	}
	
	public void SetFill(RenderColor fill) {
		this.baseColor = fill;
	}
	
	public void SetOutline(RenderColor outline) {
		this.outline = outline;
	}

}
