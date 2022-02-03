package gui;

import gui.node.base.GuiOwner;
import gui.node.layout.HorizontalLayout;

public class FSelectionPanel extends HorizontalLayout {
	
	int LastSelected = 0;

	public FSelectionPanel(GuiOwner owner, GuiFrame size)
	{
		super(owner, size);
	}
	
	public void Resize(int Size)
	{
		if (subNodes.size() == Size)
		{
			return;
		}
		
		while (subNodes.size() > Size)
		{
			RemoveElement(subNodes.get(Size));
		}
		
		while (subNodes.size() < Size)
		{
			FSelectionButton Btn = new FSelectionButton(this, GuiFrame.FRAME_FILL(), subNodes.size());
			Btn.SetText(subNodes.size() == LastSelected ? "X" : "");
			subNodes.add(Btn);
		}
		
		ReorganizeElements();

		if (LastSelected >= Size)
		{
			Select(Size - 1);
		}
	}
	
	public void Select(int Index)
	{
		if (Index == LastSelected)
		{
			return;
		}
		
		if (LastSelected < subNodes.size())
		{
			FSelectionButton Btn = (FSelectionButton)subNodes.get(LastSelected);
			Btn.SetText("");
		}
		
		{
			FSelectionButton Btn = (FSelectionButton)subNodes.get(Index);
			Btn.SetText("X");
		}
		
		LastSelected = Index;
		
		if (!(owner instanceof FNetworkDisplayUnit))
		{
			return;
		}
		FNetworkDisplayUnit NDU = (FNetworkDisplayUnit)owner;
		NDU.Select(this, Index);
	}

}
