/********************************************************************************
** The concrete command class used to define a new variable.
*/

public class AddVarCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public AddVarCmd(String strName, String strParam)
	{
		m_strName = strName;
		m_strParam = strParam;
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(ExecContext oContext)
	{
		double dValue = oContext.getExprParser().evaluate(m_strParam);

		oContext.getVariables().add(m_strName, dValue);
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public void getSource(SourceLines oLines)
	{
		oLines.add(this, "VAR " + m_strName + " = " + m_strParam);
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
				return "VAR";
			}

			public Cmd createCmd(String strSource) throws InvalidCmdException
			{
				int nEquals = strSource.indexOf('=');
				int nLen    = strSource.length();

				// String must consist of 'name = value'.
				if ( (nLen < 3) || (nEquals < 0) )
					throw new InvalidCmdException("Missing '=' operator");

				if (nEquals > (nLen-2))
					throw new InvalidCmdException("Missing variable value");

				// Extract name and value.
				String strName  = strSource.substring(0, nEquals).trim();
				String strValue = strSource.substring(nEquals+1, nLen).trim();

				if (!Variables.isValidName(strName))
					throw new InvalidCmdException("Invalid variable name: " + strName);

				return new AddVarCmd(strName, strValue);
			}
		};
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private String	m_strName;
	private String	m_strParam;
}
