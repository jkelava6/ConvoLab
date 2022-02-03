package gui.render;

public class RenderColor {
	
	private final static int _offsetAlpha = 24;
	private final static int _offsetRed = 16;
	private final static int _offsetGreen = 8;
	private final static int _offsetBlue = 0;
	private final static int _maskAlpha = 0xff000000;
	private final static int _maskRed = 0x00ff0000;
	private final static int _maskGreen = 0x0000ff00;
	private final static int _maskBlue = 0x000000ff;
	private final static int _maskRight = 0x000000ff;
	
	public int code = 0;
	
	public RenderColor(int code) {
		this.code = code;
	}
	
	public RenderColor(int alpha, int red, int green, int blue) {
		code = alpha << _offsetAlpha
				| red << _offsetRed
				| green << _offsetGreen
				| blue << _offsetBlue;
	}
	
	public RenderColor(int red, int green, int blue) {
		this(255, red, green, blue);
	}
	
	public RenderColor(float alpha, float red, float green, float blue) {
		this((int)(255 * alpha), (int)(255 * red), (int)(255 * green), (int)(255 * blue));
	}
	
	public RenderColor(float red, float green, float blue) {
		this(1.0f, red, green, blue);
	}
	
	public RenderColor Clone() {
		return new RenderColor(code);
	}
	
	public int GetAlpha() {
		return ((code & _maskAlpha) >> _offsetAlpha) & _maskRight;
	}
	
	public int GetRed() {
		return (code & _maskRed) >> _offsetRed;
	}
	
	public int GetGreen() {
		return (code & _maskGreen) >> _offsetGreen;
	}
	
	public int GetBlue() {
		return (code & _maskBlue) >> _offsetBlue;
	}
	
	public void SetAlpha(int value) {
		code = (code & ~_maskAlpha) | (value << _offsetAlpha);
	}
	
	public void SetRed(int value) {
		code = (code & ~_maskRed) | (value << _offsetRed);
	}
	
	public void SetGreen(int value) {
		code = (code & ~_maskGreen) | (value << _offsetGreen);
	}
	
	public void SetBlue(int value) {
		code = (code & ~_maskBlue) | (value << _offsetBlue);
	}
	
	public final static RenderColor COL_WHITE = new RenderColor(0xffffffff);
	public final static RenderColor COL_LIGHT_GRAY = new RenderColor(0xffc0c0c0);
	public final static RenderColor COL_GRAY = new RenderColor(0xff808080);
	public final static RenderColor COL_DARK_GRAY = new RenderColor(0xff404040);
	public final static RenderColor COL_BLACK = new RenderColor(0xff000000);
	
	public final static RenderColor COL_VERY_LIGHT_RED = new RenderColor(0xffffbfbf);
	public final static RenderColor COL_LIGHT_RED = new RenderColor(0xffff8080);
	public final static RenderColor COL_SLIGHTLY_LIGHT_RED = new RenderColor(0xffff4040);
	public final static RenderColor COL_RED = new RenderColor(0xffff0000);
	public final static RenderColor COL_SLIGHTLY_DARK_RED = new RenderColor(0xffbf0000);
	public final static RenderColor COL_DARK_RED = new RenderColor(0xff800000);
	public final static RenderColor COL_VERY_DARK_RED = new RenderColor(0xff400000);

	public final static RenderColor COL_VERY_LIGHT_GREEN = new RenderColor(0xffbfffbf);
	public final static RenderColor COL_LIGHT_GREEN = new RenderColor(0xff80ff80);
	public final static RenderColor COL_SLIGHTLY_LIGHT_GREEN = new RenderColor(0xff40ff40);
	public final static RenderColor COL_GREEN = new RenderColor(0xff00ff00);
	public final static RenderColor COL_SLIGHTLY_DARK_GREEN = new RenderColor(0xff00bf00);
	public final static RenderColor COL_DARK_GREEN = new RenderColor(0xff008000);
	public final static RenderColor COL_VERY_DARK_GREEN = new RenderColor(0xff004000);

	public final static RenderColor COL_LIGHT_BLUE = new RenderColor(0xff8080ff);
	public final static RenderColor COL_BLUE = new RenderColor(0xff0000ff);
	public final static RenderColor COL_DARK_BLUE = new RenderColor(0xff000080);

	public final static RenderColor COL_LIGHT_YELLOW = new RenderColor(0xffffff80);
	public final static RenderColor COL_YELLOW = new RenderColor(0xffffff00);
	public final static RenderColor COL_DARK_YELLOW = new RenderColor(0xff808000);

	public final static RenderColor COL_LIGHT_MAGENTA = new RenderColor(0xffff80ff);
	public final static RenderColor COL_MAGENTA = new RenderColor(0xffff00ff);
	public final static RenderColor COL_DARK_MAGENTA = new RenderColor(0xff800080);

	public final static RenderColor COL_LIGHT_CYAN = new RenderColor(0xff80ffff);
	public final static RenderColor COL_CYAN = new RenderColor(0xff00ffff);
	public final static RenderColor COL_DARK_CYAN = new RenderColor(0xff008080);
}
