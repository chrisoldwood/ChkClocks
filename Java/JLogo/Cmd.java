/********************************************************************************
** The base class for all commands.
*/

public abstract class Cmd
{
	/********************************************************************************
	** Execute the command.
	*/

	public abstract void execute(ExecContext oContext);

	/********************************************************************************
	** Get the source code for the command.
	*/

	public abstract void getSource(SourceLines oLines);

	/********************************************************************************
	** Parent accessor/modifier.
	*/

	public CmdBlock getParent()
	{
		return m_oParent;
	}

	public void setParent(CmdBlock oParent)
	{
		m_oParent = oParent;
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private CmdBlock m_oParent;
}
