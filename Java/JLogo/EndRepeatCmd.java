/********************************************************************************
** The concrete command class used to end the repetition of other commands.
*/

public class EndRepeatCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public EndRepeatCmd()
	{
	}

	/********************************************************************************
	** Set the owning loop command.
	*/

	public void setRepeatCmd(RepeatCmd oCmd)
	{
		m_oRepeat = oCmd;
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(Turtle oTurtle)
	{
	}

	/********************************************************************************
	** Get the next command.
	*/

	public Cmd getNext(boolean bExecuting)
	{
		return m_oRepeat.getNext(bExecuting);
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public String getSource()
	{
		return "END REPEAT\n";
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private RepeatCmd	m_oRepeat;
}
