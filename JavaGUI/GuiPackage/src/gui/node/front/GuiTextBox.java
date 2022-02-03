package gui.node.front;

import gui.GuiFrame;
import gui.action.ActionButtonPressType;
import gui.action.ActionData;
import gui.action.ActionType;
import gui.action.KeyboardActionData;
import gui.node.base.GuiOwner;
import gui.render.RenderColor;

public class GuiTextBox extends GuiBaseButton {
	
	public static String tbox_strLettersCapital = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	public static String tbox_strLettersNonCapital = "abcdefghijklmnopqrstuvwxyz";
	public static String tbox_strNumbers = "0123456789";
	public static String tbox_strSigns = ".,:;<>/?\"\'\\[]{}`~!@#$%^&*()-_=+";
	public static String tbox_strLettersAll = tbox_strLettersCapital + tbox_strLettersNonCapital;
	public static String tbox_strAlphaNumerical = tbox_strLettersAll + tbox_strNumbers;
	public static String tbox_strAllCharacters = tbox_strAlphaNumerical + tbox_strSigns;
	public static boolean FOCUS_CHECK = false;
	
	protected String tbox_strDictionary;

	public GuiTextBox(GuiOwner owner, GuiFrame size) {
		this(owner, size, tbox_strAllCharacters);
	}
	
	public GuiTextBox(GuiOwner owner, GuiFrame size, String dictionary) {
		super(owner, size);
		this.SetText("GuiTextBox instance");
		this.SetBaseColor(RenderColor.COL_WHITE);
		this.SetAlignment(HorizontalAlign.LEFT, VerticalAlign.CENTER);
		tbox_strDictionary = dictionary;
	}
	
	@Override
	public boolean AcceptAction(ActionData action) {
		if (action.type == ActionType.KEYBOARD && (AmFocused() || !FOCUS_CHECK)) {
			KeyboardActionData data = (KeyboardActionData)action;
			
			if (data.key.equals("Backspace")) {
				if (data.pressType == ActionButtonPressType.PRESS) {
					String str = guiText.GetText();
					if (str.length() > 0) {
						guiText.SetText(str.substring(0, str.length() - 1));
					}
				}
				return true;
			}

			if (data.pressType == ActionButtonPressType.DOWN) {
				if (tbox_strDictionary.contains(data.typed)) {
					guiText.SetText(guiText.GetText() + data.typed);
				}
				return true;
			}
		}
		
		return super.BasicAcceptAction(action);
	}

}
