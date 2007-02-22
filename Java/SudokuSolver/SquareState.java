
/********************************************************************************
** This holds the state for a puzzle square. Each square can hold either an
** answer provided originally, an answer provided by the user or a set of
** possible answers provided by the user.
*/

public class SquareState
{
	/********************************************************************************
	** Default constructor.
	*/

	public SquareState()
	{
	}

	/********************************************************************************
	** Get the state.
	*/

	public int getState()
	{
		return m_nState;
	}

	/********************************************************************************
	** Is the answer a predefined one, i.e. provided by the puzzle author?
	*/

	public boolean isPredefinedValue()
	{
		return (m_nState == PREDEFINED_ANSWER);
	}

	/********************************************************************************
	** Is the value provided by the user, i.e. an answer or list of choices?
	*/

	public boolean isUserValue()
	{
		return ( (m_nState == USER_ANSWER) || (m_nState == USER_CHOICES) );
	}

	/********************************************************************************
	** Get the answer.
	*/

	public int getAnswer()
	{
		if ( (m_nState != PREDEFINED_ANSWER) && (m_nState != USER_ANSWER) )
			throw new IllegalStateException();

		return m_nAnswer;
	}

	/********************************************************************************
	** Clear the state.
	*/

	public void clear()
	{
		m_nState    = NO_ANSWER;
		m_nAnswer   = -1;
		m_abChoices = null;
	}

	/********************************************************************************
	** Set the answer. The answer can either be one provided by the puzzle or one
	** provided by the user.
	*/

	public void setAnswer(int nAnswer, boolean bPredefined)
	{
		m_nState    = (bPredefined) ? PREDEFINED_ANSWER : USER_ANSWER;
		m_nAnswer   = nAnswer;
		m_abChoices = null;
	}

	/********************************************************************************
	** Get the array of possible answers.
	*/

	public boolean[] getChoices()
	{
		if (m_nState != USER_CHOICES)
			throw new IllegalStateException();

		return m_abChoices;
	}

	/********************************************************************************
	** Set the array of possible answers.
	*/

	public void setChoices(boolean[] abChoices)
	{
		m_nState    = USER_CHOICES;
		m_nAnswer   = -1;
		m_abChoices = abChoices;
	}

	/********************************************************************************
	** Contants.
	*/

	public static final int NO_ANSWER			= 0;
	public static final int PREDEFINED_ANSWER	= 1;
	public static final int USER_CHOICES		= 2;
	public static final int USER_ANSWER			= 3;

	/********************************************************************************
	** Members.
	*/

	private int			m_nState    = NO_ANSWER;
	private int			m_nAnswer   = -1;
	private boolean[]	m_abChoices = null;
}
