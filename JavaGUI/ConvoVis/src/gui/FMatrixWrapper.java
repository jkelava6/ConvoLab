package gui;

import gui.node.base.GuiGroup;
import gui.node.base.GuiOwner;

public class FMatrixWrapper extends GuiGroup
{
	FMatrixVisualizer Matrix = null;
	FSlider SliderLight = null;
	FSlider SliderDark = null;
	
	float MinValue = -1.0f;
	float MaxValue = 1.0f;

	public FMatrixWrapper(GuiOwner owner, GuiFrame size)
	{
		super(owner, size);
		
		Matrix = new FMatrixVisualizer(this, new GuiFrame(0.0f, 0.0f, 0.8f, 1.0f));
		AddElement(Matrix);
		
		SliderLight = new FSlider(this, new GuiFrame(0.83f, 0.4f, 0.88f, 0.6f));
		AddElement(SliderLight);
		SliderDark = new FSlider(this, new GuiFrame(0.92f, 0.4f, 0.97f, 0.6f));
		SliderDark.SetSliderValue(1.0f);
		AddElement(SliderDark);
	}
	
	public void SetMatrix(float[][] InMatrix)
	{
		Matrix.SetMatrix(InMatrix);
	}
	
	public void SetRange(float InMinValue, float InMaxValue)
	{
		MinValue = InMinValue;
		MaxValue = InMaxValue;
	}
	
	@Override
	public void TakeTick(float DeltaTime)
	{
		super.TakeTick(DeltaTime);
		
		Matrix.SetRange(MinValue + SliderLight.GetSliderValue() * (MaxValue - MinValue),
				MinValue + SliderDark.GetSliderValue() * (MaxValue - MinValue));
	}

}
