package gui.render;

import java.util.Comparator;

import gui.node.base.GuiElement;

public class RenderingOrderComparator implements Comparator<GuiElement> {
	
	public RenderingOrderComparator() {
	}
	
	@Override
	public int compare(GuiElement left, GuiElement right) {
		if (left.GetRenderingGroup() < right.GetRenderingGroup()) {
			return -1;
		}
		if (left.GetRenderingGroup() > right.GetRenderingGroup()) {
			return 1;
		}
		return 0;
	}

}
