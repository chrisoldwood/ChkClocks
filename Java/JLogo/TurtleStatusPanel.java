import java.awt.*;
import java.awt.event.*;
import gort.ui.*;

/********************************************************************************
** This panel is used to host the turtle status contols.
*/

public class TurtleStatusPanel extends GroupPanel
	implements ActionListener, ItemListener
{
	/********************************************************************************
	** Constructor.
	*/

	public TurtleStatusPanel(Turtle oTurtle)
	{
		super("Turtle Status", GroupPanel.CENTER, new BorderLayout(5, 5));

		// Save parameters.
		m_oTurtle  = oTurtle;

		// Create this panel.
		setBackground(SystemColor.control);
		add(BorderLayout.WEST, m_pnlFields1);
		add(BorderLayout.EAST, m_pnlFields2);

		// Create the child panels.
		m_pnlFields1.add(m_lblXPos,  m_ebXPos);
		m_pnlFields1.add(m_lblYPos,  m_ebYPos);
		m_pnlFields1.add(m_lblAngle, m_ebAngle);

		m_pnlFields2.add(m_lblState,   m_ebState);
		m_pnlFields2.add(m_lblColour,  m_ebColour);
		m_pnlFields2.add(m_lblVisible, m_ckVisible);

		// Make status fields read-only.
		m_ebXPos.setEditable(false);
		m_ebYPos.setEditable(false);
		m_ebAngle.setEditable(false);
		m_ebState.setEditable(false);
		m_ebColour.setEditable(false);
		m_ckVisible.addItemListener(this);

		// Add event handlers.
		m_oTurtle.addActionListener(this);
	}

	/********************************************************************************
	** Action event handler.
	*/

	public void actionPerformed(ActionEvent eEvent)
	{
		// From the turtle?
		if (eEvent.getSource() == m_oTurtle)
		{
			String strEvent = eEvent.getActionCommand();

			if (strEvent.equals(Turtle.MOVED))
			{
				m_ebXPos.setText(String.valueOf(m_oTurtle.getXPos()));
				m_ebYPos.setText(String.valueOf(m_oTurtle.getYPos()));
			}

			if (strEvent.equals(Turtle.ROTATED))
			{
				m_ebAngle.setText(String.valueOf(m_oTurtle.getAngle()));
			}

			if (strEvent.equals(Turtle.PENSTATE))
			{
				m_ebState.setText(m_oTurtle.getPenDown() ? "Down" : "Up");
			}

			if (strEvent.equals(Turtle.PENCOLOR))
			{
				m_ebColour.setText(m_oTurtle.getPenColour());
			}

			if (strEvent.equals(Turtle.VISIBILITY))
			{
				m_ckVisible.setState(m_oTurtle.isVisible());
			}
		}
	}

	/********************************************************************************
	** Item state event handler.
	*/

	public void itemStateChanged(ItemEvent eEvent)
	{
		if (eEvent.getSource() == m_ckVisible)
			onTurtleVisible();
	}

	/********************************************************************************
	** Visible state changed.
	*/

	public void	onTurtleVisible()
	{
		m_oTurtle.showTurtle(!m_oTurtle.isVisible());
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// Child panels.
	private DualColPanel m_pnlFields1  = new DualColPanel();
	private DualColPanel m_pnlFields2  = new DualColPanel();

	// Child controls.
	private Label		m_lblXPos  = new Label("X:", Label.RIGHT);
	private TextField	m_ebXPos   = new TextField(3);
	private Label		m_lblYPos  = new Label("Y:", Label.RIGHT);
	private TextField	m_ebYPos   = new TextField(3);
	private Label		m_lblAngle = new Label("Angle:", Label.RIGHT);
	private TextField	m_ebAngle  = new TextField(3);

	private Label		m_lblState   = new Label("Pen State:", Label.RIGHT);
	private TextField	m_ebState    = new TextField(4);
	private Label		m_lblColour  = new Label("Pen Color:", Label.RIGHT);
	private TextField	m_ebColour   = new TextField(4);
	private Label		m_lblVisible = new Label("Visible?", Label.RIGHT);
	private Checkbox 	m_ckVisible  = new Checkbox();

	// Data members.
	private Turtle		m_oTurtle;
}
