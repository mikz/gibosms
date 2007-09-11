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
	public int titleVarK = 9;
        
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
                      
			titleVarK = Global.LoadIntFromVariablesStore(varStore, "titleVarK", 9);
		}
	}

	public boolean Write() {
		Vector varStore = new Vector();

		Global.WriteStringToVariablesStore(varStore, "srcNum", srcNum, true);
		Global.WriteStringToVariablesStore(varStore, "pwd", pwd, true);
		Global.WriteStringToVariablesStore(varStore, "sign", sign, true);
		Global.WriteIntToVariablesStore(varStore, "useInternalPhonebook", useInternalPhonebook ? 1 : 0, true);
		Global.WriteIntToVariablesStore(varStore, "confirmReceiver", confirmReceiver ? 1 : 0, true);
		Global.WriteIntToVariablesStore(varStore, "animateSmile", animateSmile ? 1 : 0, true);
		Global.WriteIntToVariablesStore(varStore, "allowSounds", allowSounds ? 1 : 0, true);
		Global.WriteStringToVariablesStore(varStore, "email", email, true);
		Global.WriteIntToVariablesStore(varStore, "msgsReadPage", msgsReadPage, true);
		Global.WriteStringToVariablesStore(varStore, "msgsReadPassword", msgsReadPassword, true);
		Global.WriteIntToVariablesStore(varStore, "msgsReadRememberPassword", msgsReadRememberPassword ? 1 : 0, true);
		Global.WriteIntToVariablesStore(varStore, "expertMode", expertMode ? 1 : 0, true);
		Global.WriteIntToVariablesStore(varStore, "clearDiacritics", clearDiacritics ? 1 : 0, true);
		Global.WriteIntToVariablesStore(varStore, "capitalizeSentences", capitalizeSentences ? 1 : 0, true);
		Global.WriteIntToVariablesStore(varStore, "showCounter", showCounter ? 1 : 0, true);
		Global.WriteIntToVariablesStore(varStore, "quitOnSuccessfullSend", quitOnSuccessfullSend ? 1 : 0, true);

		Global.WriteStringToVariablesStore(varStore, "lastDestNum", lastDestNum, true);
		Global.WriteStringToVariablesStore(varStore, "lastDestEmail", lastDestEmail, true);
		Global.WriteIntToVariablesStore(varStore, "nSentMessages", nSentMessages, true);
		Global.WriteIntToVariablesStore(varStore, "lastSendWay", lastSendWay, true);
		Global.WriteStringToVariablesStore(varStore, "msg", msg, true);

                Global.WriteIntToVariablesStore(varStore, "holdGPRS", holdGPRS ? 1 : 0, true);
                Global.WriteStringToVariablesStore(varStore, "title", title, true);
		
		Global.WriteIntToVariablesStore(varStore, "SWdelEmail", SWdelEmail ? 1 : 0, true);
		Global.WriteIntToVariablesStore(varStore, "SWdelGibo", SWdelGibo ? 1 : 0, true);
		Global.WriteIntToVariablesStore(varStore, "SWdelNorm", SWdelNorm ? 1 : 0, true);
		Global.WriteIntToVariablesStore(varStore, "SWdelPaid", SWdelPaid ? 1 : 0, true);
		Global.WriteIntToVariablesStore(varStore, "SWdelMemail", SWdelMemail ? 1 : 0, true);
		
		Global.WriteIntToVariablesStore(varStore, "titleVarK", titleVarK, true);
		
		return Global.WriteVariablesToFile(varStore, settingFile);
	}
}