/********************************************************************************
** The concrete command class used to turn the turtle left.
*/

public class TurnLeftCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public TurnLeftCmd(int nDegrees)
	{
		m_nDegrees = nDegrees;
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(Turtle oTurtle)
	{
		oTurtle.turnLeft(m_nDegrees);
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public String getSource()
	{
		return "LEFT " + m_nDegrees + "\n";
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private int	m_nDegrees;
}
