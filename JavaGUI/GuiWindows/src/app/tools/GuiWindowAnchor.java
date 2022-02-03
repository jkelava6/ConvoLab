package app.tools;

import java.awt.MouseInfo;
import java.awt.Point;
import javax.swing.JFrame;

import app.Game;
import gui.GuiFrame;
import gui.action.ActionButtonPressType;
import gui.action.ActionData;
import gui.action.ActionType;
import gui.node.base.GuiOwner;
import gui.node.front.GuiButton;

public class GuiWindowAnchor extends GuiButton {
	
	private int globalMouseX;
	private int globalMouseY;
	private boolean mouseHeld;
	private Game gameInstance;

	public GuiWindowAnchor(GuiOwner owner, GuiFrame size, Game gameInstance) {
		super(owner, size);
		this.gameInstance = gameInstance;
		
		mouseHeld = false;
		SetText("GuiWindowAnchor instance");
	}
	
	@Override
	public void HandleAction(ActionData action) {
		if (action.type == ActionType.MOUSE) {
			if (action.pressType == ActionButtonPressType.DOWN) {
				mouseHeld = true;
				return;
			}
			if (action.pressType == ActionButtonPressType.UP) {
				mouseHeld = false;
				return;
			}
		}
		
		super.HandleAction(action);
	}
	
	@Override
	public void TakeTick(float step) {
		Point mouse = MouseInfo.getPointerInfo().getLocation();
		
		if (mouseHeld && gameInstance != null) {
			JFrame frame = gameInstance.GetFrame();
			Point topLeft = frame.getLocationOnScreen();
			int diffX = mouse.x - globalMouseX;
			int diffY = mouse.y - globalMouseY;
			frame.setLocation(topLeft.x + diffX, topLeft.y + diffY);
		}
		
		globalMouseX = mouse.x;
		globalMouseY = mouse.y;
		
		super.TakeTick(step);
	}
	
	public void SetGameInstance(Game gameInstance) {
		this.gameInstance = gameInstance;
	}

}
