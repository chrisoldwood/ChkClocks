/********************************************************************************
** The concrete command class used to set the turtles pen state.
*/

public class PenStateCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public PenStateCmd(boolean bDown)
	{
		m_bDown = bDown;
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(Turtle oTurtle)
	{
		oTurtle.setPenDown(m_bDown);
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public String getSource()
	{
		return "PEN " + ((m_bDown) ? "DOWN" : "UP") + "\n";
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private boolean	m_bDown;
}
