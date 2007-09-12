/*
 * SENDWAYstr.java
 *
 * Created on 12. záøí 2007, 9:20
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

/**
 *
 * @author MikZ
 */
public class SENDWAYstr {
        public static String GPRSMSG = "SMS zdarma pøes GPRS";
    	public static String EMAIL = "Email";
	public static String MAILBOX = "Gibomeska";
	public static String NORMALSMS = "Normální SMS";
	public static String WEBSMS = "Web SMS zdarma";
	public static String MOBEMAIL = "Mobilní email";
	public static String PAIDSMS = "SMS za 1 Kè";
        public int sendWay = -1;
        public SENDWAYstr(String str)
        {
            if (str == this.EMAIL) this.sendWay = SENDWAY.EMAIL;
            if (str == this.GPRSMSG) this.sendWay = SENDWAY.GPRSMSG;
            if (str == this.MAILBOX) this.sendWay = SENDWAY.MAILBOX;
            if (str == this.NORMALSMS) this.sendWay = SENDWAY.NORMALSMS;
            if (str == this.WEBSMS) this.sendWay = SENDWAY.WEBSMS;
            if (str == this.MOBEMAIL) this.sendWay = SENDWAY.MOBEMAIL;
            if (str == this.PAIDSMS) this.sendWay = SENDWAY.PAIDSMS;
        }
};
