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
	** Queries if the command requires parameters.
	*/

	public boolean isParameterised()
	{
		return false;
	}

	/********************************************************************************
	** Get the commands parameter.
	*/

	public String getParameter()
	{
		return null;
	}

	/********************************************************************************
	** Set the commands parameter.
	*/

	public void setParameter(String strParam)
	{
	}

	/********************************************************************************
	** Queries if the command can be removed.
	*/

	public boolean isRemoveable()
	{
		return true;
	}

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
