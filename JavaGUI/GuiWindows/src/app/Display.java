package app;

import java.awt.Canvas;
import java.awt.Dimension;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.WindowEvent;

import javax.swing.JFrame;

public class Display {
	
	public static Display debugDisplay = null;
	
	private JFrame frame;
	private Canvas canvas;
	
	private String title;
	private int width;
	private int height;
	
	public Display(String title, int width, int height, boolean decorated, boolean fixedSize) {
		this.title = title;
		this.width = width;
		this.height = height;
		
		CreateDisplay(decorated, fixedSize);
		debugDisplay = this;
	}
	
	private void CreateDisplay(boolean decorated, boolean fixedSize) {
		frame = new JFrame(title);
		frame.setSize(width, height);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setUndecorated(!decorated);
		frame.setResizable(!fixedSize);
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
		
		canvas = new Canvas();
		canvas.setPreferredSize(new Dimension(width, height));
		canvas.setMaximumSize(new Dimension(width, height));
		canvas.setMinimumSize(new Dimension(width, height));
		
		frame.add(canvas);
		frame.pack();
		width = canvas.getWidth();
		height = canvas.getHeight();
		
		canvas.addComponentListener(new ComponentListener(){

			@Override
			public void componentResized(ComponentEvent e) {
				width = canvas.getWidth();
				height = canvas.getHeight();
			}

			@Override
			public void componentMoved(ComponentEvent e) {
			}

			@Override
			public void componentShown(ComponentEvent e) {
			}

			@Override
			public void componentHidden(ComponentEvent e) {
			}
			
		});
	}
	
	public JFrame GetFrame() {
		return frame;
	}
	
	public Canvas getCanvas() {
		return canvas;
	}
	
	public void Close() {
		frame.dispatchEvent(new WindowEvent(frame, WindowEvent.WINDOW_CLOSING));
	}
	
	public int GetWidth() {
		return width;
	}
	
	public int GetHeight() {
		return height;
	}

}
