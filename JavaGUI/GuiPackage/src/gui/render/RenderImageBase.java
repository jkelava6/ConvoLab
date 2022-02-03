package gui.render;

import java.io.File;
import java.io.FileReader;
import java.util.TreeMap;
import java.util.Vector;

import gui.GuiFrame;

public abstract class RenderImageBase {
	
	protected TreeMap<String, byte[]> library;
	
	public RenderImageBase(boolean storeInMemory) {
		if (storeInMemory) {
			library = new TreeMap<String, byte[]>();
		}
	}
		
		public RenderImage LoadImage(String file, GuiFrame area) {
		byte[] contents = null;
		
		if (library != null && library.containsKey(file)) {
			contents = library.get(file);
		}
		
		if (contents == null) {
			try (FileReader reader = new FileReader(new File(file))) {
				Vector<Byte> bytes = new Vector<Byte>();
				int read;
				while ((read = reader.read()) != -1) {
					bytes.add(Byte.valueOf((byte)read));
				}
				contents = new byte[bytes.size()];
				for (int i = 0; i < bytes.size(); ++i) {
					contents[i] = bytes.elementAt(i);
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

		if (contents != null && library != null) {
			library.put(file, contents);
		}
		
		return CreateImage(contents, area);
	}
	
	public abstract RenderImage CreateImage(byte[] imageString, GuiFrame area);

}
