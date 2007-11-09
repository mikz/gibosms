import java.util.Vector;

public class Setting {
	protected final String settingFile = "setting2";
	public String srcNum = "";
	public String pwd = "";
	public String sign = "";
	public boolean useInternalPhonebook = true;
	public boolean confirmReceiver = false;
	public boolean animateSmile = false;
	public boolean allowSounds = false;
	public String email = "";
	public int msgsReadPage = 3;
	public String msgsReadPassword = "";
	public boolean msgsReadRememberPassword = false;
	public boolean expertMode = false;
	public boolean clearDiacritics = true;
	public boolean capitalizeSentences = false;
	public boolean showCounter = false;
	public boolean quitOnSuccessfullSend = true;

	public String lastDestNum = "";
	public String lastDestEmail = "";
	public int lastSendWay;
	public int nSentMessages = 0;
	public String msg = "";

        // edit
        public boolean holdGPRS = false;
        public String title = "%Np%:%Nz% %Vc%%:Vz%|%Oc%%:Oz%|%Tc%%:Tz%";
	public boolean SWdelEmail = false;
	public boolean SWdelGibo = false;
	public boolean SWdelNorm = false;
	public boolean SWdelPaid = false;
	public boolean SWdelMemail = false;
	public int titleVarK = 10;
        public int num_tm_accounts = 0;
        public String tm_accounts = "";
        public String selected_tm_account = "";
        
        
	public void Load() {
		Vector varStore = Global.LoadVariablesFromFile(settingFile, true);
		if (varStore != null) {
			srcNum = Global.LoadStringFromVariablesStore(varStore, "srcNum", "");
			pwd = Global.LoadStringFromVariablesStore(varStore, "pwd", "");
			sign = Global.LoadStringFromVariablesStore(varStore, "sign", "");
			useInternalPhonebook = Global.LoadIntFromVariablesStore(varStore, "useInternalPhonebook", 1) != 0;
			confirmReceiver = Global.LoadIntFromVariablesStore(varStore, "confirmReceiver", 0) != 0;
			animateSmile = Global.LoadIntFromVariablesStore(varStore, "animateSmile", 0) != 0;
			allowSounds = Global.LoadIntFromVariablesStore(varStore, "allowSounds", 0) != 0;
			email = Global.LoadStringFromVariablesStore(varStore, "email", "");
			msgsReadPage = Global.LoadIntFromVariablesStore(varStore, "msgsReadPage", 3);
			msgsReadPassword = Global.LoadStringFromVariablesStore(varStore, "msgsReadPassword", "");
			msgsReadRememberPassword = Global.LoadIntFromVariablesStore(varStore, "msgsReadRememberPassword", 0) != 0;
			expertMode = Global.LoadIntFromVariablesStore(varStore, "expertMode", 0) != 0;
			clearDiacritics = Global.LoadIntFromVariablesStore(varStore, "clearDiacritics", 1) != 0;
			capitalizeSentences = Global.LoadIntFromVariablesStore(varStore, "capitalizeSentences", 0) != 0;
			showCounter = Global.LoadIntFromVariablesStore(varStore, "showCounter", 0) != 0;
			quitOnSuccessfullSend = Global.LoadIntFromVariablesStore(varStore, "quitOnSuccessfullSend", 0) != 0;

			lastDestNum = Global.LoadStringFromVariablesStore(varStore, "lastDestNum", "");
			lastDestEmail = Global.LoadStringFromVariablesStore(varStore, "lastDestEmail", "");
			nSentMessages = Global.LoadIntFromVariablesStore(varStore, "nSentMessages", 0);
			lastSendWay = Global.LoadIntFromVariablesStore(varStore, "lastSendWay", 0);
			msg = Global.LoadStringFromVariablesStore(varStore, "msg", "");
                        
                        holdGPRS = Global.LoadIntFromVariablesStore(varStore, "holdGPRS", 0) != 0;
                        title = Global.LoadStringFromVariablesStore(varStore, "title", "%Np%:%Nz% %Vc%%:Vz%|%Oc%%:Oz%|%Tc%%:Tz%");
			
			SWdelEmail = Global.LoadIntFromVariablesStore(varStore, "SWdelEmail", 0) != 0;
			SWdelGibo = Global.LoadIntFromVariablesStore(varStore, "SWdelGibo", 0) != 0;
			SWdelNorm = Global.LoadIntFromVariablesStore(varStore, "SWdelNorm", 0) != 0;
			SWdelPaid = Global.LoadIntFromVariablesStore(varStore, "SWdelPaid", 0) != 0;
			SWdelMemail = Global.LoadIntFromVariablesStore(varStore, "SWdelMemail", 0) != 0;
                      
			titleVarK = Global.LoadIntFromVariablesStore(varStore, "titleVarK", 10);
                        
                        num_tm_accounts = Global.LoadIntFromVariablesStore(varStore, "num_tm_accounts", 0);
                        selected_tm_account = Global.LoadStringFromVariablesStore(varStore, "selected_tm_account", "");
                        tm_accounts = Global.LoadStringFromVariablesStore(varStore, "tm_accounts", "");
		}
	}

	public boolean Write() {
		Vector varStore = new Vector();

		Global.WriteStringToVariablesStore(varStore, "srcNum", srcNum, false);
		Global.WriteStringToVariablesStore(varStore, "pwd", pwd, false);
		Global.WriteStringToVariablesStore(varStore, "sign", sign, false);
		Global.WriteIntToVariablesStore(varStore, "useInternalPhonebook", useInternalPhonebook ? 1 : 0, false);
		Global.WriteIntToVariablesStore(varStore, "confirmReceiver", confirmReceiver ? 1 : 0, false);
		Global.WriteIntToVariablesStore(varStore, "animateSmile", animateSmile ? 1 : 0, false);
		Global.WriteIntToVariablesStore(varStore, "allowSounds", allowSounds ? 1 : 0, false);
		Global.WriteStringToVariablesStore(varStore, "email", email, false);
		Global.WriteIntToVariablesStore(varStore, "msgsReadPage", msgsReadPage, false);
		Global.WriteStringToVariablesStore(varStore, "msgsReadPassword", msgsReadPassword, false);
		Global.WriteIntToVariablesStore(varStore, "msgsReadRememberPassword", msgsReadRememberPassword ? 1 : 0, false);
		Global.WriteIntToVariablesStore(varStore, "expertMode", expertMode ? 1 : 0, false);
		Global.WriteIntToVariablesStore(varStore, "clearDiacritics", clearDiacritics ? 1 : 0, false);
		Global.WriteIntToVariablesStore(varStore, "capitalizeSentences", capitalizeSentences ? 1 : 0, false);
		Global.WriteIntToVariablesStore(varStore, "showCounter", showCounter ? 1 : 0, false);
		Global.WriteIntToVariablesStore(varStore, "quitOnSuccessfullSend", quitOnSuccessfullSend ? 1 : 0, false);

		Global.WriteStringToVariablesStore(varStore, "lastDestNum", lastDestNum, false);
		Global.WriteStringToVariablesStore(varStore, "lastDestEmail", lastDestEmail, false);
		Global.WriteIntToVariablesStore(varStore, "nSentMessages", nSentMessages, false);
		Global.WriteIntToVariablesStore(varStore, "lastSendWay", lastSendWay, false);
		Global.WriteStringToVariablesStore(varStore, "msg", msg, false);

                Global.WriteIntToVariablesStore(varStore, "holdGPRS", holdGPRS ? 1 : 0, false);
                Global.WriteStringToVariablesStore(varStore, "title", title, false);
		
		Global.WriteIntToVariablesStore(varStore, "SWdelEmail", SWdelEmail ? 1 : 0, false);
		Global.WriteIntToVariablesStore(varStore, "SWdelGibo", SWdelGibo ? 1 : 0, false);
		Global.WriteIntToVariablesStore(varStore, "SWdelNorm", SWdelNorm ? 1 : 0, false);
		Global.WriteIntToVariablesStore(varStore, "SWdelPaid", SWdelPaid ? 1 : 0, false);
		Global.WriteIntToVariablesStore(varStore, "SWdelMemail", SWdelMemail ? 1 : 0, false);
		
		Global.WriteIntToVariablesStore(varStore, "titleVarK", titleVarK, false);
		Global.WriteStringToVariablesStore(varStore, "tm_accounts", tm_accounts, false);
                Global.WriteStringToVariablesStore(varStore, "selected_tm_account", selected_tm_account, false);
                Global.WriteIntToVariablesStore(varStore, "num_tm_accounts", num_tm_accounts, false);
		return Global.WriteVariablesToFile(varStore, settingFile);
	}
}