package gui.node.base;

import gui.GuiFrame;
import gui.action.ActionData;
import gui.action.ActionType;
import gui.action.MouseActionData;
import gui.render.RenderColor;
import gui.render.RenderEngine;
import gui.render.RenderingContext;

public class GuiElement {
	
	protected GuiFrame size;
	protected GuiOwner owner;
	protected boolean requestsFocus = true;
	protected RenderColor baseColor = null;
	private RenderEngine Engine = null;
	
	public GuiElement(GuiOwner owner, GuiFrame size) {
		this.owner = owner;
		this.size = size;
		if (owner != null)
		{
			this.Engine = owner.AsElement().GetEngine();
		}
	}
	
	public GuiElement(GuiOwner owner, GuiFrame size, RenderEngine InEngine) {
		this.owner = owner;
		this.size = size;
		Engine = InEngine;
	}
	
	public void SetEngine(RenderEngine InEngine)
	{
		Engine = InEngine;
	}
	
	protected RenderEngine GetEngine()
	{
		return Engine;
	}
	
	protected GuiPoint GetLocalMousePos()
	{
		if (owner != null)
		{
			return size.LocalizePoint(owner.AsElement().GetLocalMousePos());
		}
		if (Engine != null)
		{
			return Engine.GetMousePos();
		}
		return new GuiPoint(-1.0f, -1.0f);
	}
	
	public boolean AcceptAction(ActionData action) {
		if (action.type == ActionType.MOUSE) {
			MouseActionData mouseData = (MouseActionData)action;
			GuiPoint where = mouseData.point;
			if (where.x >= 0 && where.x <= 1 && where.y >= 0 && where.y <= 1) {
				HandleAction(action);
				return true;
			}
		}
		
		return false;
	}
	
	protected boolean AmFocused() {
		return owner == null || owner.CheckFocus(this);
	}
	
	protected void HandleAction(ActionData action) {
		if (owner != null && requestsFocus) {
			owner.RequestFocus(this);
		}
	}
	
	public GuiFrame GetSize() {
		return size;
	}
	
	public void SetSize(GuiFrame size) {
		this.size = size;
	}
	
	public void TakeTick(float step) {
	}
	
	public void OnSwitched(boolean bNowShown) {
	}
	
	public void Render(RenderingContext renContext) {
		RenderEngine renderer = renContext.renderer;
		
		if (baseColor != null) {
			GuiFrame frame = renContext.frame;
			renderer.SetColor(baseColor);
			renderer.FillBox(frame.IntLeft(), frame.IntTop(), frame.IntWidth(), frame.IntHeight());
		}
	}
	
	public int GetRenderingGroup() {
		return 0;
	}
	
	public void SetReqesutsFocus(boolean requests) {
		requestsFocus = requests;
	}
	
	public boolean GetRequestsFocus() {
		return requestsFocus;
	}
	
	public RenderColor GetBaseColor() {
		return baseColor;
	}
	
	public void SetBaseColor(RenderColor color) {
		baseColor = color;
	}

}
