/********************************************************************************
** The base class for all commands.
*/

public abstract class Cmd
{
	/********************************************************************************
	** Execute the command.
	*/

	public abstract void execute(Turtle oTurtle);

	/********************************************************************************
	** Get the source code for the command.
	*/

	public abstract String getSource();

	/********************************************************************************
	** Get the next command.
	*/

	public Cmd getNext(boolean bExecuting)
	{
		return m_oNext;
	}

	/********************************************************************************
	** Set the next command.
	*/

	public void setNext(Cmd oNext)
	{
		m_oNext = oNext;
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private Cmd	m_oNext;
}
