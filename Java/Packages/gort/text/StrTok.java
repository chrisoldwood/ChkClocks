package gort.text;

import java.util.Vector;

/********************************************************************************
** This is a string tokeniser class designed to split text strings into lines
** and fields, returning them as String arrays.
*/

public class StrTok
{
	/********************************************************************************
	** Default constructor.
	*/

	public StrTok()
	{
		this(false, true);
	}

	/********************************************************************************
	** Full constructor.
	*/

	public StrTok(boolean bTrim, boolean bRetEmpty)
	{
		m_bTrim     = bTrim;
		m_bRetEmpty = bRetEmpty;
	}

	/********************************************************************************
	** Split the text into separate lines.
	*/

	public String[] splitLines(String str)
	{
		// Empty string?
		if (str.length() == 0)
		{
			if (m_bRetEmpty)	return new String[] { str };
			else				return new String[0];
		}

		Vector vLines  = new Vector();
		int    nLength = str.length();

		// Split the string.
		for (int i = 0, s = 0; i < nLength; i++)
		{
			char    cChar = str.charAt(i);
			boolean bEOL  = ((cChar == '\r') || (cChar == '\n'));
			boolean bEOS  = ((i+1) == nLength);

			// Found EOL or EOS?
			if (bEOL || bEOS)
			{
				// Calculate the string end.
				int e = (bEOL) ? (i-1) : i;

				String strLine = str.substring(s, e+1);

				// Trim whitespace?
				if (m_bTrim)
					strLine = strLine.trim();

				// Add the line.
				if ( (m_bRetEmpty) || (strLine.length() > 0) )
					vLines.addElement(strLine);

				// FoundCR, Skip LF?
				if ( (cChar == '\r') && (!bEOS) && (str.charAt(i+1) == '\n') )
					i++;

				// Mark start of next string.
				s = i + 1;
			}
		}

		// Allocate return array.
		String[] astrLines = new String[vLines.size()];

		// Convert vector contents to return array.
		vLines.copyInto(astrLines);

		return astrLines;
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private boolean m_bTrim;			// Trim leading/trailing whitespace?
	private boolean	m_bRetEmpty;		// Return empty fields?
}
