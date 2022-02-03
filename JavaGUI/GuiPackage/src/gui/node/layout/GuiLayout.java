package gui.node.layout;

import gui.GuiFrame;
import gui.node.base.GuiElement;
import gui.node.base.GuiGroup;
import gui.node.base.GuiOwner;

public abstract class GuiLayout extends GuiGroup {
	
	public GuiLayout(GuiOwner owner, GuiFrame size) {
		super(owner, size);
	}
	
	@Override
	public boolean AddElement(GuiElement element) {
		if (super.AddElement(element)) {
			ReorganizeElements();
			return true;
		}
		return false;
	}
	
	@Override
	public boolean RemoveElement(GuiElement element) {
		if (super.RemoveElement(element)) {
			ReorganizeElements();
			return true;
		}
		return false;
	}
	
	protected abstract void ReorganizeElements();

}
