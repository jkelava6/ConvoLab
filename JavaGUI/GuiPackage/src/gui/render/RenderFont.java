package gui.render;

public interface RenderFont {
	
	public void SetName(String name);
	public void SetStyle(int style);
	public void SetSize(int size);

	public String GetName();
	public int GetStyle();
	public int GetSize();
	
	public RenderFont Clone();

}
