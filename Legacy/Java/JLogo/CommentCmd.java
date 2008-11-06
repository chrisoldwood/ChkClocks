/********************************************************************************
** The concrete command class used to store a source code comment.
*/

public class CommentCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public CommentCmd(String strParam)
	{
		m_strParam = strParam;
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
		oLines.add(this, "// " + m_strParam);
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
				return "//";
			}

			public Cmd createCmd(String strSource)
			{
				return new CommentCmd(strSource);
			}
		};
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private String	m_strParam;
}
