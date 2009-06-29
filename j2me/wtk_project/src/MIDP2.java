import javax.microedition.io.Connector;
import javax.wireless.messaging.*;
import javax.microedition.media.control.*;
import javax.microedition.media.*;
import javax.microedition.lcdui.*;

public class MIDP2 {
	public static int TextField_ANYINITIAL_CAPS_SENTENCE = TextField.ANY|0x200000;

	public static boolean sendNormalSMS(String number, String message, StringBuffer ret) {
		boolean ok = false;
		MessageConnection sender = null;
		try {
			sender = (MessageConnection)Connector.open("sms://" + number);
			TextMessage msg = (TextMessage)sender.newMessage(MessageConnection.TEXT_MESSAGE);
			msg.setPayloadText(message);
			sender.send(msg);
			sender.close();
			ok = true;
			ret.append("Zpráva byla odeslána");
		}catch (Exception e) {
			ret.append(e.getMessage());
		}
		return ok;
	}

	public static void playStatusSound(boolean success) {
		byte tempo = 60; // set tempo to 120 bpm
		byte d = 8;      // eighth-note

		byte C4 = (byte)(ToneControl.C4 + (!success ? 4 : 0));
		byte D4 = (byte)(C4 + 2); // a whole step
		byte E4 = (byte)(C4 + 4); // a major third
		byte rest = ToneControl.SILENCE; // rest

		try {
			Player p = Manager.createPlayer(Manager.TONE_DEVICE_LOCATOR);
			p.realize();
			ToneControl c = (ToneControl)p.getControl("ToneControl");

			if (!success) {
				byte[] mySequence = {
					ToneControl.VERSION, 1,   // version 1
					ToneControl.TEMPO, tempo, // set tempo
					E4,d, E4,d,E4,d     // content
				};

				c.setSequence(mySequence);
			}else {
				byte[] mySequence = {
						ToneControl.VERSION, 1,       // version 1
						ToneControl.TEMPO, tempo,     // set tempo
						D4,d, D4,d, E4,d, D4,d, C4,d  // content
				};

				c.setSequence(mySequence);
			}

			p.start();
		}catch (Exception e) {
			//showErr(e.toString());
		}
	}
}