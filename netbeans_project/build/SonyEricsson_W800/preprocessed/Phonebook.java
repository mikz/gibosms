import java.util.Vector;

public class Phonebook {
	public class PhonebookItem {
		public String name;
		public String number;
		public String email;

		public PhonebookItem(String new_name, String new_number, String new_email) {
			name = new_name;
			number = new_number != null ? new_number : "";
			email = new_email != null ? new_email : "";
		}
	}

	public Vector phones = new Vector();
	protected final String phonebookFile = "phonebook2";

	public int GetCount() {
		return phones.size();
	}

	public PhonebookItem Get(int index) {
		return (PhonebookItem)phones.elementAt(index);
	}

	public void Import(Vector lines) {
		if (lines != null) {
			for (int i = 0; i < lines.size(); i++) {
				String str = (String)lines.elementAt(i);
				String name = null, number = null, email = null;
				int iNum = str.indexOf('=');
				if (iNum == -1) continue;
				int iMail = str.indexOf('|', iNum);
				if (iMail > 0) Add(str.substring(0, iNum), str.substring(iNum + 1, iMail), str.substring(iMail + 1));
				else Add(str.substring(0, iNum), str.substring(iNum + 1), null);
			}
		}
	}

	public Vector Export() {
		Vector lines = new Vector();
		for (int i = 0; i < phones.size(); i++) {
			PhonebookItem phItm = (PhonebookItem)phones.elementAt(i);
			lines.addElement(phItm.name + "=" + phItm.number + "|" + phItm.email);
		}
		return lines;
	}

	public void Load() {
		Vector lines = Global.GetFileLines(phonebookFile, true);
		Import(lines);
	}

	public void Write() {
		Global.WriteFileLines(Export(), phonebookFile);
	}

	public void Add(String name, String number, String email) {
		for (int i = 0; i < phones.size(); i++) {
			PhonebookItem phItm = (PhonebookItem)phones.elementAt(i);
			if (phItm.name.compareTo(name) == 0) return;
		}
		phones.addElement(new PhonebookItem(name, number, email));
	}

	public void Change(int index, String name, String number, String email) {
		phones.setElementAt(new PhonebookItem(name, number, email), index);
	}

	public void Delete(int index) {
		phones.removeElementAt(index);
	}

	public void DeleteAll() {
		phones.removeAllElements();
	}

	protected int CompareItems(int index1, int index2) {
		String name1 = ((PhonebookItem)phones.elementAt(index1)).name;
		String name2 = ((PhonebookItem)phones.elementAt(index2)).name;
		return name1.compareTo(name2);
	}

	public void Sort() {
		for (int i = 0; i < GetCount(); i++) {
			int minIndex = i;
			for (int n = i + 1; n < GetCount(); n++) {
				if (CompareItems(minIndex, n) > 0) minIndex = n;
			}
			if (minIndex != i) {
				Object o = phones.elementAt(minIndex);
				phones.setElementAt(phones.elementAt(i), minIndex);
				phones.setElementAt(o, i);
			}
		}
	}
}