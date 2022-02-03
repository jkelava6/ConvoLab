package gui.node.layout;

import gui.GuiFrame;
import gui.node.base.GuiOwner;

public class VerticalLayout extends GuiLayout {
	
	public static float DEFAULT_MAX_SIZE = 1.0f;
	public static float DEFAULT_TOP_MARGIN = 0.0f;
	public static float DEFAULT_BOTTOM_MARGIN = 0.0f;
	
	public float maxSize;
	public float topMargin;
	public float bottomMargin;

	public VerticalLayout(GuiOwner owner, GuiFrame size) {
		super(owner, size);
		maxSize = DEFAULT_MAX_SIZE;
		topMargin = DEFAULT_TOP_MARGIN;
		bottomMargin = DEFAULT_BOTTOM_MARGIN;
	}

	@Override
	protected void ReorganizeElements() {
		float withoutMargins = 1.0f - topMargin - bottomMargin;
		float height = Math.min(maxSize, withoutMargins / subNodes.size());
		for (int i = 0; i < subNodes.size(); i++) {
			GuiFrame childSize = new GuiFrame(0.0f, topMargin + i * height,
					1.0f, topMargin + (i + 1) * height);
			subNodes.elementAt(i).SetSize(childSize);
		}
	}

}
