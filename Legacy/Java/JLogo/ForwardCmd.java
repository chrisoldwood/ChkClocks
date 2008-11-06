/********************************************************************************
** The concrete command class used to move the turtle forward.
*/

public class ForwardCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public ForwardCmd(String strParam)
	{
		m_strParam = strParam;
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(ExecContext oContext)
	{
		double dDistance = oContext.getExprParser().evaluate(m_strParam);

		oContext.getTurtle().forward(dDistance);
		oContext.pause();
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public void getSource(SourceLines oLines)
	{
		oLines.add(this, "FORWARD " + m_strParam);
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
				return "FORWARD";
			}

			public Cmd createCmd(String strSource)
			{
				return new ForwardCmd(strSource);
			}
		};
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private String	m_strParam;
}
