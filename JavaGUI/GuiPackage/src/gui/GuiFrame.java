package gui;

import gui.node.base.GuiPoint;

public class GuiFrame {
	
	public static float frm_fPixelRoundingEpsilon = 1e-3f;
	
	public static GuiFrame FRAME_FILL() {
		return new GuiFrame(0, 0, 1, 1);
	}
	
	public float loX;
	public float loY;
	public float hiX;
	public float hiY;
	
	public GuiFrame(float width, float height) {
		loX = 0;
		loY = 0;
		hiX = width;
		hiY = height;
	}
	
	public GuiFrame(float loX, float loY, float hiX, float hiY) {
		this.loX = loX;
		this.loY = loY;
		this.hiX = hiX;
		this.hiY = hiY;
	}
	
	private GuiFrame() {
	}
	
	public GuiFrame SubFrame(GuiFrame subFrame) {
		GuiFrame child = new GuiFrame();
		float width = hiX - loX;
		float height = hiY - loY;
		
		child.loX = loX + subFrame.loX * width;
		child.loY = loY + subFrame.loY * height;
		child.hiX = loX + subFrame.hiX * width;
		child.hiY = loY + subFrame.hiY * height;
		
		return child;
	}
	
	public GuiFrame Scaled(float fSize) {
		GuiFrame frmScaled = new GuiFrame();
		float fMidX = 0.5f * (loX + hiX);
		float fMidY = 0.5f * (loY + hiY);
		
		frmScaled.loX = fMidX + fSize * (loX - fMidX);
		frmScaled.hiX = fMidX + fSize * (hiX - fMidX);
		frmScaled.loY = fMidY + fSize * (loY - fMidY);
		frmScaled.hiY = fMidY + fSize * (hiY - fMidY);
		
		return frmScaled;
	}
	
	public GuiPoint LocalizePoint(GuiPoint global) {
		float width = hiX - loX;
		float height = hiY - loY;
		GuiPoint local = new GuiPoint((global.x - loX) / width, (global.y - loY) / height);
		return local;
	}
	
	@Override
	public String toString() {
		return String.format("GuiFrame(%f, %f - %f, %f)", loX, loY, hiX, hiY);
	}
	
	public float GetLeft() {
		return loX;
	}
	
	public float GetTop() {
		return loY;
	}
	
	public float GetRight() {
		return hiX;
	}
	
	public float GetBottom() {
		return hiY;
	}
	
	public float GetWidth() {
		return hiX - loX;
	}
	
	public float GetHeight() {
		return hiY - loY;
	}
	
	public int IntLeft() {
		return (int) (loX + frm_fPixelRoundingEpsilon);
	}
	
	public int IntTop() {
		return (int) (loY + frm_fPixelRoundingEpsilon);
	}
	
	public int IntRight() {
		return (int) (hiX + frm_fPixelRoundingEpsilon);
	}
	
	public int IntBottom() {
		return (int) (hiY + frm_fPixelRoundingEpsilon);
	}
	
	public int IntWidth() {
		return IntRight() - IntLeft();
	}
	
	public int IntHeight() {
		return IntBottom() - IntTop();
	}

}
