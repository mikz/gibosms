import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;
import javax.microedition.io.*;
import java.io.*;
import java.util.*;
import java.lang.*;

public class SMS extends MIDlet implements CommandListener {
	private final String _VERSION = "2.14";
	private final static String _CLIENT = "mobile";
	private boolean writeSetting = false;
	private boolean writePhonebook = false;
	private boolean saveText = false;
        public static Display display;
	private Form frmNumber = null;
	private Form frmSetting = null;
	private Form frmAdvSetting = null;
	private Form frmStatus = null;
	private Form frmPhonebookItem = null;
	private Form frmConfirm = null;
	private Form frmStatistics = null;
	private Form frmAbout = null;
	private Form frmConfirmSend = null;
	private Form frmSubject = null;
	private Form frmConfirmClearNewSMS = null;
	private Form frmPassword = null;
	private Form frmMsgsRead = null;
	private Form frmJumpOver = null;
	private Form frmSearch = null;
	private Form frmInputPictogram = null;
	private Form frmSettingWarning = null;
	private Displayable prevDisplay = null;
	private Displayable backDisplay = null;
	private Displayable pbBackDisplay = null;
	private Canvas cnvsWait = null;
	private boolean bAddAction = false;
	private TextField ctrlNumber = null;
	private TextBox ctrlMessage = null;
	private List lstPhonebook = null;
	private List lstImport = null;
	private List lstInformation = null;
	private List lstSendWays = null;
	private List lstMsgsReadType = null;
	private List lstTMAccounts = null;
	private Phonebook phonebook = new Phonebook();
	private final static Command CMD_ADVSETTING = new Command("Pokroèilé", Command.ITEM, 1);
	private final static Command CMD_SETTING = new Command("Nastavení", Command.ITEM, 1);
	private final static Command CMD_CLEAR = new Command("Vymazat text", Command.ITEM, 1);
	private final static Command CMD_INSERTSYMBOL = new Command("Smajlík", Command.ITEM, 1);
	private final static Command CMD_SEND = new Command("Poslat", Command.OK, 1);
	private final static Command CMD_OK = new Command("OK", Command.OK, 1);
	private final static Command CMD_EXIT = new Command("Konec", Command.EXIT, 4);
	private final static Command CMD_BACK = new Command("Zpìt", Command.BACK, 2);
	private final static Command CMD_NEWSMS = new Command("Nová SMS", Command.ITEM, 2);
	private final static Command CMD_NEWSMS_BACK = new Command("Zpìt", Command.BACK, 2);
	private final static Command CMD_RETRY = new Command("Opakovat", Command.ITEM, 1);
	private final static Command CMD_PHONEBOOK = new Command("Adresáø", Command.ITEM, 1);
	private final static Command CMD_ADD = new Command("Pøidat", Command.ITEM, 1);
	private final static Command CMD_EDIT = new Command("Upravit", Command.ITEM, 1);
	private final static Command CMD_DELETE = new Command("Smazat", Command.ITEM, 1);
	private final static Command CMD_DELETEALL = new Command("Smazat vše", Command.ITEM, 1);
	private final static Command CMD_EXPORT = new Command("Export", Command.ITEM, 1);
	private final static Command CMD_IMPORT = new Command("Import", Command.ITEM, 1);
	private final static Command CMD_YES = new Command("Ano", Command.OK, 1);
	private final static Command CMD_NO = new Command("Ne", Command.BACK, 1);
	private final static Command CMD_CLEARSPACES = new Command("Vymazat mezery", Command.ITEM, 1);
	private final static Command CMD_CLEARDIACRITICS = new Command("Odstranit diakritiku", Command.ITEM, 1);
	private final static Command CMD_UNDO = new Command("Vrátit text", Command.ITEM, 1);
	private final static Command CMD_CANCEL = new Command("Pøerušit", Command.ITEM, 1);
	private final static Command CMD_INSERTNUMBER = new Command("Vložit èíslo", Command.ITEM, 1);
	private final static Command CMD_INSERTEMAIL = new Command("Vložit email", Command.ITEM, 1);
	private final static Command CMD_MANUALINSERT = new Command("Vložit ruènì", Command.ITEM, 1);
	private final static Command CMD_INFORMATION = new Command("Informace", Command.ITEM, 1);
	private final static Command CMD_INFOSELECT = new Command("Vybrat", Command.ITEM, 1);
	private final static Command CMD_SENDWAYSELECT = new Command("Vybrat", Command.ITEM, 1);
	private final static Command CMD_MSGSREADTYPESELECT = new Command("Vybrat", Command.ITEM, 1);
	private final static Command CMD_RETRYSENDNORMALSMS = new Command("Poslat jako normální SMS", Command.ITEM, 1);
	private final static Command CMD_RETRYSENDPAIDSMS = new Command("Poslat za 1 Kè", Command.ITEM, 1);
	private final static Command CMD_MESSAGESREADING = new Command("Ètení zpráv", Command.ITEM, 1);
	private final static Command CMD_NEXTPAGE = new Command("Další zprávy", Command.ITEM, 1);
	private final static Command CMD_PREVIOUSPAGE = new Command("Pøedchozí zprávy", Command.ITEM, 1);
	private final static Command CMD_SEARCH = new Command("Vyhledat", Command.ITEM, 1);
	private final static Command CMD_COPYTOEDITFIRST = new Command("Zkopírovat první zprávu", Command.ITEM, 1);
	private final static Command CMD_COPYTOEDITALL = new Command("Zkopírovat vše", Command.ITEM, 1);
	private final static Command CMD_REFRESH = new Command("Obnovit", Command.ITEM, 1);
	private final static Command CMD_JUMPOVER = new Command("Pøesunout se o...", Command.ITEM, 1);
	private final static Command CMD_JUMPFORWARD = new Command("Dopøedu", Command.ITEM, 1);
	private final static Command CMD_JUMPBACKWARD = new Command("Dozadu", Command.ITEM, 1);
	private final static Command CMD_SETTINGWARNING = new Command("Upozornìní", Command.ITEM, 1);
	private final static Command CMD_TMACCOUNTSELECT = new Command("Vybrat", Command.ITEM, 1);
	private Setting setting = new Setting();
	private String lastMessage = null;
	private Thread runningThread = null;
	private int sendWay = SENDWAY.GPRSMSG;
	private boolean send_retry = false;
	private String message_id = "";
	private String pictogram = "";
	private Timer tCounter = null;
	private String selected_tm_account = "";

	private String msgsReadType;
	private String msgsReadPassword;
	private boolean msgsReadCorrectPasswordSubmitted = false;
	private String msgsReadCont = "";
	private String msgsReadSearch_text = "";
	private int msgsReadPos = 0;
	private int msgsReadPage = 3;
	private long msgsReadStartTime = 0, msgsReadEndTime = 0;
	private int msgsReadMsg_0_start = 0, msgsReadMsg_0_length = 0;

	private String receiver = "";
	private String subject = "";
	private boolean deleteAll = false;
	private String undoText = null;
	private boolean pbInMainMenu = false;
	private int caretPos = 0;
	private final static String giboServer = "http://www.gibosms.cz/";
	//private final static String giboServer = "http://localhost/sms/";
        
        // edit
        private boolean initialized =  false;
        private StreamConnection gprs_holder = null;
        private	DataInputStream gprs_holder_is = null;
        private DataOutputStream gprs_holder_os = null; 
        private final static Command CMD_MINIMIZE = new Command("Minimalizovat", Command.ITEM, 1);
        private Form mini = null;

	public SMS() {
		display = Display.getDisplay(this);
	}

 	class CTimer extends TimerTask {
		private int tick = 0;
		private int lastWrittenChars = 0;
		private int lastCaretPos = 0;
                private String sett = setting.title;
                private String title = null;

		public void run() {
			this.tick++;
			if (this.tick % 60 == 0) 
                        {
                            System.gc();
                            this.tick = 0; 
                        }
			if (isShown(ctrlMessage)) {
                                this.title = "";
				Title title = new Title();
				this.title = title.parse(ctrlMessage.getString().length(),ctrlMessage.getMaxSize());
				//ctrlMessage.setTitle(String.valueOf(writtenChars) + "/" + String.valueOf(ctrlMessage.getMaxSize() - ctrlMessage.getString().length()) + " " + String.valueOf(partsV) + ":" + (vodafone_chars*partsV - writtenChars)+ "/" + String.valueOf(partsO) + ":" + (o2_chars*partsO - writtenChars));
                                ctrlMessage.setTitle(this.title);
			}
		}
	}

	public boolean getURL(String page, String request, Thread currentThread, StringBuffer output, StringBuffer extras) {
		boolean ok = false;
		HttpConnection c = null;
		InputStream is = null;
                OutputStream os = null;

                if(setting.holdGPRS) 
                    if(gprs_holder == null)
                    {
                        try{
                          gprs_holder = (StreamConnection)Connector.open("http://www.gibosms.cz/send.php");
                            gprs_holder_os = gprs_holder.openDataOutputStream();
                            gprs_holder_os.writeByte(0);
                            gprs_holder_os.flush();
                        }
                        catch(Exception e)
                        {
                            System.out.println("exeption: "+e.toString());
                        }
                    }   
       
                        
		try {
			c = (HttpConnection)Connector.open(page);

			// should continue?
			

			// Set the request method and headers
			c.setRequestMethod(HttpConnection.POST);

			c.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
                      //  c.setRequestProperty("Connection", "keep-alive");
			String s6 = System.getProperty("microedition.configuration");
			String s7 = System.getProperty("microedition.profiles");
			int i8 = s7.indexOf(' ');
			if (i8 != -1) {
				s7 = s7.substring(0, i8 - 1);
			}
			String s9 = System.getProperty("microedition.platform");
			String s10 = new StringBuffer().append("Profile/").append(s7).append(" Configuration/").append(s6).append(" Platform/").append(s9).toString();
			//c.setRequestProperty("User-Agent", s10);
			// Getting the output stream may flush the headers
			os = c.openOutputStream();
			os.write(request.getBytes());
			os.close();

			// should continue?
			if (runningThread != currentThread) return ok;

			// read content
			is = c.openInputStream();
			StringBuffer data = new StringBuffer();
			StringBuffer dataExtras = new StringBuffer();
			int ch;
			while ((ch = is.read()) != -1) {
				if (ch == 0) {
					// nacist extra data
					while ((ch = is.read()) != -1) {
						dataExtras.append((char)ch);
					}
					break;
				}
				data.append((char)ch);
			}
			is.close();

			c.close();
			output.append(data.toString());
			if (extras != null) extras.append(dataExtras.toString());
			ok = true;
		}
		catch (Exception e) {
			// should continue?
			if (runningThread != currentThread) return ok;
			output.append(e.getMessage());
		}

		return ok;
	}

	class CSendMSG extends Thread {
		private String destination;
		private String src_number;
		private String message;
		private String subject;
		private int sendWay;
		private String msg_id;
		private String pictogram;
		private String selected_tm_account;
		private boolean send_retry;

		public CSendMSG(String destination, String src_number, String message, String subject, int sendWay, String message_id, String pictogram, String selected_tm_account, boolean send_retry) {
			this.destination = destination;
			this.src_number = src_number;
			this.message = message;
			this.subject = subject;
			this.sendWay = sendWay;
			this.msg_id = message_id;
			this.pictogram = pictogram;
			this.selected_tm_account = selected_tm_account;
			this.send_retry = send_retry;
		}

		public void run() {
			boolean ok = false;
			StringBuffer ret = new StringBuffer();
			StringBuffer extras = new StringBuffer();
			if (sendWay != SENDWAY.NORMALSMS) {
				String request = (sendWay != SENDWAY.EMAIL ? "d_n=" : "d_e=") + Global.URLEncode(destination) + (sendWay == SENDWAY.EMAIL ? ("&s_e=" + setting.email) : "") + "&pwd=" + Global.URLEncode(setting.pwd) + (sendWay == SENDWAY.EMAIL ? ("&sbj=" + Global.URLEncode(subject)) : "") + "&s_n=" + Global.URLEncode(src_number) + "&msg=" + Global.URLEncode(message) + "&sm=" + setting.nSentMessages + "&sendway=" + sendWay;
				request +=  "&message_id=" + Global.URLEncode(this.msg_id) + "&pictogram=" + Global.URLEncode(this.pictogram) + "&selected_tm_account=" + Global.URLEncode(this.selected_tm_account) + "&send_retry=" + (this.send_retry ? "1" : "0");
				request +=  "&client=" + _CLIENT + "&ver=" + _VERSION;

				ok = getURL(giboServer + "send.php", request, currentThread(), ret, extras);
				if (ok) {
					ok = ret.length() > 0 ? ret.charAt(0) == '1' : false;
					if (ret.length() > 0) ret.deleteCharAt(0);
					if (ok) {
						setting.nSentMessages++;
						// !!! writeSetting = true;
					}
				}
			}else {
				ok = MIDP2.sendNormalSMS(destination, message, ret);
			}
			// should continue?
			if (runningThread != currentThread()) return;

			Vector varStore = Global.LoadVariablesFromString(extras.toString(), false);

			message_id = Global.LoadStringFromVariablesStore(varStore, "message_id", "");

			String action = Global.LoadStringFromVariablesStore(varStore, "action", "");
			if (action.compareTo("enter_pictogram") == 0) {
				showInputPictogram(Global.LoadStringFromVariablesStore(varStore, "pictogram_data", ""), Global.LoadIntFromVariablesStore(varStore, "pictogram_width", 0), Global.LoadIntFromVariablesStore(varStore, "pictogram_height", 0), Global.LoadIntFromVariablesStore(varStore, "pictogram_numeric", 0) != 0);
			}else if (action.compareTo("select_tm_account") == 0) {
				int numAccounts = Global.LoadIntFromVariablesStore(varStore, "num_tm_accounts", 0);
				Vector accounts = new Vector();
				for (int i = 0; i < numAccounts; i++) {
					accounts.addElement(Global.LoadStringFromVariablesStore(varStore, "tm_account" + i, ""));
				}
				showSelectTMAccount(accounts, Global.LoadStringFromVariablesStore(varStore, "selected_tm_account", ""));
			}else {
				// show status
				backDisplay = ctrlMessage;
				showStatus("Stav odeslání", ok, ret.toString(), true, true, true, sendWay != SENDWAY.EMAIL, ok ? CMD_NEWSMS : CMD_NEWSMS_BACK);
			}
		}
	}

	class CShowPage extends Thread {
		private String page;
		private String request;
		private String title;

		public CShowPage(String page, String request, String title) {
			this.page = page;
			this.request = request;
			this.title = title;
		}

		public void run() {
			StringBuffer ret = new StringBuffer();
			boolean ok = getURL(page, request, currentThread(), ret, null);
			// should continue?
			if (runningThread != currentThread()) return;
			// show status
			showStatus(title, ok, ret.toString(), false, false, false, false, CMD_BACK);
		}
	}

	class CPhonebookExport extends Thread {
		public void run() {
			StringBuffer ret = new StringBuffer();
			StringBuffer request = new StringBuffer();
			Vector lines = phonebook.Export();
			for (int i = 0; i < lines.size(); i++) {
				request.append((String)lines.elementAt(i));
				request.append("\r\n");
			}
			boolean ok = getURL(giboServer + "pb_export.php", "num=" + setting.srcNum + "&pwd=" + Global.URLEncode(setting.pwd) + "&data=" + Global.URLEncode(request.toString()) + "&client=" + _CLIENT + "&ver=" + _VERSION, currentThread(), ret, null);
			if (ok) {
				ok = ret.length() > 0 ? ret.charAt(0) == '1' : false;
				if (ret.length() > 0) ret.deleteCharAt(0);
			}
			// should continue?
			if (runningThread != currentThread()) return;
			// show status
			backDisplay = lstPhonebook;
			showStatus("Export adresáøe...", ok, ret.toString(), false, false, false, false, CMD_BACK);
		}
	}

	class CPhonebookImportReadList extends Thread {
		public void run() {
			StringBuffer ret = new StringBuffer();
			boolean ok = getURL(giboServer + "pb_import.php", "num=" + setting.srcNum + "&action=list" + "&client=" + _CLIENT + "&ver=" + _VERSION, currentThread(), ret, null);
			if (ok) {
				ok = ret.length() > 0 ? ret.charAt(0) == '1' : false;
				if (ret.length() > 0) ret.deleteCharAt(0);
			}
			// should continue?
			if (runningThread != currentThread()) return;
			// show status
			backDisplay = lstPhonebook;
			if (!ok) showStatus("Import adresáøe...", ok, ret.toString(), false, false, false, false, CMD_BACK);
			else showSelectImport(ret.toString());
		}
	}

	class CPhonebookImportReadData extends Thread {
		private int id;

		public CPhonebookImportReadData(int id) {
			this.id = id;
		}

		public void run() {
			StringBuffer ret = new StringBuffer();
			boolean ok = getURL(giboServer + "pb_import.php", "num=" + setting.srcNum + "&action=read" + "&id=" + id + "&pwd=" + Global.URLEncode(setting.pwd) + "&client=" + _CLIENT + "&ver=" + _VERSION, currentThread(), ret, null);
			if (ok) {
				ok = ret.length() > 0 ? ret.charAt(0) == '1' : false;
				if (ret.length() > 0) ret.deleteCharAt(0);
			}
			// should continue?
			if (runningThread != currentThread()) return;
			// show status
			backDisplay = lstImport;
			if (!ok) showStatus("Import adresáøe...", ok, ret.toString(), false, false, false, false, CMD_BACK);
			else {
				phonebook.Import(Global.GetLines(ret.toString().getBytes(), ret.length(), false));
				phonebook.Sort();
				writePhonebook = true;
				showPhonebook(receiver, pbInMainMenu);
			}
		}
	}

	class CExit extends Thread {
		public void run() {
			saveEverything();
			destroyApp(false);
		}
	}

	class CShowMsgsRead extends Thread {
		private String pwd;

		public CShowMsgsRead(String pwd) {
			this.pwd = pwd;
		}

		public void run() {
			StringBuffer ret = new StringBuffer();
			StringBuffer extras = new StringBuffer();
			boolean ok = getURL(giboServer + "messages.php", "num=" + setting.srcNum +
				"&pwd=" + this.pwd + "&type=" + msgsReadType + "&cont=" + Global.URLEncode(msgsReadCont) +
				"&search_text=" + Global.URLEncode(msgsReadSearch_text) + "&pos=" + msgsReadPos + "&page=" + msgsReadPage +
				"&startTime=" + msgsReadStartTime + "&endTime=" + msgsReadEndTime + "&varsformat=lines" + "&client=" + _CLIENT + "&ver=" + _VERSION, currentThread(), ret, extras);
			if (ok) {
				ok = ret.length() > 0 ? ret.charAt(0) == '1' : false;
				if (ret.length() > 0) ret.deleteCharAt(0);
			}

			// should continue?
			if (runningThread != currentThread()) return;

			showMsgsRead(ok, ret.toString(), extras.toString());
		}
	}

	public static void showErr(String text) {
		display.setCurrent(new Alert("Chyba", text, null, AlertType.ERROR));
	}

	public static void showWarning(String text) {
		display.setCurrent(new Alert("Informace", text, null, AlertType.WARNING));
	}

	protected void saveEverything() {
		String msg = saveText ? ctrlMessage.getString() : "";
		if (!writeSetting) writeSetting = setting.msg.compareTo(msg) != 0;
		setting.msg = msg.replace('\r', ' ').replace('\n', ' ');


		if (writeSetting) {
			setting.Write();
			writeSetting = false;
		}
		if (writePhonebook && phonebook != null) {
			phonebook.Write();
			writePhonebook = false;
		}
	}

	protected void initCtrlMessage(String msg, boolean addUndoButton, boolean show) {
		ctrlMessage = new TextBox("", msg, 760, setting.capitalizeSentences ? MIDP2_TextField.ANYINITIAL_CAPS_SENTENCE : TextField.ANY);
		// add menu commands
		ctrlMessage.addCommand(CMD_SEND);
		ctrlMessage.addCommand(CMD_INSERTSYMBOL);
		if (addUndoButton) ctrlMessage.addCommand(CMD_UNDO);
		ctrlMessage.addCommand(CMD_CLEARSPACES);
		ctrlMessage.addCommand(CMD_CLEARDIACRITICS);
		ctrlMessage.addCommand(CMD_CLEAR);
		ctrlMessage.addCommand(CMD_MESSAGESREADING);
		ctrlMessage.addCommand(CMD_INFORMATION);
		ctrlMessage.addCommand(CMD_PHONEBOOK);
		ctrlMessage.addCommand(CMD_SETTING);
                ctrlMessage.addCommand(CMD_MINIMIZE);
		ctrlMessage.addCommand(CMD_EXIT);

		ctrlMessage.setCommandListener(this);

		if (show) display.setCurrent(ctrlMessage);
	}

	protected void startApp() {
            if(!initialized)
            {
                initialized = true;
                System.out.println("loading seddings");
		setting.Load();
                System.out.println("loading phonebook");
		phonebook.Load();
                System.out.println("loading counter");
		// set tmer for diplaying number of already written letters
		if (setting.showCounter) {
			tCounter = new Timer();
			tCounter.schedule(new CTimer(), 1000, 1000);
		}
                System.out.println("loading message window");
		initCtrlMessage(setting.msg, false, false);

		if (setting.srcNum.length() > 0)
                {
                    System.out.println("setting display to message");
                    display.setCurrent(ctrlMessage);
                }
		else showSetting();
               
            }
            else {	
                System.out.println("loading message window(initialized)");
                if(isShown(ctrlMessage)) {
                    initCtrlMessage(setting.msg, false, true);
                }
                else initCtrlMessage(setting.msg, false, false);
            }
	}

	protected void destroyApp(boolean unconditional) {
		if (unconditional) saveText = true;
		saveEverything();
		notifyDestroyed();
	}

  protected void pauseApp() {
           setting.msg = ctrlMessage.getString();
	}

	protected void refresh() {
		display.setCurrent(display.getCurrent());
	}

	protected boolean isShown(Displayable dis) {
		return display.getCurrent() == dis;
	}
	protected void showAdvSetting() {
	    frmAdvSetting = new Form("Pokroèilé nastavení");
	    StringItem si = new StringItem("","Nastavte si možnosti odeslání:");
	    
	    ChoiceGroup cg = new ChoiceGroup("Vymazat \"Email\"", ChoiceGroup.MULTIPLE);
	    cg.append("Ano", null);
	    if (setting.SWdelEmail) cg.setSelectedIndex(0, true);
	    frmAdvSetting.append(cg);
	    
	    cg = new ChoiceGroup("Vymazat \"Gibomeska\"", ChoiceGroup.MULTIPLE);
	    cg.append("Ano", null);
	    if (setting.SWdelGibo) cg.setSelectedIndex(0, true);
	    frmAdvSetting.append(cg);
	    
	    cg = new ChoiceGroup("Vymazat \"Normální SMS\"", ChoiceGroup.MULTIPLE);
	    cg.append("Ano", null);
	    if (setting.SWdelNorm) cg.setSelectedIndex(0, true);
	    frmAdvSetting.append(cg);
	    
	    cg = new ChoiceGroup("Vymazat \"SMS za 1 Kè\"", ChoiceGroup.MULTIPLE);
	    cg.append("Ano", null);
	    if (setting.SWdelPaid) cg.setSelectedIndex(0, true);
	    frmAdvSetting.append(cg);
	    	    
	    cg = new ChoiceGroup("Vymazat \"Mobilní email\"", ChoiceGroup.MULTIPLE);
	    cg.append("Ano", null);
	    if (setting.SWdelMemail) cg.setSelectedIndex(0, true);
	    frmAdvSetting.append(cg);
	    
	                    
            TextField tf = new TextField("Nastavení poèitadla znakù:", setting.title, 100, TextField.ANY);
	    frmAdvSetting.append(tf);
	    
	    frmAdvSetting.addCommand(CMD_BACK);
	    frmAdvSetting.addCommand(CMD_OK);
	    frmAdvSetting.setCommandListener(this);

	    display.setCurrent(frmAdvSetting);
	}
	
	protected void showSetting() {
		frmSetting = new Form("Nastavení");
		TextField tf = new TextField("Vaše èíslo:", setting.srcNum, 9, TextField.NUMERIC);
		frmSetting.append(tf);

		tf = new TextField("Vaše heslo:", setting.pwd, 100, TextField.PASSWORD);
		frmSetting.append(tf);

		tf = new TextField("Pøipojovat podpis:", setting.sign, 100, TextField.ANY);
		frmSetting.append(tf);

		ChoiceGroup cg = new ChoiceGroup("Upøednostòovat adresáø aplikace", ChoiceGroup.MULTIPLE);
		cg.append("Ano", null);
		if (setting.useInternalPhonebook) cg.setSelectedIndex(0, true);
		frmSetting.append(cg);

		cg = new ChoiceGroup("Potvrzovat pøed odesláním", ChoiceGroup.MULTIPLE);
		cg.append("Ano", null);
		if (setting.confirmReceiver) cg.setSelectedIndex(0, true);
		frmSetting.append(cg);

		cg = new ChoiceGroup("Animovat smajlíka", ChoiceGroup.MULTIPLE);
		cg.append("Ano", null);
		if (setting.animateSmile) cg.setSelectedIndex(0, true);
		frmSetting.append(cg);

		cg = new ChoiceGroup("Povolit zvuky", ChoiceGroup.MULTIPLE);
		cg.append("Ano", null);
		if (setting.allowSounds) cg.setSelectedIndex(0, true);
		frmSetting.append(cg);

		cg = new ChoiceGroup("Odstraòovat diakritiku", ChoiceGroup.MULTIPLE);
		cg.append("Ano", null);
		if (setting.clearDiacritics) cg.setSelectedIndex(0, true);
		frmSetting.append(cg);

		cg = new ChoiceGroup("Expertní mode", ChoiceGroup.MULTIPLE);
		cg.append("Ano", null);
		if (setting.expertMode) cg.setSelectedIndex(0, true);
		frmSetting.append(cg);

		cg = new ChoiceGroup("Poèitadlo znakù", ChoiceGroup.MULTIPLE);
		cg.append("Ano", null);
		if (setting.showCounter) cg.setSelectedIndex(0, true);
		frmSetting.append(cg);
                
                cg = new ChoiceGroup("Podržet GPRS spojení", ChoiceGroup.MULTIPLE);
		cg.append("Ano", null);
		if (setting.holdGPRS) cg.setSelectedIndex(0, true);
		frmSetting.append(cg);

		cg = new ChoiceGroup("Velká písmena u vìt", ChoiceGroup.MULTIPLE);
		cg.append("Ano", null);
		if (setting.capitalizeSentences) cg.setSelectedIndex(0, true);
		frmSetting.append(cg);

		cg = new ChoiceGroup("Ukonèit po úspìšném odeslání", ChoiceGroup.MULTIPLE);
		cg.append("Ano", null);
		if (setting.quitOnSuccessfullSend) cg.setSelectedIndex(0, true);
		frmSetting.append(cg);

		tf = new TextField("Zpráv na stránku pøi ètení", Global.itoa(setting.msgsReadPage), 2, TextField.NUMERIC);
		frmSetting.append(tf);

		tf = new TextField("Váš email:", String.valueOf(setting.email), 50, TextField.EMAILADDR);
		frmSetting.append(tf);

		frmSetting.addCommand(CMD_BACK);
		frmSetting.addCommand(CMD_OK);
		frmSetting.addCommand(CMD_ADVSETTING);
		frmSetting.addCommand(CMD_SETTINGWARNING);
		frmSetting.setCommandListener(this);

		display.setCurrent(frmSetting);
	}

	protected void showStatus(String title, boolean ok, String statusText, boolean showSmiley, boolean allowEnd, boolean allowRetry, boolean allowNormalSMS, Command scndButton) {
		frmStatus = new Form(title);
		frmStatus.addCommand(scndButton);
		if (allowRetry && !ok) frmStatus.addCommand(CMD_RETRY);
		if (allowNormalSMS && !ok) {
			frmStatus.addCommand(CMD_RETRYSENDPAIDSMS);
			frmStatus.addCommand(CMD_RETRYSENDNORMALSMS);
		}
		if (allowEnd) frmStatus.addCommand(CMD_EXIT);
		Image img = null;
		final Image imgSmile[] = new Image[4];
		try {
			if (!ok) img = Image.createImage("/pics/error.png");
			else if (showSmiley) {
				imgSmile[0] = Image.createImage("/pics/smile0.png");
				if (setting.animateSmile) {
					imgSmile[1] = Image.createImage("/pics/smile1.png");
					imgSmile[2] = Image.createImage("/pics/smile2.png");
					imgSmile[3] = Image.createImage("/pics/smile3.png");
				}
				img = imgSmile[0];
			}
		}catch (Exception e) {
			//frmStatus.append(e.toString());
			// nothing
		}
		StringItem si = new StringItem("", ok ? statusText : "Chyba: " + statusText);
		frmStatus.append(si);
		ImageItem imgItm = null;
		if (img != null) {
			imgItm = new ImageItem("", img, ImageItem.LAYOUT_NEWLINE_BEFORE | ImageItem.LAYOUT_CENTER, "");
			frmStatus.append(imgItm);
		}
		frmStatus.setCommandListener(this);

		display.setCurrent(frmStatus);

		if (ok && imgItm != null && setting.animateSmile) {
			class CTimerSmileAnim extends TimerTask {
				int picPos = 0;
				ImageItem imgItm = null;

				public CTimerSmileAnim(ImageItem _imgItm) {
					imgItm = _imgItm;
				}

				public void run() {
					if (picPos > 48) picPos = 0;
					// <0, 10> - 0, <11> - 1, <12, 25> - 0, <26> - 2, <27, 47> - 3, <48> - 2
					if (picPos == 0 || picPos == 11 || picPos == 12 || picPos == 26 || picPos == 27 || picPos == 48) {
						try {
							int i = picPos == 11 ? 1 : picPos == 26 ? 2 : picPos == 27 ? 3 : picPos == 48 ? 2 : 0;
							imgItm.setImage(imgSmile[i]);
						}catch (Exception e) {
							// nothing
						}
					}
					picPos++;
				}
			}
			Timer t = new Timer();
			t.schedule(new CTimerSmileAnim(imgItm), 0, 150);
		}
		if (setting.allowSounds && (showSmiley || !ok)) {
			class CTimerPlaySound extends TimerTask {
				boolean success = false;

				public CTimerPlaySound(boolean _success) {
					success = _success;
				}

				public void run() {
					MIDP2.playStatusSound(success);
				}
			}
			Timer t = new Timer();
			t.schedule(new CTimerPlaySound(ok), 2000);
		}

		if (setting.quitOnSuccessfullSend && ok && showSmiley) {
			class CTimerQuitCountdown extends TimerTask {
				int timeToQuit;

				public CTimerQuitCountdown(int _timeToQuit) {
					timeToQuit = _timeToQuit;
				}

				public void run() {
					timeToQuit--;

					frmStatus.setTitle("Zavøít za " + timeToQuit + " s");

					if (timeToQuit == 0) {
						if (isShown(frmStatus)) {
							commandAction(CMD_EXIT, null);
						}else {
							this.cancel();
						}
					}
				}
			}
			Timer t = new Timer();
			t.schedule(new CTimerQuitCountdown(60), 0, 1000);
		}
	}

	protected void showPhonebook(String receiver, boolean inMainMenu) {
		pbInMainMenu = inMainMenu;
		lstPhonebook = new List("Adresáø", List.IMPLICIT);
		if (!pbInMainMenu) {
			lstPhonebook.addCommand(CMD_SEND);
			lstPhonebook.addCommand(CMD_MANUALINSERT);
		}else {
			lstPhonebook.addCommand(CMD_INSERTNUMBER);
			lstPhonebook.addCommand(CMD_INSERTEMAIL);
		}
		lstPhonebook.addCommand(CMD_ADD);
		lstPhonebook.addCommand(CMD_EDIT);
		lstPhonebook.addCommand(CMD_DELETE);
		lstPhonebook.addCommand(CMD_DELETEALL);
		lstPhonebook.addCommand(CMD_EXPORT);
		lstPhonebook.addCommand(CMD_IMPORT);
		lstPhonebook.addCommand(CMD_BACK);
		lstPhonebook.setCommandListener(this);

		Image img = null;
		try {
			img = Image.createImage("/pics/user.png");
		}catch (Exception e) {
			// nothing
		}

		for (int i = 0; i < phonebook.GetCount(); i++) {
			lstPhonebook.append(phonebook.Get(i).name, img);
			if (receiver.compareTo(sendWay != SENDWAY.EMAIL ? phonebook.Get(i).number : phonebook.Get(i).email) == 0)
				lstPhonebook.setSelectedIndex(i, true);
		}

		display.setCurrent(lstPhonebook);
	}

	protected void showPhonebookItem(String name, String number, String email) {
		bAddAction = name.length() == 0;
		frmPhonebookItem = new Form(bAddAction ? "Nový záznam" : name);
		frmPhonebookItem.append(new TextField("Jméno:", name, 50, TextField.ANY));
		frmPhonebookItem.append(new TextField("Èíslo:", number, 13, TextField.PHONENUMBER));
		frmPhonebookItem.append(new TextField("Email:", email, 50, TextField.EMAILADDR));
		frmPhonebookItem.setCommandListener(this);
		frmPhonebookItem.addCommand(CMD_OK);
		frmPhonebookItem.addCommand(CMD_BACK);

		display.setCurrent(frmPhonebookItem);
	}

	protected void showConfirm(String name) {
		frmConfirm = new Form("Vymazat...");
		frmConfirm.append(new StringItem("", !deleteAll ? "Smazat záznam?\r\n'" + name + "'" : "Smazat vše?"));
		frmConfirm.setCommandListener(this);
		frmConfirm.addCommand(CMD_YES);
		frmConfirm.addCommand(CMD_NO);

		display.setCurrent(frmConfirm);
	}

	protected void showAbout() {
		frmAbout = new Form("Gibo SMS...");
		frmAbout.append(new StringItem("Autor:", "Josef Kyrian, MikZ"));
		frmAbout.append(new StringItem("Verze:", _VERSION));
		frmAbout.append(new StringItem("Web stránka:", "http://www.gibosms.cz"));
		frmAbout.append(new StringItem("Diskuzní fórum:", "http://www.gibosms.cz/forum"));
		frmAbout.append(new StringItem("e-mail:", "gibo@gibosms.cz"));
		frmAbout.append(new StringItem("ICQ:", "239553554"));
		frmAbout.setCommandListener(this);
		frmAbout.addCommand(CMD_BACK);

		display.setCurrent(frmAbout);
	}

	protected void showSettingWarning() {
		frmSettingWarning = new Form("Upozornìní");
		frmSettingWarning.append(new StringItem("", "Pokud se aplikace chová podivnì, zkuste vypnout poèitadlo znakù, velká písmena u vìt, zvuky a animování smajlíka. Tyto funkce na nìkterých mobilech zpùsobují nemožnost psaní textu zprávy a vypínání aplikace."));
		frmSettingWarning.setCommandListener(this);
		frmSettingWarning.addCommand(CMD_BACK);

		display.setCurrent(frmSettingWarning);
	}

	protected void showWait(boolean allowCancel) {
		class CWaitCanvas extends Canvas implements CommandListener {
			private Image img = null;
			private int animPos = 0;
			public CWaitCanvas() {
				try {
					img = Image.createImage("/pics/wait.png");
				}catch (Exception e) {
					// nothing
				}
				// display animation
				Timer t = new Timer();
				t.schedule(new CTimerWaitAnim(this), 0, 100);
			}

			public void paint(Graphics g) {
				g.setColor(255, 255, 255);
				g.fillRect(0, 0, g.getClipWidth(), g.getClipHeight());
				int wid = 93, hei = 11;
				int baseX = g.getClipWidth() / 2 - wid / 2, baseY = g.getClipHeight() / 2 - hei / 2;
				g.drawImage(img, baseX, baseY, Graphics.TOP|Graphics.LEFT);
				// nakreslit animaci
				int n = 19;
				wid = 95; baseX -= 1;
				int w = wid / n;
				boolean mono = !display.isColor();
				if (mono) {
					g.setColor(0, 0, 0);
					g.fillRect(baseX  - 1, baseY + 15 + 5 - 1, wid + 2, 5 + 2);
				}
				for (int i = 0; i < n; i++) {
					if (!mono) g.setColor(0, 0, Math.abs(255 - 2 * i * (255 / n)));
					else {
						if (i < 4) g.setColor(0, 0, 0);
						else g.setColor(255, 255, 255);
					}
					g.fillRect(baseX + ((animPos + i) * w) % wid, baseY + 15 + 5, w, 5);
				}
				animPos++;
			}

			public void commandAction(Command c, Displayable d) {
				if (c == CMD_CANCEL) {
					try {
						runningThread = null;
						System.gc();
					}
					catch(Exception e) {
					}
					display.setCurrent(ctrlMessage);
				}
			}

			class CTimerWaitAnim extends TimerTask {
				Canvas can;
				CTimerWaitAnim(Canvas can) {
					this.can = can;
				}

				public void run() {
					can.repaint();
				}
			}
		}

		CWaitCanvas c = new CWaitCanvas();
		cnvsWait = c;
		if (allowCancel) {
			c.addCommand(CMD_CANCEL);
			c.setCommandListener(c);
		}
		display.setCurrent(c);
	}

	protected void showInsertSymbol() {
		class CSymbolCanvas extends Canvas implements CommandListener {
			private final String[] symbols = {
				":-)", ";-)", ":-(",
				":-D", "%-)", ":-/",
				";o)", ":o))", ":p",

				"(-:", ":-X", ":-*",
				":-o", ":,-(", ":-S",
				"8-)", ":-((", ":-Q",

				"LOL", "OK", "pls",
				"hm?", "OK?", "btw",
				"yup", "W8", "thx",
			};
			private int page = 0;

			public void paint(Graphics g) {
				int wid = g.getClipWidth(), hei = g.getClipHeight();
				g.setColor(255, 255, 255);
				g.fillRect(0, 0, wid, hei);
				//g.setColor(200, 200, 200);
				g.setColor(255, 180, 180);
				g.setFont(Font.getFont(Font.FACE_MONOSPACE, Font.STYLE_PLAIN, Font.SIZE_SMALL));
				if (display.isColor()) {
					for (int i = 0; i < 12; i++) {
						int x = ((i % 3) + 1) * (wid / 4), y = (i / 3) * (hei / 5) + (hei / 5) / 2;
						x -= 4; y -= 2;
						if (i / 3 > 2) y += (hei / 5) / 2;
						if (i < 9) g.drawString(String.valueOf(i + 1), x, y, Graphics.HCENTER|Graphics.TOP);
						else if (i == 9) g.drawString("*", x, y, Graphics.HCENTER|Graphics.TOP);
						else if (i == 10) g.drawString("0", x, y, Graphics.HCENTER|Graphics.TOP);
						else if (i == 11) g.drawString("#", x, y, Graphics.HCENTER|Graphics.TOP);
					}
				}
				g.setColor(0);
				g.setFont(Font.getFont(Font.FACE_MONOSPACE, Font.STYLE_PLAIN, Font.SIZE_MEDIUM));
				for (int i = 0; i < 12; i++) {
					int x = ((i % 3) + 1) * (wid / 4), y = (i / 3) * (hei / 5) + (hei / 5) / 2;
					if (i / 3 > 2) y += (hei / 5) / 2;
					if (i < 9) g.drawString(symbols[page * 9 + i], x, y, Graphics.HCENTER|Graphics.TOP);
					else if (i == 9) g.drawString("<", x, y, Graphics.HCENTER|Graphics.TOP);
					else if (i == 10) g.drawString("konec", x, y, Graphics.HCENTER|Graphics.TOP);
					else if (i == 11) g.drawString(">", x, y, Graphics.HCENTER|Graphics.TOP);
				}
			}

			protected void keyPressed(int keyCode) {
				if (keyCode < 0) keyCode = -keyCode;
				if (keyCode == Canvas.KEY_STAR || keyCode == Canvas.LEFT || keyCode == Canvas.UP || keyCode == 59 || keyCode == 61) {
					if (page  > 0) page--;
					else page = symbols.length / 9 - 1;
					repaint();
				}else if (keyCode == Canvas.KEY_POUND || keyCode == Canvas.RIGHT || keyCode == Canvas.DOWN || keyCode == 60 || keyCode == 62) {
					if (page + 1 < symbols.length / 9) page++;
					else page = 0;
					repaint();
				}else if (keyCode >= Canvas.KEY_NUM1 && keyCode <= Canvas.KEY_NUM9) {
					//ctrlMessage.insert(symbols[page * 9 + keyCode - Canvas.KEY_NUM1], ctrlMessage.getCaretPosition());
					ctrlMessage.insert(symbols[page * 9 + keyCode - Canvas.KEY_NUM1], caretPos);
					display.setCurrent(ctrlMessage);
				}else display.setCurrent(ctrlMessage);
			}

			public void commandAction(Command c, Displayable d) {
				if (c == CMD_BACK) {
					display.setCurrent(ctrlMessage);
				}
			}
		}

		CSymbolCanvas c = new CSymbolCanvas();
		c.addCommand(CMD_BACK);
		c.setCommandListener(c);
		display.setCurrent(c);
	}

	protected void showConfirmSend(String name) {
		frmConfirmSend = new Form("Poslat...");
		frmConfirmSend.append(new StringItem("", "Poslat na?\r\n" + name + ""));
		frmConfirmSend.setCommandListener(this);
		frmConfirmSend.addCommand(CMD_YES);
		frmConfirmSend.addCommand(CMD_NO);

		display.setCurrent(frmConfirmSend);
	}

	protected void showInputReceiver(String receiver) {
		this.receiver = receiver;
		frmNumber = new Form("Zadejte pøíjemce");
		if (sendWay != SENDWAY.EMAIL) ctrlNumber = new TextField("Poslat na:", receiver, 13, TextField.PHONENUMBER);
		else ctrlNumber = new TextField("Poslat na:", receiver, 50, TextField.EMAILADDR);
		frmNumber.append(ctrlNumber);
		//frmNumber.append(new StringItem("", "???"));
		frmNumber.addCommand(CMD_SEND);
		frmNumber.addCommand(CMD_PHONEBOOK);
		frmNumber.addCommand(CMD_BACK);
		frmNumber.setCommandListener(this);

		display.setCurrent(frmNumber);
	}

	protected void showInputSubject() {
		frmSubject = new Form("Pøedmìt E-mailu...");
		frmSubject.append(new TextField("Pøedmìt:", "", 50, TextField.ANY));
		frmSubject.setCommandListener(this);
		frmSubject.addCommand(CMD_OK);
		frmSubject.addCommand(CMD_BACK);

		display.setCurrent(frmSubject);
	}

	protected void showSelectImport(String data) {
		lstImport = new List("Vyberte zálohu...", List.IMPLICIT);
		lstImport.addCommand(CMD_OK);
		lstImport.addCommand(CMD_BACK);
		lstImport.setCommandListener(this);

		Image img = null;
		try {
			img = Image.createImage("/pics/data.png");
		}catch (Exception e) {
			// nothing
		}

		Vector lines = Global.GetLines(data.getBytes(), data.length(), false);
		for (int i = 0; i < lines.size(); i++) {
			lstImport.append((String)lines.elementAt(i), img);
		}

		display.setCurrent(lstImport);
	}

	protected void showConfirmClearNewSMS() {
		frmConfirmClearNewSMS = new Form("Nová SMS...");
		frmConfirmClearNewSMS.append(new StringItem("", "Chcete vymazat pøedchozí zprávu?"));
		frmConfirmClearNewSMS.setCommandListener(this);
		frmConfirmClearNewSMS.addCommand(CMD_YES);
		frmConfirmClearNewSMS.addCommand(CMD_NO);

		display.setCurrent(frmConfirmClearNewSMS);
	}

	protected void showInformation() {
		lstInformation = new List("Informace", List.IMPLICIT);
		lstInformation.addCommand(CMD_INFOSELECT);
		lstInformation.addCommand(CMD_BACK);
		lstInformation.setCommandListener(this);

		Image img = null;
		try {
			img = Image.createImage("/pics/statistika.png");
			lstInformation.append("Statistika", img);
			img = Image.createImage("/pics/aktuality.png");
			lstInformation.append("Vaše aktuality", img);
			img = Image.createImage("/pics/aktivace.png");
			lstInformation.append("Aktivace", img);
			img = Image.createImage("/pics/tm.png");
			lstInformation.append("Volné T-Mobile SMS...", img);
			img = Image.createImage("/pics/oprogramu.png");
			lstInformation.append("O programu...", img);
			img = Image.createImage("/pics/novinky.png");
			lstInformation.append("Co je nového?", img);
			img = Image.createImage("/pics/vseobecne.png");
			lstInformation.append("Všeobecné", img);
		}catch (Exception e) {
			// nothing
		}

		display.setCurrent(lstInformation);
	}

	protected void showSelectSendWay() {
		lstSendWays = new List("Vyberte zpùsob odeslání zprávy", List.IMPLICIT);
		lstSendWays.addCommand(CMD_SENDWAYSELECT);
		lstSendWays.addCommand(CMD_BACK);
		lstSendWays.setCommandListener(this);

		try {
			if (!setting.expertMode) {
				lstSendWays.append("SMS zdarma pøes GPRS", null);
				if(!setting.SWdelPaid) lstSendWays.append("SMS za 1 Kè", null);
				if(!setting.SWdelEmail)lstSendWays.append("Email", null);
				if(!setting.SWdelGibo)lstSendWays.append("Gibomeska", null);
				if(!setting.SWdelNorm)lstSendWays.append("Normální SMS", null);
			}else {
				lstSendWays.append("Web SMS zdarma", null);
				if(!setting.SWdelPaid)lstSendWays.append("SMS za 1 Kè", null);
				if(!setting.SWdelMemail)lstSendWays.append("Mobilní email", null);
				if(!setting.SWdelEmail)lstSendWays.append("Email", null);
				if(!setting.SWdelGibo)lstSendWays.append("Gibomeska", null);
				if(!setting.SWdelNorm)lstSendWays.append("Normální SMS", null);
			}
		}catch (Exception e) {
			// nothing
		}
		lstSendWays.setSelectedIndex(setting.lastSendWay, true);

		display.setCurrent(lstSendWays);
	}

	protected void showSelectMessagesReading() {
		lstMsgsReadType = new List("Vyberte jaké zprávy chcete zobrazit", List.IMPLICIT);
		lstMsgsReadType.addCommand(CMD_MSGSREADTYPESELECT);
		lstMsgsReadType.addCommand(CMD_BACK);
		lstMsgsReadType.setCommandListener(this);

		try {
			lstMsgsReadType.append("Moje gibomesky", null);
			lstMsgsReadType.append("Odeslané", null);
			lstMsgsReadType.append("Pøijaté", null);
		}catch (Exception e) {
			// nothing
		}

		display.setCurrent(lstMsgsReadType);
	}

	protected void showInputPassword() {
		frmPassword = new Form("Zadejte heslo");
		if (setting.msgsReadRememberPassword) frmPassword.append(new TextField("Heslo:", setting.msgsReadPassword, 50, TextField.PASSWORD));
		else frmPassword.append(new TextField("Heslo:", setting.msgsReadPassword, 50, TextField.ANY));
		ChoiceGroup cg = new ChoiceGroup("Pamatovat heslo", ChoiceGroup.MULTIPLE);
		cg.append("Ano", null);
		if (setting.msgsReadRememberPassword) cg.setSelectedIndex(0, true);
		frmPassword.append(cg);
		frmPassword.setCommandListener(this);
		frmPassword.addCommand(CMD_OK);
		frmPassword.addCommand(CMD_BACK);

		display.setCurrent(frmPassword);
	}

	protected void showInputJumpOver() {
		if (frmJumpOver == null) {
			frmJumpOver = new Form("Pøesunout se o...");
			frmJumpOver.append(new TextField("Pøesunout se o:", "1", 4, TextField.NUMERIC));
			frmJumpOver.append(new TextField("Zpráv na stránku:", Global.itoa(msgsReadPage), 2, TextField.NUMERIC));
			frmJumpOver.addCommand(CMD_JUMPFORWARD);
			frmJumpOver.addCommand(CMD_JUMPBACKWARD);
			frmJumpOver.addCommand(CMD_BACK);
			frmJumpOver.setCommandListener(this);
		}

		display.setCurrent(frmJumpOver);
	}

	protected void showSearch() {
		if (frmSearch == null) {
			frmSearch = new Form("Vyhledat zprávy...");
			frmSearch.append(new TextField((msgsReadType == "sent" ? "Pøíjemce" : "Odesilatel") + " (jméno nebo telefon):", "", 50, TextField.ANY));
			frmSearch.append(new TextField("Text:", "", 50, TextField.ANY));
			frmSearch.append(new DateField("Datum od:", DateField.DATE));
			frmSearch.append(new DateField("Datum do:", DateField.DATE));
			frmSearch.addCommand(CMD_OK);
			frmSearch.addCommand(CMD_BACK);
			frmSearch.setCommandListener(this);
		}

		display.setCurrent(frmSearch);
	}

	protected void showMsgsRead(boolean ok, String text, String extras) {
		Vector varStore = Global.LoadVariablesFromString(extras, false);
		msgsReadCorrectPasswordSubmitted = Global.LoadIntFromVariablesStore(varStore, "correct_password", 0) != 0;
		msgsReadMsg_0_start = Global.LoadIntFromVariablesStore(varStore, "msg_0_start", 0);
		msgsReadMsg_0_length = Global.LoadIntFromVariablesStore(varStore, "msg_0_length", 0);

		frmMsgsRead = new Form(msgsReadType == "sent" ? "Odeslané SMS" : (msgsReadType == "received" ? "Pøijaté SMS" : "Moje gibomesky"));
		if (Global.LoadIntFromVariablesStore(varStore, "end_eof", 1) == 0) frmMsgsRead.addCommand(CMD_NEXTPAGE);
		if (Global.LoadIntFromVariablesStore(varStore, "start_eof", 1) == 0) frmMsgsRead.addCommand(CMD_PREVIOUSPAGE);
		if (Global.LoadIntFromVariablesStore(varStore, "allow_jump", 1) != 0) frmMsgsRead.addCommand(CMD_JUMPOVER);
		if (Global.LoadIntFromVariablesStore(varStore, "allow_search", 1) != 0) frmMsgsRead.addCommand(CMD_SEARCH);
		if (Global.LoadIntFromVariablesStore(varStore, "allow_edit_first", 1) != 0) frmMsgsRead.addCommand(CMD_COPYTOEDITFIRST);
		if (Global.LoadIntFromVariablesStore(varStore, "allow_edit_all", 1) != 0) frmMsgsRead.addCommand(CMD_COPYTOEDITALL);
		if (Global.LoadIntFromVariablesStore(varStore, "allow_refresh", 1) != 0) frmMsgsRead.addCommand(CMD_REFRESH);
		frmMsgsRead.addCommand(CMD_BACK);
		frmMsgsRead.setCommandListener(this);

		Image img = null;
		try {
			if (!ok) img = Image.createImage("/pics/error.png");
		}catch (Exception e) {
			// nothing
		}
		frmMsgsRead.append(new StringItem("", ok ? text : "Chyba: " + text));
		ImageItem imgItm = null;
		if (img != null) {
			imgItm = new ImageItem("", img, ImageItem.LAYOUT_NEWLINE_BEFORE | ImageItem.LAYOUT_CENTER, "");
			frmMsgsRead.append(imgItm);
		}

		display.setCurrent(frmMsgsRead);
	}

	protected void showInputPictogram(String data, int width, int height, boolean onlyNumbers) {
		frmInputPictogram = new Form("Piktogram");
		frmInputPictogram.append(new TextField("Opište kód:", "", 50, onlyNumbers ? TextField.NUMERIC : TextField.ANY));
		Image img = Global.createImageFromPictogram(data, width, height);
		Image imgImutable = Image.createImage(img);
		ImageItem imgItem = new ImageItem("", imgImutable, ImageItem.LAYOUT_NEWLINE_BEFORE | ImageItem.LAYOUT_CENTER, "");
		frmInputPictogram.append(imgItem);
		frmInputPictogram.setCommandListener(this);
		frmInputPictogram.addCommand(CMD_BACK);
		frmInputPictogram.addCommand(CMD_OK);

		display.setCurrent(frmInputPictogram);
	}

	protected void showSelectTMAccount(Vector accounts, String selected_account) {
		lstTMAccounts = new List("Vyberte T-Zones úèet", List.IMPLICIT);
		lstTMAccounts.addCommand(CMD_TMACCOUNTSELECT);
		lstTMAccounts.addCommand(CMD_BACK);
		lstTMAccounts.setCommandListener(this);

		int selectedAccount = 0;
		try {
			for (int i = 0; i < accounts.size(); i++) {
				String account = (String)accounts.elementAt(i);
				lstTMAccounts.append(account, null);
				if (account.compareTo(selected_account) == 0) {
					selectedAccount = i;
				}
			}
		}catch (Exception e) {
			// nothing
		}
		lstTMAccounts.setSelectedIndex(selectedAccount, true);

		display.setCurrent(lstTMAccounts);
	}

	public void sendSMS(int sendWay) {
		showWait(true);

		lastMessage = ctrlMessage.getString();
		if (setting.sign.length() > 0) lastMessage += " " + setting.sign;
		if (setting.clearDiacritics) lastMessage = Global.ClearDiacritics(lastMessage);

		CSendMSG p = new CSendMSG(receiver, setting.srcNum, !send_retry || message_id == "" || sendWay == SENDWAY.NORMALSMS ? lastMessage : "", subject, sendWay, message_id, pictogram, selected_tm_account, send_retry);
		runningThread = new Thread(p);
		runningThread.start();

		if (!writeSetting) writeSetting = sendWay != SENDWAY.EMAIL ? (setting.lastDestNum.compareTo(receiver) != 0) : (setting.lastDestEmail.compareTo(receiver) != 0);
		if (sendWay != SENDWAY.EMAIL) setting.lastDestNum = receiver;
		else setting.lastDestEmail = receiver;
	}

	public void refreshMsgsRead() {
		showWait(true);
		runningThread = new Thread(new CShowMsgsRead(msgsReadPassword));
		runningThread.start();
	}

	public void commandAction(Command c, Displayable d) {
		if (c == CMD_EXIT) {
			saveText = isShown(ctrlMessage);
			showWait(false);
			new Thread(new CExit()).start();
		}else if (c == CMD_RETRY) {
			send_retry = true;
			pictogram = "";

			sendSMS(sendWay);
		}else if (c == CMD_RETRYSENDPAIDSMS) {
			send_retry = false;
			message_id = "";
			pictogram = "";
			selected_tm_account = "";
			sendSMS(SENDWAY.PAIDSMS);
		}else if (c == CMD_RETRYSENDNORMALSMS) {
			send_retry = false;
			message_id = "";
			pictogram = "";
			selected_tm_account = "";
			sendSMS(SENDWAY.NORMALSMS);
		}else if (c == CMD_BACK) {
			if (isShown(frmPhonebookItem)) {
				display.setCurrent(lstPhonebook);
			}else if (isShown(lstPhonebook)) {
				display.setCurrent(pbBackDisplay);
			}else if (isShown(frmNumber)) {
				if (ctrlNumber.size() > 0) {
					ctrlNumber.setString("");
				}else {
					display.setCurrent(ctrlMessage);
				}
			}else if (isShown(lstImport)) {
				display.setCurrent(lstPhonebook);
			}else if (isShown(frmStatus)) {
				display.setCurrent(backDisplay);
			}else if (isShown(frmAbout)) {
				display.setCurrent(backDisplay);
			}else if (isShown(frmSubject)) {
				display.setCurrent(lstSendWays);
			}else if (isShown(lstSendWays)) {
				display.setCurrent(ctrlMessage);
			}else if (isShown(frmMsgsRead)) {
				display.setCurrent(lstMsgsReadType);
			}else if (isShown(frmSettingWarning)) {
				display.setCurrent(frmSetting);
			}else if (isShown(frmAdvSetting)) {
				display.setCurrent(frmSetting);
			}else if (isShown(frmPassword) || isShown(frmMsgsRead)) {
				ChoiceGroup cg = (ChoiceGroup)frmPassword.get(1);
				boolean[] sa = new boolean[1];
				setting.msgsReadRememberPassword = cg.getSelectedFlags(sa) != 0;
				setting.msgsReadPassword = setting.msgsReadRememberPassword ? msgsReadPassword : "";
				writeSetting = true;
				display.setCurrent(lstMsgsReadType);
			}else {
				display.setCurrent(ctrlMessage);
			}
		}else if (c == CMD_NEWSMS || c == CMD_NEWSMS_BACK) {
			showConfirmClearNewSMS();
		}else if (c == CMD_SEND) {
			if (isShown(frmNumber) || isShown(lstPhonebook)) {
				receiver = isShown(frmNumber) ? ctrlNumber.getString() : "";
				String receiverName = receiver;
				if (isShown(lstPhonebook)) {
					int selIndex = lstPhonebook.getSelectedIndex();
					if (selIndex != -1 ) {
						receiver = sendWay != SENDWAY.EMAIL ? phonebook.Get(selIndex).number : phonebook.Get(selIndex).email;
						receiverName = phonebook.Get(selIndex).name;
					}
				}

				if (receiver.length() > 0) {
					if (!setting.confirmReceiver) {
						send_retry = false;
						message_id = "";
						pictogram = "";
						selected_tm_account = "";
						sendSMS(sendWay);
					}else {
						prevDisplay = display.getCurrent();
						showConfirmSend(receiverName);
					}
				}else {
					showErr(sendWay != SENDWAY.EMAIL ? "Èíslo pøíjemce není definováno" : "Email pøíjemce není definován");
				}
			}else if (isShown(ctrlMessage)) {
				if (setting.srcNum.length() < 9) {
					showSetting();
				}else {
					showSelectSendWay();
				}
			}
		}else if (c == CMD_OK) {
			if (isShown(frmSetting)) {
				TextField tf = (TextField)frmSetting.get(0);
				if (tf.getString().length() < 9) {
					showErr("Musíte zadat svoje telefonní èíslo");
				}else {
					//showWait(false);

					setting.srcNum = tf.getString();

					setting.pwd = ((TextField)frmSetting.get(1)).getString();

					setting.sign = ((TextField)frmSetting.get(2)).getString();

					ChoiceGroup cg = (ChoiceGroup)frmSetting.get(3);
					boolean[] sa = new boolean[1];
					setting.useInternalPhonebook = cg.getSelectedFlags(sa) != 0;

					cg = (ChoiceGroup)frmSetting.get(4);
					setting.confirmReceiver = cg.getSelectedFlags(sa) != 0;

					cg = (ChoiceGroup)frmSetting.get(5);
					setting.animateSmile = cg.getSelectedFlags(sa) != 0;

					cg = (ChoiceGroup)frmSetting.get(6);
					setting.allowSounds = cg.getSelectedFlags(sa) != 0;

					cg = (ChoiceGroup)frmSetting.get(7);
					setting.clearDiacritics = cg.getSelectedFlags(sa) != 0;

					cg = (ChoiceGroup)frmSetting.get(8);
					setting.expertMode = cg.getSelectedFlags(sa) != 0;

					cg = (ChoiceGroup)frmSetting.get(9);
					setting.showCounter = cg.getSelectedFlags(sa) != 0;
                                           
                                        cg = (ChoiceGroup)frmSetting.get(10);
					setting.holdGPRS = cg.getSelectedFlags(sa) != 0;

					cg = (ChoiceGroup)frmSetting.get(11);
					setting.capitalizeSentences = cg.getSelectedFlags(sa) != 0;

					cg = (ChoiceGroup)frmSetting.get(12);
					setting.quitOnSuccessfullSend = cg.getSelectedFlags(sa) != 0;

					setting.msgsReadPage = Math.max(Global.atoi(((TextField)frmSetting.get(13)).getString()), 1);

					setting.email = ((TextField)frmSetting.get(14)).getString();
                                        
					if (setting.showCounter) {
						if (tCounter == null) {
							tCounter = new Timer();
							tCounter.schedule(new CTimer(), 1000, 1000);
						}
					}else if (tCounter != null) {
						tCounter.cancel();
						tCounter = null;
					}

					initCtrlMessage(ctrlMessage.getString(), undoText != null && undoText.length() > 0, true);

					setting.Write();

					writeSetting = false;}
			}else if (isShown(frmAdvSetting)) {
	
					boolean[] sa = new boolean[1];
					ChoiceGroup cg = (ChoiceGroup)frmAdvSetting.get(0);
					setting.SWdelEmail = cg.getSelectedFlags(sa) != 0;

					cg = (ChoiceGroup)frmAdvSetting.get(1);
					setting.SWdelGibo = cg.getSelectedFlags(sa) != 0;

					cg = (ChoiceGroup)frmAdvSetting.get(2);
					setting.SWdelNorm = cg.getSelectedFlags(sa) != 0;

					cg = (ChoiceGroup)frmAdvSetting.get(3);
					setting.SWdelPaid = cg.getSelectedFlags(sa) != 0;
					
					cg = (ChoiceGroup)frmAdvSetting.get(4);
					setting.SWdelMemail = cg.getSelectedFlags(sa) != 0;
					
					setting.title = ((TextField)frmAdvSetting.get(5)).getString();
				
					display.setCurrent(frmSetting);
					setting.Write();
					writeSetting = false;
			}else if (isShown(frmPhonebookItem)) {
				String name = ((TextField)frmPhonebookItem.get(0)).getString();
				String number = ((TextField)frmPhonebookItem.get(1)).getString();
				String email = ((TextField)frmPhonebookItem.get(2)).getString();
				if (name.length() > 0) {
					if (bAddAction) {
						phonebook.Add(name, number, email);
						lstPhonebook.append(name, null);
					}else {
						int index = lstPhonebook.getSelectedIndex();
						phonebook.Change(index, name, number, email);
						lstPhonebook.set(index, name, null);
					}
					phonebook.Sort();
					writePhonebook = true;
					showPhonebook(sendWay != SENDWAY.EMAIL ? number : email, pbInMainMenu);
				}
			}else if (isShown(frmSubject)) {
				subject = ((TextField)frmSubject.get(0)).getString();
				if (setting.useInternalPhonebook) {
					pbBackDisplay = display.getCurrent();
					showPhonebook(setting.lastDestEmail, false);
				}else {
					showInputReceiver("");
				}
			}else if (isShown(lstImport)) {
				if (lstImport.getSelectedIndex() != -1) {
					prevDisplay = display.getCurrent();
					showWait(true);
					runningThread = new Thread(new CPhonebookImportReadData(lstImport.getSelectedIndex()));
					runningThread.start();
				}
			}else if (isShown(frmPassword)) {
				// nacist zadane heslo
				msgsReadPassword = ((TextField)frmPassword.get(0)).getString();
				ChoiceGroup cg = (ChoiceGroup)frmPassword.get(1);
				boolean[] sa = new boolean[1];
				setting.msgsReadRememberPassword = cg.getSelectedFlags(sa) != 0;
				setting.msgsReadPassword = setting.msgsReadRememberPassword ? msgsReadPassword : "";
				writeSetting = true;
				refreshMsgsRead();
			}else if (isShown(frmSearch)) {
				msgsReadCont = ((TextField)frmSearch.get(0)).getString();
				msgsReadSearch_text = ((TextField)frmSearch.get(1)).getString();
				Date dateFrom = ((DateField)frmSearch.get(2)).getDate();
				if (dateFrom != null) msgsReadStartTime = dateFrom.getTime() / 1000;
				else msgsReadStartTime = 0;
				System.out.println(msgsReadStartTime);
				Date dateTo = ((DateField)frmSearch.get(3)).getDate();
				if (dateTo != null) msgsReadEndTime = dateTo.getTime() / 1000;
				else msgsReadEndTime = 0;
				msgsReadPos = 0;
				refreshMsgsRead();
			}else if (isShown(frmInputPictogram)) {
				send_retry = true;
				pictogram = ((TextField)frmInputPictogram.get(0)).getString();
				sendSMS(sendWay);
			}
		}else if (c == CMD_PHONEBOOK) {
			caretPos = ctrlMessage.getCaretPosition();
			pbBackDisplay = display.getCurrent();
			if (isShown(ctrlMessage)) showPhonebook(setting.lastDestNum, true);
			else showPhonebook(ctrlNumber.getString(), false);
		}else if (c == List.SELECT_COMMAND) {
			if (isShown(lstPhonebook)) {
				if (!pbInMainMenu) commandAction(CMD_SEND, null);
				else commandAction(CMD_INSERTNUMBER, null);
			}else if (isShown(lstImport)) {
				commandAction(CMD_OK, null);
			}else if (isShown(lstInformation)) {
				commandAction(CMD_INFOSELECT, null);
			}else if (isShown(lstSendWays)) {
				commandAction(CMD_SENDWAYSELECT, null);
			}else if (isShown(lstMsgsReadType)) {
				commandAction(CMD_MSGSREADTYPESELECT, null);
			}else if (isShown(lstTMAccounts)) {
				commandAction(CMD_TMACCOUNTSELECT, null);
			}
		}else if (c == CMD_MANUALINSERT) {
			showInputReceiver(sendWay != SENDWAY.EMAIL ? setting.lastDestNum : setting.lastDestEmail);
		}else if (c == CMD_ADD) {
			showPhonebookItem("", "", "");
		}else if (c == CMD_EDIT && lstPhonebook.getSelectedIndex() != -1) {
			showPhonebookItem(phonebook.Get(lstPhonebook.getSelectedIndex()).name, phonebook.Get(lstPhonebook.getSelectedIndex()).number, phonebook.Get(lstPhonebook.getSelectedIndex()).email);
		}else if (c == CMD_DELETE && lstPhonebook.getSelectedIndex() != -1) {
			deleteAll = false;
			showConfirm(phonebook.Get(lstPhonebook.getSelectedIndex()).name);
		}else if (c == CMD_DELETEALL) {
			deleteAll = true;
			showConfirm(null);
		}else if (c == CMD_YES) {
			if (isShown(frmConfirm)) {
				int index = lstPhonebook.getSelectedIndex();
				if (!deleteAll) {
					phonebook.Delete(index);
					lstPhonebook.delete(index);
					display.setCurrent(lstPhonebook);
				}else {
					phonebook.DeleteAll();
					lstPhonebook.delete(index);
					showPhonebook("", pbInMainMenu);
				}

				writePhonebook = true;
			}else if (isShown(frmConfirmSend)) {
				send_retry = false;
				message_id = "";
				pictogram = "";
				selected_tm_account = "";
				sendSMS(sendWay);
			}else if (isShown(frmConfirmClearNewSMS)) {
				ctrlMessage.setString("");
				display.setCurrent(ctrlMessage);
			}
		}else if (c == CMD_NO) {
			if (isShown(frmConfirm)) {
				display.setCurrent(lstPhonebook);
			}else if (isShown(frmConfirmSend)) {
				display.setCurrent(prevDisplay);
			}else if (isShown(frmConfirmClearNewSMS)) {
				display.setCurrent(ctrlMessage);
			}
		}else if (c == CMD_INSERTSYMBOL) {
			caretPos = ctrlMessage.getCaretPosition();
			showInsertSymbol();
		}else if (c == CMD_CLEARSPACES) {
			undoText = ctrlMessage.getString();
			initCtrlMessage(Global.ClearSpaces(ctrlMessage.getString()), true, true);
		}else if (c == CMD_CLEARDIACRITICS) {
			undoText = ctrlMessage.getString();
			initCtrlMessage(Global.ClearDiacritics(ctrlMessage.getString()), true, true);
		}else if (c == CMD_CLEAR) {
			undoText = ctrlMessage.getString();
			initCtrlMessage("", true, true);
		}else if (c == CMD_UNDO) {
			initCtrlMessage(undoText, false, true);
		}else if (c == CMD_INSERTNUMBER || c == CMD_INSERTEMAIL) {
			if (isShown(lstPhonebook)) {
				int selIndex = lstPhonebook.getSelectedIndex();
				if (selIndex != -1 ) {
					String rec = c == CMD_INSERTNUMBER ? phonebook.Get(selIndex).number : phonebook.Get(selIndex).email;
					if (rec.length() > 0) {
						ctrlMessage.insert(rec, caretPos);
						display.setCurrent(ctrlMessage);
					}else showErr(c == CMD_INSERTNUMBER ? "Èíslo není definováno" : "Email není definován");
				}
			}
		}else if (c == CMD_SETTING) {
			showSetting();
		}else if (c == CMD_SETTINGWARNING) {
			showSettingWarning();
		}else if (c == CMD_INFORMATION) {
			showInformation();
		}else if (c == CMD_INFOSELECT) {
			int selIndex = lstInformation.getSelectedIndex();
			if (selIndex != -1 ) {
				String[] pages = {"statistika.php", "aktuality.php", "info_aktivace.php", "t-mobile.php", null, "whatsnew.php", "info_general.php"};
				String[] params = {"num=" + setting.srcNum + "&pwd=" + Global.URLEncode(setting.pwd) + "&client=" + _CLIENT + "&ver=" + _VERSION, "num=" + setting.srcNum + "&pwd=" + Global.URLEncode(setting.pwd) + "&client=" + _CLIENT + "&ver=" + _VERSION, "ver=" + _VERSION + "&client=" + _CLIENT, "num=" + setting.srcNum + "&pwd=" + Global.URLEncode(setting.pwd) + "&action=getfreesms" + "&client=" + _CLIENT + "&ver=" + _VERSION, null, "ver=" + _VERSION + "&client=" + _CLIENT, "ver=" + _VERSION + "&client=" + _CLIENT};
				String[] titles = {"Statistika...", "Vaše aktuality", "Informace o aktivaci", "Volné zprávy z T-Zones na T-Mobile", null, "Co je v nové verzi...", "Všeobecné informace"};
				boolean showPage = true;
				backDisplay = lstInformation;
				if (selIndex == 0) {
					if (setting.srcNum.length() < 9) {
						showPage = false;
						showSetting();
					}
				}else if (selIndex == 4) {
					showPage = false;
					showAbout();
				}
				if (showPage) {
					showWait(true);
					runningThread = new Thread(new CShowPage(giboServer + pages[selIndex], params[selIndex], titles[selIndex]));
					runningThread.start();
				}
			}else showErr("Musíte vybrat nìjakou položku");
		}else if (c == CMD_EXPORT) {
			prevDisplay = display.getCurrent();

			showWait(true);
			runningThread = new Thread(new CPhonebookExport());
			runningThread.start();
		}else if (c == CMD_IMPORT) {
			showWait(true);

			runningThread = new Thread(new CPhonebookImportReadList());
			runningThread.start();
		}else if (c == CMD_SENDWAYSELECT) {
			int selIndex = lstSendWays.getSelectedIndex();
			if (selIndex != -1 ) {
				setting.lastSendWay = selIndex;
				writeSetting = true;

				if (!setting.expertMode) {
					if (selIndex == 0) {
						sendWay = SENDWAY.GPRSMSG;
					}else if (selIndex == 1) {
						sendWay = SENDWAY.PAIDSMS;
					}else if (selIndex == 2) {
						sendWay = SENDWAY.EMAIL;
					}else if (selIndex == 3) {
						sendWay = SENDWAY.MAILBOX;
					}else if (selIndex == 4) {
						sendWay = SENDWAY.NORMALSMS;
					}
				}else {
					if (selIndex == 0) {
						sendWay = SENDWAY.WEBSMS;
					}else if (selIndex == 1) {
						sendWay = SENDWAY.PAIDSMS;
					}else if (selIndex == 2) {
						sendWay = SENDWAY.MOBEMAIL;
					}else if (selIndex == 3) {
						sendWay = SENDWAY.EMAIL;
					}else if (selIndex == 4) {
						sendWay = SENDWAY.MAILBOX;
					}else if (selIndex == 5) {
						sendWay = SENDWAY.NORMALSMS;
					}
				}

				if (sendWay != SENDWAY.EMAIL) {
					if (setting.useInternalPhonebook) {
						pbBackDisplay = display.getCurrent();
						showPhonebook(setting.lastDestNum, false);
					}else {
						showInputReceiver("");
					}
				}else showInputSubject();
			}else showErr("Musíte vybrat nìjakou položku");
		}else if (c == CMD_MESSAGESREADING) {
			showSelectMessagesReading();
		}else if (c == CMD_MSGSREADTYPESELECT) {
			int selIndex = lstMsgsReadType.getSelectedIndex();
			if (selIndex != -1 ) {
				if (selIndex == 0) msgsReadType = "mailbox";
				else if (selIndex == 1) msgsReadType = "sent";
				else if (selIndex == 2) msgsReadType = "received";

				// vynuloval cteci pozici
				msgsReadPos = 0;
				msgsReadPage = setting.msgsReadPage;
				msgsReadCont = "";
				msgsReadSearch_text = "";
				msgsReadStartTime = msgsReadEndTime = 0;

				if (msgsReadCorrectPasswordSubmitted) refreshMsgsRead();
				else showInputPassword();
			}
		}else if (c == CMD_NEXTPAGE) {
			msgsReadPos += msgsReadPage;
			refreshMsgsRead();
		}else if (c == CMD_PREVIOUSPAGE) {
			msgsReadPos = Math.max(msgsReadPos - msgsReadPage, 0);
			refreshMsgsRead();
		}else if (c == CMD_REFRESH) {
			refreshMsgsRead();
		}else if (c == CMD_COPYTOEDITFIRST || c == CMD_COPYTOEDITALL) {
			String text = ((StringItem)frmMsgsRead.get(0)).getText();
			if (c == CMD_COPYTOEDITFIRST && msgsReadMsg_0_length > 0) {
				text = text.substring(msgsReadMsg_0_start, msgsReadMsg_0_start + msgsReadMsg_0_length);
			}
			ctrlMessage.insert(text, caretPos);
			display.setCurrent(ctrlMessage);
		}else if (c == CMD_JUMPOVER) {
			showInputJumpOver();
		}else if (c == CMD_SEARCH) {
			showSearch();
		}else if (c == CMD_JUMPFORWARD || c == CMD_JUMPBACKWARD) {
			int n = Global.atoi(((TextField)frmJumpOver.get(0)).getString());
			if (c == CMD_JUMPBACKWARD) n = -n;
			msgsReadPos = Math.max(msgsReadPos + n, 0);
			msgsReadPage = Global.atoi(((TextField)frmJumpOver.get(1)).getString());
			msgsReadPage = Math.max(msgsReadPage, 1);
			refreshMsgsRead();
		}else if (c == CMD_TMACCOUNTSELECT) {
			int selIndex = lstTMAccounts.getSelectedIndex();
			if (selIndex != -1 ) {
				selected_tm_account = lstTMAccounts.getString(selIndex);
				send_retry = true;
	
				sendSMS(sendWay);
			}
		}else if (c == CMD_MINIMIZE) {
                    display.setCurrent(mini);
                }
		else if (c == CMD_ADVSETTING) {
		    showAdvSetting();
                }
                
	}
}
