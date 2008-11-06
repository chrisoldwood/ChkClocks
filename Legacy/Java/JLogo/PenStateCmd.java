/********************************************************************************
** The concrete command class used to set the turtles pen state.
*/

public class PenStateCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public PenStateCmd(String strParam)
	{
		m_strParam = strParam.trim();
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(ExecContext oContext)
	{
		int nState = 0;

		for (nState = 0; nState < s_astrStates.length; nState++)
		{
			if (s_astrStates[nState].equalsIgnoreCase(m_strParam))
				break;
		}

		if (nState >= s_astrStates.length)
			throw new ExpressionException("Invalid pen state: " + m_strParam);

		oContext.getTurtle().setPenDown(s_abStates[nState]);
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public void getSource(SourceLines oLines)
	{
		oLines.add(this, "PEN " + m_strParam);
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
				return "PEN";
			}

			public Cmd createCmd(String strSource)
			{
				return new PenStateCmd(strSource);
			}
		};
	}

	/********************************************************************************
	** Constants.
	*/

	// States.
	private static final String[]  s_astrStates = new String[]  { "down", "up"  };
	private static final boolean[] s_abStates   = new boolean[] { true,   false };

	/********************************************************************************
	** Members.
	*/

	private String	m_strParam;
}
