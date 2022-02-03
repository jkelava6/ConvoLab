package app.render;

import gui.GuiFrame;
import gui.render.RenderImage;
import gui.render.RenderImageBase;

public class WindowsImageBase extends RenderImageBase {

	public WindowsImageBase(boolean storeInMemory) {
		super(storeInMemory);
	}
	
	@Override
	public RenderImage LoadImage(String file, GuiFrame area) {
		return new WindowsImage(file, area);
	}

	@Override
	public RenderImage CreateImage(byte[] imageString, GuiFrame area) {
		if (imageString == null) {
			return null;
		}
		return new WindowsImage(imageString, area);
	}

}
