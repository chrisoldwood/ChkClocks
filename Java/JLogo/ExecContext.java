/********************************************************************************
** This class is used to maintain the context whilst a program is executing.
*/

public class ExecContext
{
	/********************************************************************************
	** Constructor.
	*/

	public ExecContext(Turtle oTurtle)
	{
		this(oTurtle, null, 0);
	}

	/********************************************************************************
	** Constructor.
	*/

	public ExecContext(Turtle oTurtle, Thread oThread, int nInterval)
	{
		m_oTurtle     = oTurtle;
		m_oThread     = oThread;
		m_nInterval   = nInterval;
		m_oVariables  = new Variables();
		m_oExprParser = new ExpressionParser(m_oVariables);
	}

	/********************************************************************************
	** Accessors.
	*/

	public Turtle           getTurtle()			{ return m_oTurtle;     }
	public ExpressionParser getExprParser()		{ return m_oExprParser; }
	public Variables        getVariables()		{ return m_oVariables;  }

	/********************************************************************************
	** Pause the execution.
	*/

	public void pause()
	{
		if (m_oThread != null)
		{
			try
			{
				m_oThread.sleep(m_nInterval);
			}
			catch(InterruptedException e)
			{ }
		}
	}

	/********************************************************************************
	** Reset the context.
	*/

	public void clear()
	{
		m_oVariables.clear();
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private Turtle				m_oTurtle;
	private Thread				m_oThread;
	private int					m_nInterval;
	private ExpressionParser	m_oExprParser;
	private Variables			m_oVariables;
}
