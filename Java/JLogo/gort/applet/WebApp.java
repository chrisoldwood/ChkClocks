import java.applet.*;
import java.awt.*;

/********************************************************************************
** The base class for all Applets.
** It provides methods useful for all applets.
*/

public class WebApp extends Applet
{
	/********************************************************************************
	** Methods to get page parameters of various types.
	*/

	public String getStrParam(String strName, String strDefault)
	{
		String str = getParameter(strName);

		if ( (str == null) || (str.length() == 0) )
			return strDefault;

		return str;
	}

	public short getShortParam(String strName, short sDefault)
	{
		String str = getParameter(strName);

		if ( (str == null) || (str.length() == 0) )
			return sDefault;

		return Short.parseShort(str);
	}

	public int getIntParam(String strName, int nDefault)
	{
		String str = getParameter(strName);

		if ( (str == null) || (str.length() == 0) )
			return nDefault;

		return Integer.parseInt(str);
	}

	public double getDoubleParam(String strName, double dDefault)
	{
		String str = getParameter(strName);

		if ( (str == null) || (str.length() == 0) )
			return dDefault;

		return Double.valueOf(str).doubleValue();
	}

	public boolean getBoolParam(String strName, boolean bDefault)
	{
		String str = getParameter(strName);

		if ( (str == null) || (str.length() == 0) )
			return bDefault;

		return Boolean.valueOf(str).booleanValue();
	}

	public Color getColorParam(String strName, Color clrDefault)
	{
		String str = getParameter(strName);

		if ( (str == null) || (str.length() == 0) )
			return clrDefault;

		if ( (str.length() != 7) || (str.charAt(0) != '#') )
			throw new NumberFormatException("Invalid colour value: " + str);

		int nRed = Integer.parseInt("0x" + str.substring(1, 3));
		int nGrn = Integer.parseInt("0x" + str.substring(3, 5));
		int nBlu = Integer.parseInt("0x" + str.substring(5, 7));

		return new Color(nRed, nGrn, nBlu);
	}

	/********************************************************************************
	** Finds the parent frame of the applet.
	*/

	public Frame getParentFrame()
	{
		Component c = this;

        while((c instanceof Frame) == false)
			c = c.getParent();

		return (Frame) c;
	}

	/********************************************************************************
	** Constants
	*/

	/********************************************************************************
	** Members.
	*/
}
