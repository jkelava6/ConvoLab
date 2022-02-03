package gui.node.front;

import gui.GuiFrame;
import gui.node.base.GuiElement;
import gui.node.base.GuiOwner;
import gui.render.RenderColor;
import gui.render.RenderEngine;
import gui.render.RenderFont;
import gui.render.RenderingContext;

public class GuiText extends GuiElement {
	
	public static RenderColor DEFAULT_COLOR = RenderColor.COL_BLACK;
	public static String DEFAULT_TEXT = "GuiText instance";
	public static RenderFont DEFAULT_FONT = null;
	private RenderColor color;
	private String text;
	private RenderFont font;
	
	private HorizontalAlign horizontalAlign;
	private VerticalAlign verticalAlign;
	
	public GuiText(GuiOwner owner, GuiFrame size) {
		super(owner, size);
		color = DEFAULT_COLOR;
		text = DEFAULT_TEXT;
		font = DEFAULT_FONT;
		horizontalAlign = HorizontalAlign.CENTER;
		verticalAlign = VerticalAlign.CENTER;
	}
	
	@Override
	public void Render(RenderingContext renContext) {
		RenderEngine renderer = renContext.renderer;
		renderer.SetColor(color);
		if (font == null) {
			font = renderer.GetDefaultFont();
		}
		renderer.SetFont(font);
		GuiFrame textRect = renderer.GetTextSize(text);
		
		int left = renContext.IntLeft() - (int)textRect.GetLeft();
		if (horizontalAlign == HorizontalAlign.CENTER) {
			left += (renContext.IntWidth() - (int)textRect.GetWidth()) / 2;
		}
		if (horizontalAlign == HorizontalAlign.RIGHT) {
			left += renContext.IntWidth() - (int)textRect.GetWidth();
		}
		
		int top = renContext.IntTop() - (int)textRect.GetTop();
		if (verticalAlign == VerticalAlign.CENTER) {
			top += (renContext.IntHeight() - (int)textRect.GetHeight()) / 2;
		}
		if (verticalAlign == VerticalAlign.BOTTOM) {
			top += renContext.IntHeight() - (int)textRect.GetHeight();
		}

		renderer.DrawText(text, left, top, renContext.IntWidth(), renContext.IntHeight());
	}
	
	public void SetText(String text) {
		this.text = text;
	}
	
	public String GetText() {
		return text;
	}
	
	public void SetColor(RenderColor color) {
		this.color = color;
	}
	
	public void SetFont(RenderFont font) {
		this.font = font;
	}
	
	public RenderFont GetFont() {
		return font;
	}
	
	public void SetAlignment(HorizontalAlign horizontal, VerticalAlign vertical) {
		horizontalAlign = horizontal;
		verticalAlign = vertical;
	}

}
