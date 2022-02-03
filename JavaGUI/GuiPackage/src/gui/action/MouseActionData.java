package gui.action;

import gui.node.base.GuiPoint;

public class MouseActionData extends ActionData {
	public GuiPoint point;
	
	public MouseActionData(ActionType type, ActionButtonPressType clickType, GuiPoint point) {
		super(type);
		this.point = point;
		this.pressType = clickType;
	}
}
