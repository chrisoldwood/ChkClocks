/********************************************************************************
** The concrete command class used to store an empty line of source.
*/

public class EmptyCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public EmptyCmd()
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
		oLines.add(this, "");
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
				return "";
			}

			public Cmd createCmd(String strSource)
			{
				return new EmptyCmd();
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
