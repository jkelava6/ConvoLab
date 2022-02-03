package app.render;

import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

import gui.GuiFrame;
import gui.render.RenderImage;

public class WindowsImage extends RenderImage {
	
	private BufferedImage image = null;

	public WindowsImage(byte[] imageString, GuiFrame area) {
		super(imageString, area.SubFrame(GuiFrame.FRAME_FILL()));
		try {
			image = ImageIO.read(new ByteArrayInputStream(imageString));
			//System.out.println(image);
			if (image == null) {
				return;
			}
			int width = image.getWidth();
			int height = image.getHeight();
			ScaleImageSize(width, height);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public WindowsImage(String path, GuiFrame area) {
		super(null, area.SubFrame(GuiFrame.FRAME_FILL()));
		try {
			image = ImageIO.read(new File(path));
			this.area.loX *= image.getWidth(null);
			this.area.hiX *= image.getWidth(null);
			this.area.loY *= image.getHeight(null);
			this.area.hiY *= image.getHeight(null);
			//System.out.printf("%f %f %f %f\n", area.loX, area.loY, area.hiX, area.hiY);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public Image GetImage() {
		return image;
	}

	public void ScaleImageSize(int width, int height) {
		area.loX *= width;
		area.hiX *= width;
		area.loY *= height;
		area.hiY *= height;
	}

}
