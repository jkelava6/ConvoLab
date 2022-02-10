package gui;

import gui.node.base.GuiGroup;
import gui.node.base.GuiOwner;
import gui.node.front.GuiText;
import gui.node.front.HorizontalAlign;
import gui.node.front.VerticalAlign;

public class FMatrixWrapper extends GuiGroup
{
	FMatrixVisualizer Matrix = null;
	FSlider SliderLight = null;
	FSlider SliderDark = null;
	
	GuiText MaxValueLabel = null;
	GuiText MinValueLabel = null;
	GuiText DisplayedRangeLabel = null;
	
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

		MaxValueLabel = new GuiText(this, new GuiFrame(0.85f, 0.35f, 0.95f, 0.4f));
		MaxValueLabel.SetAlignment(HorizontalAlign.CENTER, VerticalAlign.CENTER);
		AddElement(MaxValueLabel);
		MinValueLabel = new GuiText(this, new GuiFrame(0.85f, 0.6f, 0.95f, 0.65f));
		MinValueLabel.SetAlignment(HorizontalAlign.CENTER, VerticalAlign.CENTER);
		AddElement(MinValueLabel);
		DisplayedRangeLabel = new GuiText(this, new GuiFrame(0.85f, 0.7f, 0.95f, 0.75f));
		DisplayedRangeLabel.SetAlignment(HorizontalAlign.CENTER, VerticalAlign.CENTER);
		AddElement(DisplayedRangeLabel);
	}
	
	public void SetMatrix(float[][] InMatrix)
	{
		Matrix.SetMatrix(InMatrix);
	}
	
	public void SetRange(float InMinValue, float InMaxValue)
	{
		MinValue = InMinValue;
		MaxValue = InMaxValue;
		MinValueLabel.SetText(String.format("%.2f", MinValue));
		MaxValueLabel.SetText(String.format("%.2f", MaxValue));
	}
	
	@Override
	public void TakeTick(float DeltaTime)
	{
		super.TakeTick(DeltaTime);
		
		final float LowValue = MinValue + SliderLight.GetSliderValue() * (MaxValue - MinValue);
		final float HighValue = MinValue + SliderDark.GetSliderValue() * (MaxValue - MinValue);
		Matrix.SetRange(LowValue, HighValue);
		DisplayedRangeLabel.SetText(String.format("%.2f <-> %.2f", LowValue, HighValue));
	}

}
