import java.awt.*;

/********************************************************************************
** The concrete command class used to move the turtle forward.
*/

public class PenColorCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public PenColorCmd(String strParam)
	{
		m_strParam = strParam.trim();
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(ExecContext oContext)
	{
		int nClr = 0;

		for (nClr = 0; nClr < s_astrClrs.length; nClr++)
		{
			if (s_astrClrs[nClr].equalsIgnoreCase(m_strParam))
				break;
		}

		if (nClr >= s_astrClrs.length)
			throw new ExpressionException("Invalid colour: " + m_strParam);

		oContext.getTurtle().setPenColour(s_aclrClrs[nClr], m_strParam);
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public void getSource(SourceLines oLines)
	{
		oLines.add(this, "COLOUR " + m_strParam);
	}

	/********************************************************************************
	** Queries if the command requires parameters.
	*/

	public boolean isParameterised()
	{
		return true;
	}

	/********************************************************************************
	** Get the commands parameter.
	*/

	public String getParameter()
	{
		return m_strParam;
	}

	/********************************************************************************
	** Set the commands parameter.
	*/

	public void setParameter(String strParam)
	{
		m_strParam = strParam;
	}

	/********************************************************************************
	** Get the commands' factory.
	*/

	public static CmdFactory.CmdHandler getFactory()
	{
		// Anonymous inner class used by the command factory.
		return new CmdFactory.CmdHandler()
		{
			public String getName()
			{
				return "COLOUR";
			}

			public Cmd createCmd(String strSource)
			{
				return new PenColorCmd(strSource);
			}
		};
	}

	/********************************************************************************
	** Constants.
	*/

	// Colours.
	private static final String[] s_astrClrs = new String[]
	{ "Black",     "Red",     "Green",     "Blue",     "White",     "Cyan",     "Magenta",     "Yellow"};

	private static final Color[]  s_aclrClrs = new Color[]
	{ Color.black, Color.red, Color.green, Color.blue, Color.white, Color.cyan, Color.magenta, Color.yellow};

	/********************************************************************************
	** Members.
	*/

	private String	m_strParam;
}
