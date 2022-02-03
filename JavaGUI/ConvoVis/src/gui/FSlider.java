package gui;

import gui.action.ActionButtonPressType;
import gui.action.ActionData;
import gui.action.ActionType;
import gui.action.MouseActionData;
import gui.node.base.GuiElement;
import gui.node.base.GuiOwner;
import gui.render.RenderColor;
import gui.render.RenderEngine;
import gui.render.RenderingContext;

public class FSlider extends GuiElement
{
	private boolean bIsHoldingMouse = false;
	private float LastMouseY;
	private float SliderValue = 0.0f;

	public FSlider(GuiOwner owner, GuiFrame size)
	{
		super(owner, size);
	}
	
	public void SetSliderValue(float Value)
	{
		SliderValue = Value;
	}
	
	public float GetSliderValue()
	{
		return SliderValue;
	}

	@Override
	protected void HandleAction(ActionData Action)
	{
		if (Action.type == ActionType.MOUSE)
		{
			MouseActionData MouseAction = (MouseActionData)Action;
			if (Action.pressType == ActionButtonPressType.DOWN)
			{
				bIsHoldingMouse = true;
				LastMouseY = MouseAction.point.y;
				
			}
			if (Action.pressType == ActionButtonPressType.UP)
			{
				bIsHoldingMouse = false;
			}
		}
		
		super.HandleAction(Action);
	}
	
	@Override
	public void TakeTick(float DeltaTime)
	{
		super.TakeTick(DeltaTime);
		
		if (bIsHoldingMouse)
		{
			float MouseY = GetLocalMousePos().y;
			SetSliderValue(Math.max(0, Math.min(1, GetSliderValue() - (MouseY - LastMouseY))));
			LastMouseY = MouseY;
			
			if (MouseY < 0.0f || MouseY > 1.0f)
			{
				bIsHoldingMouse = false;
			}
		}
	}
	
	@Override
	public void Render(RenderingContext Context)
	{
		RenderEngine Engine = Context.renderer;
		GuiFrame Frame = Context.frame;
		
		Engine.SetColor(RenderColor.COL_WHITE);
		Engine.FillBox(Frame.IntLeft(), Frame.IntTop(), Frame.IntWidth(), Frame.IntHeight());
		Engine.SetColor(RenderColor.COL_BLACK);
		Engine.FillBox(Frame.IntLeft(), Frame.IntTop() + (int)((1.0f - SliderValue) * Frame.GetHeight()) - 2, Frame.IntWidth(), 5);
		
	}

}
