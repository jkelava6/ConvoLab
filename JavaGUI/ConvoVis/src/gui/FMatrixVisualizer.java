package gui;

import gui.node.base.GuiElement;
import gui.node.base.GuiOwner;
import gui.render.RenderColor;
import gui.render.RenderEngine;
import gui.render.RenderingContext;
import main.FCVSettings;

public class FMatrixVisualizer extends GuiElement {
	
	private float[][] Matrix = null;
	private float MinValue = 0.0f;
	private float MaxValue = 1.0f;

	public FMatrixVisualizer(GuiOwner owner, GuiFrame size)
	{
		super(owner, size);
	}
	
	public void SetMatrix(float[][] InMatrix)
	{
		Matrix = InMatrix;
	}
	
	public void SetRange(float InLowerBound, float InUpperBound)
	{
		MinValue = InLowerBound;
		MaxValue = InUpperBound;
	}
	
	@Override
	public void Render(RenderingContext Context)
	{
		if (Matrix == null)
		{
			return;
		}
		RenderEngine Engine = Context.renderer;
		GuiFrame Frame = Context.frame;
		
		final int XSize = Matrix.length;
		final int YSize = Matrix[0].length;
		
		for (int X = 0; X < XSize; ++X)
		{
			final int LoX = (int)(Frame.GetLeft() + (X * Frame.GetWidth()) / XSize);
			final int HiX = (int)(Frame.GetLeft() + ((X + 1) * Frame.GetWidth()) / XSize);
			for (int Y = 0; Y < YSize; ++Y)
			{
				final int LoY = (int)(Frame.GetTop() + (Y * Frame.GetHeight()) / YSize);
				final int HiY = (int)(Frame.GetTop() + ((Y + 1) * Frame.GetHeight()) / YSize);
				
				float RangeRatio = (Matrix[X][Y] - MinValue) / (MaxValue - MinValue);
				RangeRatio = Math.max(0, Math.min(1, RangeRatio));
				
				int R;
				int G;
				int B;
				if (RangeRatio < 0.5f)
				{
					float ColorRatio = 2.0f * RangeRatio;
					R = (int)((1.0f - ColorRatio) * FCVSettings.ColorZero.GetRed() + ColorRatio * FCVSettings.ColorHalf.GetRed());
					G = (int)((1.0f - ColorRatio) * FCVSettings.ColorZero.GetGreen() + ColorRatio * FCVSettings.ColorHalf.GetGreen());
					B = (int)((1.0f - ColorRatio) * FCVSettings.ColorZero.GetBlue() + ColorRatio * FCVSettings.ColorHalf.GetBlue());
				}
				else
				{
					float ColorRatio = 2.0f * (RangeRatio - 0.5f);
					R = (int)((1.0f - ColorRatio) * FCVSettings.ColorHalf.GetRed() + ColorRatio * FCVSettings.ColorOne.GetRed());
					G = (int)((1.0f - ColorRatio) * FCVSettings.ColorHalf.GetGreen() + ColorRatio * FCVSettings.ColorOne.GetGreen());
					B = (int)((1.0f - ColorRatio) * FCVSettings.ColorHalf.GetBlue() + ColorRatio * FCVSettings.ColorOne.GetBlue());
				}
				Engine.SetColor(new RenderColor(R, G, B));
				Engine.FillBox(LoX, LoY, HiX - LoX, HiY - LoY);
			}
		}
	}

}
