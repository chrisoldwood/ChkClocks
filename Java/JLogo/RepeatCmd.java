/********************************************************************************
** The concrete command class used to repeat a serious of other commands.
*/

public class RepeatCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public RepeatCmd(int nCount)
	{
		m_nCount = nCount;
	}

	/********************************************************************************
	** Adds a command to the loop.
	*/

	public void add(Cmd oCmd)
	{
		if (m_oFirstCmd == null)
			m_oFirstCmd = oCmd;
		else
			m_oLastCmd.setNext(oCmd);

		m_oLastCmd = oCmd;
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(Turtle oTurtle)
	{
		m_nExecCount = m_nCount;
		m_oNextCmd   = m_oFirstCmd;
	}

	/********************************************************************************
	** Get the next command.
	*/

	public Cmd getNext(boolean bExecuting)
	{
		if (bExecuting)
		{
			if (m_nExecCount == 0)
				return super.getNext(bExecuting);

			if (!(m_oNextCmd instanceof EndRepeatCmd))
			{
				Cmd oNext = m_oNextCmd;
				m_oNextCmd = m_oNextCmd.getNext(bExecuting);

				return oNext;
			}

			m_nExecCount--;
			m_oNextCmd = m_oFirstCmd;

			return getNext(bExecuting);
		}
		else // !bExecuting
		{
			if (m_oNextCmd == null)
			{
				m_oNextCmd = m_oFirstCmd;

				if (m_oNextCmd != null)
					return m_oNextCmd;
				else
					return super.getNext(bExecuting);
			}
			else
			{
				m_oNextCmd = null;
				return super.getNext(bExecuting);
			}
		}
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public String getSource()
	{
		return "REPEAT " + m_nCount + "\n";
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private int	m_nCount;
	private Cmd	m_oFirstCmd;
	private Cmd	m_oLastCmd;

	private int m_nExecCount;
	private Cmd	m_oNextCmd;
}
