/********************************************************************************
** The concrete command class used to end a block of other commands.
*/

public class EndCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public EndCmd()
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
		oLines.adjustIndentation(-1);
		oLines.add(this, "END");
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
				return "END";
			}

			public Cmd createCmd(String strSource)
			{
				return new EndCmd();
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
