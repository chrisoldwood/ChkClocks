/********************************************************************************
** The concrete command class used to turn the turtle left.
*/

public class TurnLeftCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public TurnLeftCmd(String strParam)
	{
		m_strParam = strParam;
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(ExecContext oContext)
	{
		double dDegrees = oContext.getExprParser().evaluate(m_strParam);

		oContext.getTurtle().turnLeft(dDegrees);
		oContext.pause();
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public void getSource(SourceLines oLines)
	{
		oLines.add(this, "LEFT " + m_strParam);
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
				return "LEFT";
			}

			public Cmd createCmd(String strSource)
			{
				return new TurnLeftCmd(strSource);
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
