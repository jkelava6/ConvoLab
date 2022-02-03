package app.render;

import java.awt.Font;

import gui.render.RenderFont;

public class WindowsFont implements RenderFont {
	
	private Font font;
	
	public WindowsFont(Font original) {
		font = new Font(original.getName(), original.getStyle(), original.getSize());
	}
	
	public WindowsFont() {
		font = new Font(Font.DIALOG, Font.PLAIN, 16);
	}

	@Override
	public void SetName(String name) {
		font = new Font(name, font.getStyle(), font.getSize());
	}

	@Override
	public void SetStyle(int style) {
		font = new Font(font.getName(), style, font.getSize());
	}

	@Override
	public void SetSize(int size) {
		font = new Font(font.getName(), font.getStyle(), size);
	}

	@Override
	public RenderFont Clone() {
		return new WindowsFont(font);
	}
	
	@Override
	public String GetName() {
		return font.getName();
	}

	@Override
	public int GetStyle() {
		return font.getStyle();
	}

	@Override
	public int GetSize() {
		return font.getSize();
	}
	
	public Font GetWindowsFont() {
		return font;
	}

}
