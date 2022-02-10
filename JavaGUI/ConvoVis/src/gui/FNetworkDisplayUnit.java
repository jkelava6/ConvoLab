package gui;

import java.util.Scanner;

import gui.node.base.GuiElement;
import gui.node.base.GuiGroup;
import gui.node.base.GuiOwner;
import gui.render.RenderColor;

public class FNetworkDisplayUnit extends GuiGroup {
	
	FSelectionPanel BlockSelection = null;
	FSelectionPanel FeatureSelection = null;
	FMatrixWrapper Display = null;
	
	int NumOfBlocks = 0;
	int[] NumOfFeatures = null;
	int[] XSize = null;
	int[] YSize = null;
	float[][][][] States = null;
	
	int SelectedBlock = 0;
	int SelectedFeature = 0;

	public FNetworkDisplayUnit(GuiOwner owner, GuiFrame size, Scanner Input)
	{
		super(owner, size);
		
		BlockSelection = new FSelectionPanel(this, new GuiFrame(0.0f, 0.0f, 1.0f, 0.1f));
		AddElement(BlockSelection);
		FeatureSelection = new FSelectionPanel(this, new GuiFrame(0.0f, 0.1f, 1.0f, 0.2f));
		AddElement(FeatureSelection);
		Display = new FMatrixWrapper(this, new GuiFrame(0.0f, 0.2f, 1.0f, 1.0f));
		AddElement(Display);
		
		baseColor = RenderColor.COL_LIGHT_BLUE;
		
		// input data
		NumOfBlocks = Input.nextInt();
		XSize = new int[NumOfBlocks];
		YSize = new int[NumOfBlocks];
		NumOfFeatures = new int[NumOfBlocks];
		States = new float[NumOfBlocks][][][];
		
		for (int Block = 0; Block < NumOfBlocks; ++Block)
		{
			XSize[Block] = Input.nextInt();
			YSize[Block] = Input.nextInt();
			NumOfFeatures[Block] = Input.nextInt();
			
			States[Block] = new float[NumOfFeatures[Block]][XSize[Block]][YSize[Block]];
			
			for (int Feature = 0; Feature < NumOfFeatures[Block]; ++Feature)
			{
				for (int Y = 0; Y < YSize[Block]; ++Y)
				{
					for (int X = 0; X < XSize[Block]; ++X)
					{
						States[Block][Feature][X][Y] = Input.nextFloat();
					}
				}
			}
		}
		
		// call initial setups
		BlockSelection.Resize(NumOfBlocks);
		FeatureSelection.Resize(NumOfFeatures[0]);
		UpdateMatrix();
	}
	
	public void Select(GuiElement Panel, int Index)
	{
		if (Panel == BlockSelection)
		{
			SelectedBlock = Index;
			FeatureSelection.Resize(NumOfFeatures[Index]);
			UpdateMatrix();
			return;
		}
		
		if (Panel == FeatureSelection)
		{
			SelectedFeature = Index;
			UpdateMatrix();
			return;
		}
	}
	
	public void UpdateMatrix()
	{
		float[][] NewMatrix = States[SelectedBlock][SelectedFeature];
		
		float Min = NewMatrix[0][0];
		float Max = NewMatrix[0][0];
		
		int HiX = NewMatrix.length;
		int HiY = NewMatrix[0].length;
		
		for (int X = 0; X < HiX; ++X)
		{
			for (int Y = 0; Y < HiY; ++Y)
			{
				Min = Math.min(Min, NewMatrix[X][Y]);
				Max = Math.max(Max, NewMatrix[X][Y]);
			}
		}
				
		Display.SetMatrix(NewMatrix);
		Display.SetRange(Min, Max);
	}

}
