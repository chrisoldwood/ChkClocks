/********************************************************************************
** The concrete command class used to repeat a serious of other commands.
*/

public class RepeatCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public RepeatCmd(String strParam)
	{
		m_strParam = strParam;
	}

	/********************************************************************************
	** Adds a command to the loop.
	*/

	public void add(Cmd oCmd)
	{
		m_vCmds.add(oCmd);

		if (oCmd instanceof EndCmd)
			m_bClosed = true;
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(ExecContext oContext)
	{
		int nCount = 1;

		if (m_bClosed)
			nCount = (int) oContext.getExprParser().evaluate(m_strParam);

		for (int i = 0; i < nCount; i++)
			m_vCmds.execute(oContext);
	}

	/********************************************************************************
	** Execute the all but the first iteration of the loop.
	** NB: Used when editing to repeat the edited steps of the loop.
	*/

	public void executeEndRepeat(ExecContext oContext)
	{
		int nCount = (int) oContext.getExprParser().evaluate(m_strParam);

		nCount--;

		for (int i = 0; i < nCount; i++)
			m_vCmds.execute(oContext);
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public void getSource(SourceLines oLines)
	{
		oLines.add(this, "REPEAT " + m_strParam);

		oLines.adjustIndentation(+1);
		m_vCmds.getSource(oLines);
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
				return "REPEAT";
			}

			public Cmd createCmd(String strSource)
			{
				return new RepeatCmd(strSource);
			}
		};
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private String		m_strParam;
	private CmdBlock	m_vCmds    = new CmdBlock();
	private boolean		m_bClosed  = false;
}
