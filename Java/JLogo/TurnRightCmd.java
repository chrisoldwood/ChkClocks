/********************************************************************************
** The concrete command class used to turn the turtle right.
*/

public class TurnRightCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public TurnRightCmd(int nDegrees)
	{
		m_nDegrees = nDegrees;
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(Turtle oTurtle)
	{
		oTurtle.turnRight(m_nDegrees);
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public String getSource()
	{
		return "RIGHT " + m_nDegrees + "\n";
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private int	m_nDegrees;
}
