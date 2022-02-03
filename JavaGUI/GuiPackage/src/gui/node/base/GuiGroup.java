package gui.node.base;

import java.util.Collections;
import java.util.Vector;

import gui.GuiFrame;
import gui.action.ActionData;
import gui.action.ActionType;
import gui.action.MouseActionData;
import gui.render.RenderingContext;
import gui.render.RenderingOrderComparator;

public class GuiGroup extends GuiElement implements GuiOwner {
	protected Vector<GuiElement> subNodes = new Vector<GuiElement>();
	protected GuiElement focused;
	protected boolean acceptsFocusRequests = true;

	public GuiGroup(GuiOwner owner, GuiFrame size) {
		super(owner, size);
	}

	@Override
	public GuiElement AsElement()
	{
		return this;
	}
	
	@Override
	public boolean RequestFocus(GuiElement child) {
		if (!subNodes.contains(child) || !acceptsFocusRequests) {
			return false;
		}
		
		focused = child;
		
		return owner == null || owner.RequestFocus(this);
	}
	
	@Override
	public void ForceFocus(GuiElement element, boolean propagate) {
		focused = element;
		if (propagate && owner != null) {
			owner.ForceFocus(this, true);
		}
	}
	
	public boolean AddElement(GuiElement element) {
		if (!subNodes.contains(element)) {
			subNodes.add(element);
			return true;
		}
		return false;
	}
	
	public boolean RemoveElement(GuiElement element) {
		if (subNodes.contains(element)) {
			subNodes.remove(element);
			if (element == focused)
			{
				focused = null;
			}
			return true;
		}
		return false;
	}
	
	@Override
	public boolean CheckFocus(GuiElement child) {
		return focused == child && AmFocused();
	}
	
	public boolean BasicAcceptAction(ActionData action) {
		return super.AcceptAction(action);
	}
	
	@Override
	public boolean AcceptAction(ActionData action) {
		if (focused != null && PassToChild(focused, action)) {
			return true;
		}
		
		return super.AcceptAction(action);
	}
	
	public void BasicHandleAction(ActionData action) {
		super.HandleAction(action);
	}
	
	@Override
	protected void HandleAction(ActionData action) {
		for (GuiElement child : subNodes) {
			if (PassToChild(child, action)) {
				return;
			}
		}
	}
	
	protected boolean PassToChild(GuiElement child, ActionData action) {
		ActionData passed = action;
		if (action.type == ActionType.MOUSE) {
			MouseActionData mouseAction = (MouseActionData)action;
			passed = new MouseActionData(mouseAction.type, mouseAction.pressType,
					child.size.LocalizePoint(mouseAction.point));
		}
		
		return child.AcceptAction(passed);
	}
	
	@Override
	public void TakeTick(float step) {
		for (GuiElement child : subNodes) {
			child.TakeTick(step);
		}
	}
	
	@Override
	public void Render(RenderingContext renContext) {
		super.Render(renContext);
		for (GuiElement child : subNodes) {
			child.Render(renContext.SubFrame(child.size));
		}
	}
	
	public void UpdateRenderingOrder() {
		Collections.sort(subNodes, new RenderingOrderComparator());
	}
	
	public void Clear() {
		focused = null;
		subNodes.clear();
	}
	
	public Vector<GuiElement> GetSubNoeds(){
		return subNodes;
	}

}
