import java.awt.*;

/********************************************************************************
** The concrete command class used to move the turtle forward.
*/

public class PenColorCmd extends Cmd
{
	/********************************************************************************
	** Constructor.
	*/

	public PenColorCmd(Color clrPen, String strClr)
	{
		m_clrPen = clrPen;
		m_strClr = strClr;
	}

	/********************************************************************************
	** Execute the command.
	*/

	public void execute(Turtle oTurtle)
	{
		oTurtle.setPenColour(m_clrPen, m_strClr);
	}

	/********************************************************************************
	** Get the source code for the command.
	*/

	public String getSource()
	{
		return "COLOUR " + m_strClr + "\n";
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	private Color  m_clrPen;
	private String m_strClr;
}
