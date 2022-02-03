package gui.node.layout;

import gui.GuiFrame;
import gui.node.base.GuiOwner;

public class HorizontalLayout extends GuiLayout {
	
	public static float DEFAULT_MAX_SIZE = 1.0f;
	public static float DEFAULT_LEFT_MARGIN = 0.0f;
	public static float DEFAULT_RIGHT_MARGIN = 0.0f;
	
	public float maxSize;
	public float leftMargin;
	public float rightMargin;

	public HorizontalLayout(GuiOwner owner, GuiFrame size) {
		super(owner, size);
		maxSize = DEFAULT_MAX_SIZE;
		leftMargin = DEFAULT_LEFT_MARGIN;
		rightMargin = DEFAULT_RIGHT_MARGIN;
	}

	@Override
	protected void ReorganizeElements() {
		float withoutMargins = 1.0f - leftMargin - rightMargin;
		float width = Math.min(maxSize, withoutMargins / subNodes.size());
		for (int i = 0; i < subNodes.size(); i++) {
			GuiFrame childSize = new GuiFrame(leftMargin + i * width, 0.0f,
					leftMargin + (i + 1) * width, 1.0f);
			subNodes.elementAt(i).SetSize(childSize);
		}
	}

}
