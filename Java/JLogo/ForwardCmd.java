/********************************************************************************
** The concrete command class used to move the turtle forward.
*/

public class ForwardCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public ForwardCmd(int nPixels)
	{
		m_nPixels = nPixels;
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(Turtle oTurtle)
	{
		oTurtle.forward(m_nPixels);
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public String getSource()
	{
		return "FORWARD " + m_nPixels + "\n";
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private int	m_nPixels;
}
