package gui.render;

import gui.GuiFrame;
import gui.node.base.GuiPoint;

public interface RenderEngine {
	
	public void SetColor(RenderColor color);
	
	public void DrawBox(int x, int y, int width, int height);
	public void FillBox(int x, int y, int width, int height);
	public void OutlinedBox(int x, int y, int width, int height, int outlineWidth, RenderColor outline, RenderColor fill);

	public void DrawEllipse(int x, int y, int width, int height);
	public void FillEllipse(int x, int y, int width, int height);
	
	public void DrawImage(RenderImage image, int x, int y, int width, int height);
	
	public void SetFont(RenderFont font);
	public GuiFrame GetTextSize(String text);
	public String[] WrapText(String text, String width);
	public void DrawText(String text, int x, int y, int width, int height);
	public RenderFont GetDefaultFont();
	
	public RenderImage LoadImage(String file, GuiFrame area);

	public GuiPoint GetMousePos();

}
