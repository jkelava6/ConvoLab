package app;

import gui.GuiFrame;
import gui.node.base.GuiElement;

public class Launcher {
	
	public static void main(String args[]) {
		Launch("A Window", new GuiElement(null, new GuiFrame(1.0f, 1.0f)), true, false);
	}
	
	public static Game Launch(String strWindowName, GuiElement guiRoot, boolean decorated, boolean fixedSize) {
		Game game = new Game(strWindowName, 640, 480, decorated, fixedSize);
		game.Start(guiRoot);
		return game;
	}

}
