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
	** Execute the command.
	*/

	public void execute(ExecContext oContext)
	{
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public void getSource(SourceLines oLines)
	{
		oLines.adjustIndentation(-4);
		oLines.add(this, "ENDREPEAT");
	}

	/********************************************************************************
	** Queries if the command can be removed.
	*/

	public boolean isRemoveable()
	{
		return false;
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
				return "ENDREPEAT";
			}

			public Cmd createCmd(String strSource)
			{
				return new EndRepeatCmd();
			}
		};
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/
}
