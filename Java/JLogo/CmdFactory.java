import java.util.Vector;

/********************************************************************************
** This class provides a factory for Cmd derived objects.
*/

public class CmdFactory
{
	/********************************************************************************
	** Default constructor.
	*/

	public CmdFactory()
	{
		m_vHandlers.addElement(CommentCmd.getFactory());
		m_vHandlers.addElement(PenStateCmd.getFactory());
		m_vHandlers.addElement(PenColorCmd.getFactory());
		m_vHandlers.addElement(ForwardCmd.getFactory());
		m_vHandlers.addElement(TurnRightCmd.getFactory());
		m_vHandlers.addElement(TurnLeftCmd.getFactory());
		m_vHandlers.addElement(RepeatCmd.getFactory());
		m_vHandlers.addElement(EndCmd.getFactory());
		m_vHandlers.addElement(AddVarCmd.getFactory());
		m_vHandlers.addElement(SetVarCmd.getFactory());
	}

	/********************************************************************************
	** Create a Cmd object from a line of source code.
	*/

	public Cmd createCmd(String strSource) throws InvalidCmdException
	{
		// Handle empty line specially.
		if (strSource.length() == 0)
			return new EmptyCmd();

		// For all handlers...
		for (int i = 0; i < m_vHandlers.size(); i++)
		{
			// Get handler and its name.
			CmdHandler oHandler = (CmdHandler) m_vHandlers.elementAt(i);
			String     strName  = oHandler.getName();

			// Get string lengths.
			int nNameLen = strName.length();
			int nSrcLen  = strSource.length();

			// Create cmd, if a match.
			if ( (nNameLen <= nSrcLen)
			  && (strName.equalsIgnoreCase(strSource.substring(0, nNameLen))) )
			{
				return oHandler.createCmd(strSource.substring(nNameLen, nSrcLen).trim());
			}
		}

		// Unknown command.
		throw new InvalidCmdException(strSource);
	}

	/********************************************************************************
	** This class provides a factory for Cmd derived objects.
	*/

	public static abstract class CmdHandler
	{
		/********************************************************************************
		** Get the name of the command.
		*/
		public abstract String getName();

		public abstract Cmd createCmd(String strSource) throws InvalidCmdException;
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private Vector	m_vHandlers = new Vector();
}
