package gort.ui;

import java.awt.*;

/********************************************************************************
** This panel derived class has three vertical child panels, a left, a centre and
** a right.
*/

public class TriColPanel extends Panel
{
	/********************************************************************************
	** Constructor.
	*/

	public TriColPanel()
	{
		super(new BorderLayout(5, 5));

		// Add the two child panels.
		add(BorderLayout.WEST,   m_pnlLeft);
		add(BorderLayout.CENTER, m_pnlCentre);
		add(BorderLayout.EAST,   m_pnlRight);
	}

	/********************************************************************************
	** Add a componet to each of the left, cente and right child panels.
	*/

	public void add(Component cmpLeft, Component cmpCentre, Component cmpRight)
	{
		m_pnlLeft.add (cmpLeft);
		m_pnlCentre.add(cmpCentre);
		m_pnlRight.add(cmpRight);
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// Child panels.
	private Panel	m_pnlLeft   = new Panel(new GridLayout(0, 1, 5, 5));
	private Panel	m_pnlCentre = new Panel(new GridLayout(0, 1, 5, 5));
	private Panel	m_pnlRight  = new Panel(new GridLayout(0, 1, 5, 5));
}
