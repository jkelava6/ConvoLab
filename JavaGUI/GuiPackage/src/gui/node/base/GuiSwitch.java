package gui.node.base;

import java.util.TreeMap;

import gui.GuiFrame;
import gui.action.ActionData;
import gui.render.RenderingContext;

public class GuiSwitch extends GuiElement implements GuiOwner {
	protected TreeMap<String, GuiElement> elements;
	protected GuiElement shown;
	protected boolean acceptsShownFocusRequests = true;
	protected boolean acceptsHiddenFocusRequests = false;

	public GuiSwitch(GuiOwner owner, GuiFrame size) {
		super(owner, size);
		elements = new TreeMap<String, GuiElement>();
	}

	@Override
	public GuiElement AsElement()
	{
		return this;
	}
	
	@Override
	public boolean AcceptAction(ActionData action) {
		if (shown != null) {
			if (shown.AcceptAction(action)) {
				return true;
			}
		}
		
		return super.AcceptAction(action);
	}
	
	@Override
	protected void HandleAction(ActionData action) {
		if (shown != null) {
			shown.AcceptAction(action);
		}
	}
	
	@Override
	public void Render(RenderingContext renContext) {
		if (shown != null) {
			shown.Render(renContext);
		}
	}
	
	public void RegisterElement(String name, GuiElement element) {
		elements.put(name, element);
	}
	
	public void SwitchToElement(String name) {
		GuiElement wanted = elements.get(name);
		SwitchToElement(wanted);
	}
	
	public void SwitchToElement(GuiElement element) {
		if (shown != element) {
			if (shown != null) {
				shown.OnSwitched(false);
			}
			shown = element;
			if (shown != null) {
				shown.OnSwitched(true);
			}
		}
	}
	
	public void NoElement() {
		shown = null;
	}
	
	@Override
	public boolean RequestFocus(GuiElement child) {
		if (shown == child && acceptsShownFocusRequests) {
			return owner == null || owner.RequestFocus(this);
		}
		
		if (elements.containsValue(child) && acceptsHiddenFocusRequests) {
			SwitchToElement(child);
			return owner == null || owner.RequestFocus(this);
		}
		return false;
	}

	@Override
	public boolean CheckFocus(GuiElement child) {
		return shown == child && AmFocused();
	}

	@Override
	public void ForceFocus(GuiElement element, boolean propagate) {
		shown = element;
		
		if (propagate && owner != null) {
			owner.ForceFocus(this, propagate);
		}
	}
	
	public void Clear() {
		shown = null;
		elements.clear();
	}
	
	public void TakeTick(float fStep) {
		if (shown != null) {
			shown.TakeTick(fStep);
		}
	}

}
