import java.util.Vector;

/********************************************************************************
** This class is used as a vector to store the source code text and objects.
*/

public class SourceLines
{
	/********************************************************************************
	** Default Constructor.
	*/

	public SourceLines()
	{
		this(DEF_INDENT_SIZE);
	}

	/********************************************************************************
	** Full Constructor.
	*/

	public SourceLines(int nIndentSize)
	{
		m_nIndSize = nIndentSize;
	}

	/********************************************************************************
	** Get the number of lines.
	*/

	public int count()
	{
		return m_vCmds.size();
	}

	/********************************************************************************
	** Add a line of source code and its assocaited command object.
	*/

	public void add(Cmd oCmd, String strText)
	{
		m_vCmds.addElement(oCmd);
		m_vLines.addElement(strText);
		m_vIndents.addElement(new Integer(m_nIndents));
	}

	/********************************************************************************
	** Get the command object for the source line.
	*/

	public Cmd getCmd(int i)
	{
		return (Cmd) m_vCmds.elementAt(i);
	}

	/********************************************************************************
	** Get a line of source code.
	*/

	public String getLine(int i)
	{
		return (String) m_vLines.elementAt(i);
	}

	/********************************************************************************
	** Get a source lines indentation.
	*/

	public int getIndent(int i)
	{
		return ((Integer) m_vIndents.elementAt(i)).intValue();
	}

	/********************************************************************************
	** Get a line of source code, indented by spaces.
	*/

	public String getIndentedLine(int i)
	{
		String       strLine   = getLine(i);
		int          nIndent   = getIndent(i) * m_nIndSize;
		StringBuffer strBuffer = new StringBuffer(strLine.length() + nIndent);

		for (int n = 0; n < nIndent; n++)
			strBuffer.append(' ');

		strBuffer.append(strLine);

		return strBuffer.toString();
	}

	/********************************************************************************
	** Adjust the indentation.
	*/

	public void adjustIndentation(int nOffset)
	{
		m_nIndents += nOffset;
	}

	/********************************************************************************
	** Constants.
	*/

	public static final int DEF_INDENT_SIZE = 4;

	/********************************************************************************
	** Members.
	*/

	private Vector	m_vCmds    = new Vector();
	private Vector	m_vLines   = new Vector();
	private Vector	m_vIndents = new Vector();
	private int		m_nIndSize = DEF_INDENT_SIZE;
	private int		m_nIndents = 0;
}
