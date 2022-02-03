package main;

import java.util.Scanner;

import app.Launcher;
import gui.FNetworkDisplayUnit;
import gui.GuiFrame;

public class FNetworkVisualizer {

	public static void main(String[] astrArgs) {
		Scanner StandardInput = new Scanner(System.in);
		FNetworkDisplayUnit RootElement = new FNetworkDisplayUnit(null, GuiFrame.FRAME_FILL(), StandardInput);
		Launcher.Launch("ConvoNet Visualizer", RootElement, true, false);
		StandardInput.close();
	}

}
