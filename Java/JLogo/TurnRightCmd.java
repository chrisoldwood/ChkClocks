/********************************************************************************
** The concrete command class used to turn the turtle right.
*/

public class TurnRightCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public TurnRightCmd(String strParam)
	{
		m_strParam = strParam;
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(ExecContext oContext)
	{
		double dDegrees = oContext.getExprParser().evaluate(m_strParam);

		oContext.getTurtle().turnRight(dDegrees);
		oContext.pause();
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public void getSource(SourceLines oLines)
	{
		oLines.add(this, "RIGHT " + m_strParam);
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
				return "RIGHT";
			}

			public Cmd createCmd(String strSource)
			{
				return new TurnRightCmd(strSource);
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
