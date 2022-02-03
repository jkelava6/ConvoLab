package gui.node.front;

import java.util.function.Consumer;

import gui.GuiFrame;
import gui.action.ActionButtonPressType;
import gui.action.ActionData;
import gui.action.ActionType;
import gui.action.KeyboardActionData;
import gui.node.base.GuiOwner;

public class GuiButton extends GuiBaseButton {
	
	protected Consumer<Void> handler;

	public GuiButton(GuiOwner owner, GuiFrame size) {
		super(owner, size);
		
		SetText("GuiButton instance");
	}
	
	@Override
	public boolean AcceptAction(ActionData action) {
		if (action.type == ActionType.KEYBOARD && action.pressType == ActionButtonPressType.PRESS) {
			KeyboardActionData keyData = (KeyboardActionData)action;
			
			if (keyData.key.equals("Enter")) {
				if (handler != null) {
					handler.accept(null);
				}
				return true;
			}
		}
		
		return super.BasicAcceptAction(action);
	}
	
	@Override
	public void HandleAction(ActionData action) {
		if (handler != null && action.pressType == ActionButtonPressType.DOWN) {
			handler.accept(null);
		}

		BasicHandleAction(action);
	}
	
	public void SetListener(Consumer<Void> listener) {
		handler = listener;
	}

}
