import java.awt.*;

/********************************************************************************
** This panel is used to host the program control options.
*/

public class CtrlPanel extends Panel
{
	/********************************************************************************
	** Constructor.
	*/

	public CtrlPanel(Turtle oTurtle, Display oDisplay, Program oProgram)
	{
		super(new FlowLayout(FlowLayout.LEFT, 5, 5));

		// Save parameters.
		m_oTurtle  = oTurtle;
		m_oDisplay = oDisplay;
		m_oProgram = oProgram;

		// Create the child panels.
		m_pnlProgram    = new ProgramPanel(oTurtle, oDisplay, oProgram);
		m_pnlTurtleSts  = new TurtleStatusPanel(oTurtle);
		m_pnlDisplaySts = new DisplayStatusPanel(oDisplay);

		// Create this panel.
		setBackground(SystemColor.control);
		add(m_pnlProgram);
		add(m_pnlTurtleSts);
		add(m_pnlDisplaySts);
	}

	/********************************************************************************
	** Constants
	*/

	/********************************************************************************
	** Members.
	*/

	// Child panels.
	private ProgramPanel		m_pnlProgram;
	private TurtleStatusPanel	m_pnlTurtleSts;
	private DisplayStatusPanel	m_pnlDisplaySts;

	// Data members.
	private Turtle	m_oTurtle;
	private Display	m_oDisplay;
	private Program	m_oProgram;
}
