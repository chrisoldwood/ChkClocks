import java.util.Vector;

/********************************************************************************
** The concrete command class used as a composite for other commands.
*/

public class CmdBlock extends Cmd
{
	/********************************************************************************
	** Execute the command.
	*/

	public void execute(ExecContext oContext)
	{
		for (int i = 0; i < m_vCmds.size(); i++)
		{
			Cmd oCmd = (Cmd) m_vCmds.elementAt(i);

			oCmd.execute(oContext);
		}
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public void getSource(SourceLines oLines)
	{
		for (int i = 0; i < m_vCmds.size(); i++)
		{
			Cmd oCmd = (Cmd) m_vCmds.elementAt(i);

			oCmd.getSource(oLines);
		}
	}

	/********************************************************************************
	** Queries if the command requires parameters.
	*/

	public boolean isParameterised()
	{
		return false;
	}

	/********************************************************************************
	** Adds a command to the block.
	*/

	public void add(Cmd oCmd)
	{
		oCmd.setParent(this);

		m_vCmds.addElement(oCmd);
	}

	/********************************************************************************
	** Removes a command from the block.
	*/

	public void remove(Cmd oCmd)
	{
		m_vCmds.removeElement(oCmd);
	}

	/********************************************************************************
	** Removes all commands from the block.
	*/

	public void removeAll()
	{
		m_vCmds.removeAllElements();
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private Vector	m_vCmds = new Vector();
}
