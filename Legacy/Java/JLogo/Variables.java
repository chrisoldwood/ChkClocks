import java.util.*;

/********************************************************************************
** This class is used to store the collection of variables.
*/

public class Variables
{
	/********************************************************************************
	** Clear the collection.
	*/

	public void clear()
	{
		m_htVars.clear();
	}

	/********************************************************************************
	** Checks if the variable is defined.
	*/

	public boolean exists(String strName)
	{
		strName = strName.toLowerCase();

		return m_htVars.containsKey(strName);
	}

	/********************************************************************************
	** Add a variable the collection.
	*/

	public void add(String strName, double dValue)
	{
		strName = strName.toLowerCase();

		m_htVars.put(strName, new Double(dValue));
	}

	/********************************************************************************
	** Sets the value of a variable.
	*/

	public void set(String strName, double dValue)
	{
		strName = strName.toLowerCase();

		m_htVars.put(strName, new Double(dValue));
	}

	/********************************************************************************
	** Gets the value of a variable.
	*/

	public double get(String strName)
	{
		strName = strName.toLowerCase();

		Double dValue = (Double) m_htVars.get(strName);

		return dValue.doubleValue();
	}

	/********************************************************************************
	** Checks if the variable name is valid.
	*/

	public static boolean isValidName(String strName)
	{
		strName = strName.toLowerCase();

		for (int i = 0; i < strName.length(); i++)
		{
			char cChar = strName.charAt(i);

			// Is a letter?
			if ( ((cChar >= 'a') && (cChar <= 'z')) || ((cChar >= 'A') && (cChar <= 'Z')) )
				continue;

			// Is a digit AND not first char?
			if ( ((cChar >= '0') && (cChar <= '9')) && (i != 0) )
				continue;

			// Is other special char?
			if (cChar == '_')
				continue;

			return false;
		}

		return true;
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private Hashtable m_htVars = new Hashtable();
}
