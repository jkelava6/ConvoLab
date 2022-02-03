package gui.node.front;

import gui.GuiFrame;
import gui.action.ActionData;
import gui.node.base.GuiGroup;
import gui.node.base.GuiOwner;
import gui.render.RenderColor;
import gui.render.RenderFont;

public class GuiBaseButton extends GuiGroup {
	
	GuiOutlinedBackground guiBackground;
	GuiText guiText;
	GuiImage guiImage;

	public GuiBaseButton(GuiOwner owner, GuiFrame size) {
		super(owner, size);
		
		guiBackground = new GuiOutlinedBackground(owner, GuiFrame.FRAME_FILL());
		subNodes.add(guiBackground);
		
		guiText = new GuiText(owner, GuiFrame.FRAME_FILL());
		subNodes.add(guiText);
		
		guiImage = new GuiImage(owner, GuiFrame.FRAME_FILL());
		subNodes.add(guiImage);
	}
	
	public void SetText(String text) {
		guiText.SetText(text);
	}
	
	public void SetBackgroundColor(RenderColor color) {
		guiBackground.SetFill(color);
	}
	
	public void SetOutlineColor(RenderColor color) {
		guiBackground.SetOutline(color);
	}
	
	public void SetTextColor(RenderColor color) {
		guiText.SetColor(color);
	}
	
	public void SetFont(RenderFont font) {
		guiText.SetFont(font);
	}
	
	public void SetAlignment(HorizontalAlign horizontal, VerticalAlign vertical) {
		guiText.SetAlignment(horizontal, vertical);
	}
	
	public void SetImage(String file) {
		
	}
	
	public void SetImageTextOffset(float imageRatio) {
		guiImage.SetSize(new GuiFrame(0.0f, 0.0f, imageRatio, 1.0f));
		guiText.SetSize(new GuiFrame(imageRatio, 0.0f, 1.0f, 1.0f));
	}
	
	public void SetNoImageTextOffset() {
		guiImage.SetSize(GuiFrame.FRAME_FILL());
		guiText.SetSize(GuiFrame.FRAME_FILL());
	}
	
	public GuiText GetText() {
		return guiText;
	}
	
	public GuiOutlinedBackground GetBackground() {
		return guiBackground;
	}
	
	public GuiImage GetImage() {
		return guiImage;
	}
	
	@Override
	public void HandleAction(ActionData action) {
		if (owner != null && requestsFocus) {
			owner.RequestFocus(this);
		}
	}

}
