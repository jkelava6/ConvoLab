package gui.action;

public class KeyboardActionData extends ActionData {
	public String key;
	public String typed = null;

	public KeyboardActionData(ActionType type, ActionButtonPressType clickType, String pressed) {
		super(type);
		
		this.pressType = clickType;
		key = pressed;
	}

	public KeyboardActionData(ActionType type, ActionButtonPressType clickType, String pressed, String typed) {
		super(type);
		
		this.pressType = clickType;
		key = pressed;
		this.typed = typed;
	}

}
