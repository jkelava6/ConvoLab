package app.render;

import java.awt.Color;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.geom.Rectangle2D;
import app.Game;
import gui.GuiFrame;
import gui.node.base.GuiPoint;
import gui.render.RenderColor;
import gui.render.RenderEngine;
import gui.render.RenderFont;
import gui.render.RenderImage;

public class WindowsRenderer implements RenderEngine {
	
	RenderColor color;
	RenderFont font;
	
	Graphics canvas;
	WindowsImageBase imageBase = new WindowsImageBase(true);
	Game GameInstance;
	
	public WindowsRenderer(Graphics canvas, Game InGameInstance) {
		this.canvas = canvas;
		GameInstance = InGameInstance;
	}

	@Override
	public void SetColor(RenderColor color) {
		this.color = color;
		canvas.setColor(new Color(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha()));
	}

	@Override
	public void DrawBox(int x, int y, int width, int height) {
		canvas.drawRect(x, y, width, height);
	}

	@Override
	public void FillBox(int x, int y, int width, int height) {
		canvas.fillRect(x, y, width, height);
	}

	@Override
	public void OutlinedBox(int x, int y, int width, int height, int outlineWidth, RenderColor outline, RenderColor fill) {
		// store the original color
		RenderColor originalColor = color;
		
		// clamp the outline width
		outlineWidth = Math.min(outlineWidth, Math.min(width, height) / 2);
		int iFillWidth = width - 2 * outlineWidth;
		int iFillHeight = height - 2 * outlineWidth;
		
		// calculate relevant coordinates
		int iOuterLeftX = x;
		int iInnerLeftX = iOuterLeftX + outlineWidth;
		int iOuterTopY = y;
		int iInnerTopY = iOuterTopY + outlineWidth;
		int iOuterRightX = iOuterLeftX + width;
		int iInnerRightX = iOuterRightX - outlineWidth;
		int iOuterBottomY = iOuterTopY + height;
		int iInnerBottomY = iOuterBottomY - outlineWidth;
		
		// draw the outlines
		SetColor(outline);
		FillBox(iOuterLeftX, iOuterTopY, outlineWidth, height);
		FillBox(iInnerLeftX, iOuterTopY, iFillWidth, outlineWidth);
		FillBox(iInnerLeftX, iInnerBottomY, iFillWidth, outlineWidth);
		FillBox(iInnerRightX, iOuterTopY, outlineWidth, height);
		
		// draw the inside
		SetColor(fill);
		FillBox(iInnerLeftX, iInnerTopY, iFillWidth, iFillHeight);
		
		// restore the original color
		SetColor(originalColor);
	}
	
	@Override
	public void DrawEllipse(int x, int y, int width, int height) {
		canvas.drawOval(x, y, width, height);
	}
	
	@Override
	public void FillEllipse(int x, int y, int width, int height) {
		canvas.fillOval(x, y, width, height);
	}

	@Override
	public void DrawImage(RenderImage image, int x, int y, int width, int height) {
		if (!(image instanceof WindowsImage)) {
			return;
		}
		
		WindowsImage wImage = (WindowsImage)image;
		canvas.drawImage(wImage.GetImage(),
			x, y,
			x + width, y + height,
			wImage.GetArea().IntLeft(), wImage.GetArea().IntTop(),
			wImage.GetArea().IntRight(), wImage.GetArea().IntBottom(),
			null);
	}

	@Override
	public void SetFont(RenderFont font) {
		WindowsFont winFont = (WindowsFont)font;
		canvas.setFont(winFont.GetWindowsFont());
	}

	@Override
	public GuiFrame GetTextSize(String text) {
		FontMetrics metrics = canvas.getFontMetrics();
		Rectangle2D area = metrics.getStringBounds(text, canvas);
		return new GuiFrame((float)area.getMinX(), (float)area.getMinY(),
				(float)area.getMaxX(), (float)area.getMaxY());
	}

	@Override
	public String[] WrapText(String text, String width) {
		String[] wrapped = new String[1];
		wrapped[0] = text;
		return wrapped;
	}

	@Override
	public void DrawText(String text, int x, int y, int width, int height) {
		canvas.drawString(text, x, y);
	}

	@Override
	public RenderFont GetDefaultFont() {
		return new WindowsFont();
	}

	@Override
	public RenderImage LoadImage(String file, GuiFrame area) {
		return imageBase.LoadImage(file, area);
	}
	
	@Override
	public GuiPoint GetMousePos()
	{
		return GameInstance.GetMousePos();
	}

}
