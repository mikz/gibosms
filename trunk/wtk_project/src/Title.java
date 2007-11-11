import java.util.*;
import java.lang.*;
/*
 * Title.java
 *
 * Created on 9. zברם 2007, 1:51
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

/**
 *
 * @author Administrator
 */
public class Title {
    	public final static int vodafone_chars = (160 - 8);
        public final static int o2_chars = (60 - 4);
        public final static int tmobile_chars = 160;
	private Setting setting = new Setting();
	private String sett = null;
	private String title = "";
	private int writtenChars = 0;
	private int parts = 0;
	private int chars = 0;
	private int strlen = 0;
	private int strlenmax = 0;
    /** Creates a new instance of Title */
    public Title() {
	    this.setting.Load();
	    System.gc();
    }
    public String c(int parts)
    {
	return String.valueOf(parts);
    }
    public String zk(int parts, int chars) 
    {
	return String.valueOf((chars*parts - this.writtenChars)<setting.titleVarK ? String.valueOf(chars*parts - this.writtenChars) : "");
    }
    public String p(int parts, int chars)
    {
	return String.valueOf(chars - (chars*parts - this.writtenChars));
    }
    public String pk(int parts, int chars)
    {
	return String.valueOf((chars - (chars*parts - this.writtenChars))<setting.titleVarK ? String.valueOf(chars - (chars*parts - this.writtenChars)) : "");
    }
    public String z(int parts, int chars)
    {
	return String.valueOf(chars*parts - this.writtenChars);
    }
    public String Nz(int strlenmax)
    {
	return String.valueOf(strlenmax-this.writtenChars);
    }
    public String Np()
    {
	return String.valueOf( this.writtenChars);
    }
    public String H()
    {
        Calendar cal = Calendar.getInstance(TimeZone.getDefault());
        cal.setTime(new Date());
        return (cal.get(Calendar.HOUR_OF_DAY) <= 9)?"0":"" + cal.get(Calendar.HOUR_OF_DAY);
    }
    public String M() {
        Calendar cal = Calendar.getInstance(TimeZone.getDefault());
        cal.setTime(new Date());
        return (cal.get(Calendar.MINUTE) <= 9)?"0":"" + cal.get(Calendar.MINUTE);
    }
    public String D() {
        Calendar cal = Calendar.getInstance(TimeZone.getDefault());
        cal.setTime(new Date());
        return cal.get(Calendar.DAY_OF_MONTH) + "." + (cal.get(Calendar.MONTH) + 1);
    }
    
    public String variable(String variable)
    {
	String buff = "";
	this.writtenChars = this.strlen + this.setting.sign.length() + (this.setting.sign.length() > 0 ? 1 /*mezera*/ : 0);
	int partsV = ((this.writtenChars - 1) / vodafone_chars) + 1;
	int partsO = ((this.writtenChars - 1) / o2_chars) + 1;
        int partsT = ((this.writtenChars - 1) / tmobile_chars) +1;
	         for(int i=0;i<variable.length();i++){
                                    switch (variable.charAt(i)) {
                                        case 'V':
                                            switch(variable.charAt(i+1)){
                                                case 'c':
                                                    buff += c(partsV);
						    i++;
                                                    break;
                                                case 'z':
                                                    if(variable.length() > i+2 && variable.charAt(i+2) == 'k')
							if(zk(partsV,vodafone_chars)!=""){
							    buff += zk(partsV,vodafone_chars);
							    i++;
							}
							else return "";
						    else buff+=z(partsV,vodafone_chars);
						    i++;
                                                    break;
                                                case 'p':
						    if(variable.length() > i+2 && variable.charAt(i+2) == 'k')
							if(pk(partsV,vodafone_chars) != "") {
							    buff += pk(partsV,vodafone_chars);
							    i++;
							}
							else return "";
                                                    else buff+=p(partsV,vodafone_chars);
						    i++;
                                                    break;
                                           }
                                            break;
                                        case 'O':
                                            switch(variable.charAt(i+1)){
                                                case 'c':
                                                    buff += c(partsO);
						    i++;
                                                    break;
                                                case 'z':
                                                    if(variable.length() > i+2 && variable.charAt(i+2) == 'k')
							if(zk(partsO,o2_chars) != "") {
							    buff += zk(partsO,o2_chars);
							    i++;
							}
							else return "";
                                                    else buff+=z(partsO,o2_chars);
						    i++;
                                                    break;
                                                case 'p':
						    if(variable.length() > i+2 && variable.charAt(i+2) == 'k')
							if(pk(partsO,o2_chars) != "") {
							    buff += pk(partsO,o2_chars);
							    i++;
							}
							else return "";
                                                    else buff+=p(partsO,o2_chars);
						    i++;
                                                    break;
                                            }
                                            break;
				     case 'T':
                                            switch(variable.charAt(i+1)){
                                                case 'c':
                                                    buff += c(partsT);
						    i++;
                                                    break;
                                                case 'z':
                                                    if(variable.length() > i+2 && variable.charAt(i+2) == 'k')
							if(zk(partsT,tmobile_chars) != "") {
							    buff += zk(partsT,tmobile_chars);
							    i++;
							}
							else return "";
                                                    else buff+=z(partsT,tmobile_chars);
						    i++;
                                                    break;
                                                case 'p':
						    if(variable.length() > i+2 && variable.charAt(i+2) == 'k')
							if(pk(partsT,tmobile_chars) != "") {
							    buff += pk(partsT,tmobile_chars);
							    i++;
							}
							else return "";
                                                    else buff+=p(partsT,tmobile_chars);
						    i++;
                                                    break;
                                            }
                                            break;
                                        case 'N':
                                            switch(variable.charAt(i+1)){
                                                case 'p':
						    buff += Np();
						    i++;
                                                    break;
                                                case 'z':
                                                    buff += Nz(strlenmax);
						    i++;
                                                    break;
                                            }
                                            break;
                                         case 'H':
                                            buff += H();
                                            i++;
                                            break;
                                         case 'M':
                                            buff += M();
                                            i++;
                                            break;
                                         case 'D':
                                            buff += D();
                                            i++;
                                            break;
					
					default:
					    buff += variable.charAt(i);
					    break;
                                    }
                                
                            }
	return buff;
    }
    
    public String parse(int strlen,int strlenmax)
    {
	sett = this.setting.title;
	this.strlen = strlen;
	this.strlenmax = strlenmax;
        this.title = "";
	
	String variable = new String();
	  for(int i=0;i<sett.length();i++){
	    if(this.sett.charAt(i)=='%') {
		for(int j=i+1;j<sett.length();j++)
		{
		    if(this.sett.charAt(j) == '%' && j < sett.length())
		    {
			variable = sett.substring(i+1,j);
			this.title += this.variable(variable);
			i += variable.length()+1;
			break;
			
		    } 
		}
	    }
	    else this.title += this.sett.charAt(i);
	  }
        System.gc();
	return this.title;
    }
    
}
