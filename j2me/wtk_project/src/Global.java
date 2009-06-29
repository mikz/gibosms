import java.util.Vector;
import java.io.*;
import javax.microedition.rms.RecordStore;
import javax.microedition.lcdui.Image;
import javax.microedition.lcdui.Graphics;

public class Global {
	public static byte LINEDIVIDER_1 = '\r';
	public static byte LINEDIVIDER_2 = '\n';

	public static Vector GetLines(byte[] data, int len, boolean utf) {
		Vector retVal = null;
		int last = 0, i;
		for (i = 1; i < len; i++) {
			if (data[i] == Global.LINEDIVIDER_2 && data[i - 1] == Global.LINEDIVIDER_1) {
				String line = utf ? bytesToUTF(data, last, i - last - 1) : new String(data, last, i - last - 1);
				if (line != null) {
					if (retVal == null) retVal = new Vector();
					retVal.addElement(line);
				}
				last = i + 1;
			}
		}
		String line = utf ? bytesToUTF(data, last, len - last) : new String(data, last, len - last);
		if (line != null) {
			if (retVal == null) retVal = new Vector();
			retVal.addElement(line);
		}
		return retVal;
	}

	public static Vector GetFileLines(String fileName, boolean utf) {
		Vector retVal = null;
		try {
			RecordStore rs = null;
			rs = RecordStore.openRecordStore(fileName, false);
			if (rs != null) {
				byte[] data = rs.getRecord(1);
				if (data != null) {
					int len = rs.getRecordSize(1);
					retVal = GetLines(data, len, utf);
				}
				rs.closeRecordStore();
			}
		}
		catch (Exception e) {
			//SMS.showErr(e.toString());
			// nothing
		}
		return retVal;
	}

	public static boolean WriteFileLines(Vector lines, String fileName) {
		boolean ok = false;
		try {
			// Try to delete the record store
			try {
				RecordStore.deleteRecordStore(fileName);
			}catch (Exception e) {
				// Do nothing
			}
			RecordStore rs = RecordStore.openRecordStore(fileName, true);
			if (rs != null) {
				ByteArrayOutputStream data = new ByteArrayOutputStream();
				DataOutputStream dataUTF = new DataOutputStream(data);
				for (int i = 0; i < lines.size(); i++) {
					if (i != 0) {
						dataUTF.writeByte(Global.LINEDIVIDER_1);
						dataUTF.writeByte(Global.LINEDIVIDER_2);
					}
					dataUTF.writeUTF((String)lines.elementAt(i));
				}
				dataUTF.flush();
				rs.addRecord(data.toByteArray(), 0, data.size());
				rs.closeRecordStore();
				data.close();
				dataUTF.close();
				ok = true;
			}
		}
		catch (Exception e) {
			SMS.showErr(e.toString());
			// nothing
		}
		return ok;
	}

	public static boolean WriteVariablesToFile(Vector data, String fileName) {
		Vector lines = new Vector();
		for (int i = 0; i < data.size(); i++) {
			String[] var = (String[])data.elementAt(i);
			lines.addElement(var[0] + "=" + var[1]);
		}
		return WriteFileLines(lines, fileName);
	}

	public static Vector LoadVariablesFromFile(String fileName, boolean utf) {
		Vector lines = GetFileLines(fileName, utf);
		return CreateVariablesStore(lines);
	}

	public static Vector LoadVariablesFromString(String data, boolean utf) {
		Vector lines = GetLines(data.getBytes(), data.length(), utf);
		return CreateVariablesStore(lines);
	}

	public static Vector CreateVariablesStore(Vector lines) {
		Vector data = new Vector();
		if (lines != null) {
			for (int i = 0; i < lines.size(); i++) {
				String str = (String)lines.elementAt(i);
				int iVal = str.indexOf('=');
				if (iVal == -1) continue;
				String var[] = {str.substring(0, iVal), str.substring(iVal + 1)};
				data.addElement(var);
			}
		}
		return data;
	}

	public static String LoadStringFromVariablesStore(Vector data, String varName, String defaultReturn) {
		for (int i = 0; i < data.size(); i++) {
			String[] var = (String[])data.elementAt(i);
			if (var[0].compareTo(varName) == 0) return var[1];
		}
		return defaultReturn;
	}

	public static void WriteStringToVariablesStore(Vector data, String varName, String value, boolean allowDuplicates) {
		if (!allowDuplicates) {
			for (int i = 0; i < data.size(); i++) {
				String[] var = (String[])data.elementAt(i);
				if (var[0].compareTo(varName) == 0) {
					var[1] = value;
					return;
				}
			}
		}
		String var[] = {varName, value};
		data.addElement(var);
	}

	public static int LoadIntFromVariablesStore(Vector data, String varName, int defaultReturn) {
		return atoi(LoadStringFromVariablesStore(data, varName, itoa(defaultReturn)));
	}

	public static void WriteIntToVariablesStore(Vector data, String varName, int value, boolean allowDuplicates) {
		WriteStringToVariablesStore(data, varName, itoa(value), allowDuplicates);
	}

	public static String lPadString(String str, char c, int len) {
		if (str.length() >= len) return str;
		byte buff[] = new byte[len];
		byte bstr[] = str.getBytes();
		for (int i = 0; i < len; i++) {
			buff[i] = i >= len - str.length() ? bstr[i - (len - str.length())] : (byte)c;
		}
		return new String(buff);
	}

	public static boolean IsValidURLChar(byte c) {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ' || c == '.'|| c == '-' || c == '*';
	}

	public static String URLEncode(String url) {
		byte bstr[] = url.getBytes();
		int len = url.length();
		StringBuffer encURL = new StringBuffer();
		for (int i = 0; i < len; i++) {
			if (IsValidURLChar(bstr[i])) {
				encURL.append((char)bstr[i]);
			}else {
				int b = bstr[i] < 0 ? 256 + bstr[i] : bstr[i];
				encURL.append("%" + lPadString(Integer.toHexString(b), '0', 2));
			}
		}
		return encURL.toString().replace(' ', '+');
	}

	public static String ClearSpaces(String str) {
		StringBuffer newstr = new StringBuffer();
		boolean spaceBefore = true;
		int len = str.length();
		for (int i = 0; i < len; i++) {
			if (str.charAt(i) == ' ') {
				spaceBefore = true;
			}else {
				if (spaceBefore) {
					newstr.append(str.substring(i, i + 1).toUpperCase());
					spaceBefore = false;
				}else newstr.append(str.substring(i, i + 1));
			}
		}
		return newstr.toString();
	}

	public static String ClearDiacritics(String str) {
		char[] sa = {'ò','ì','š','è','ø','ž','ý','á','í','é','','ï','ó','Ó','ú','ù','Ù','É','Ú','Í','Ý','Á','Ì','Ø','','Ž','Š','Ï','È','Ò'};
		char[] sb = {'n','e','s','c','r','z','y','a','i','e','t','d','o','O','u','u','U','E','U','I','Y','A','E','R','T','Z','S','D','C','N'};
		for (int i = 0; i < sa.length; i++) {
			str = str.replace(sa[i], sb[i]);
		}

		return str;
	}

	public static int atoi(String s) {
		if (s == null || s == "") return 0;
		int retVal;
		try {
			retVal = Integer.valueOf(s).intValue();
		}catch (Exception e) {
			retVal = 0;
		}
		return retVal;
	}

	public static String itoa(int i) {
		return String.valueOf(i);
	}

	public static String bytesToUTF(byte[] data, int offset, int length) {
		try {
			ByteArrayInputStream buff = new ByteArrayInputStream(data, offset, length);
			DataInputStream buffUTF = new DataInputStream(buff);
			String utfText = buffUTF.readUTF();
			buffUTF.close();
			buff.close();
			return utfText;
		}catch (Exception e) {
			try {
				return new String(data, offset, length);
			}catch (Exception ee) {
				return null;
			}
		}
	}

	public static Image createImageFromPictogram(String data, int width, int height) {
		if (data == null) return null;
		Image img = Image.createImage(width + 6, height + 6);
		Graphics g = img.getGraphics();

		// ohranicujici obedelnik
		g.drawLine(1, 1, width + 4, 1);
		g.drawLine(width + 4, 1, width + 4, height + 4);
		g.drawLine(1, height + 4, width + 4, height + 4);
		g.drawLine(1, 1, 1, height + 4);

		int offsetX = 3, offsetY = 3;

		int actBit = 3;
		int actPos = 0;
		int len = data.length();
		Integer value = Integer.valueOf(data.substring(actPos, actPos + 1), 16);

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				if ((value.intValue() & (1 << actBit)) != 0) g.drawLine(offsetX + x, offsetY + y, offsetX + x, offsetY + y);

				actBit--;
				if (actBit == -1 && actPos + 1 < len) {
					actBit = 3;
					actPos++;
					value = Integer.valueOf(data.substring(actPos, actPos + 1), 16);
				}
			}
		}
		return img;
	}
}