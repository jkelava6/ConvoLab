package gui;

import gui.node.base.GuiOwner;
import gui.node.front.GuiButton;

public class FSelectionButton extends GuiButton {

	public FSelectionButton(GuiOwner owner, GuiFrame size, int X)
	{
		super(owner, size);

		final int Index = X;
		if (!(owner instanceof FSelectionPanel))
		{
			return;
		}
		FSelectionPanel Panel = (FSelectionPanel)owner;
		
		SetListener((Void Nothing) -> Panel.Select(Index));
	}

}
