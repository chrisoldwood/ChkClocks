package gort.ui;

import java.awt.*;

/********************************************************************************
** This is a panel derived class used to host a serious of buttons.
*/

public class ButtonPanel extends Panel
{
	/********************************************************************************
	** Constructor.
	*/

	public ButtonPanel()
	{
		// Create this panel.
		setLayout(m_lmLayout);
		add(m_pnlButtons);
	}

	/********************************************************************************
	** Sets the FlowLayout alignment of the buttons.
	*/

	public void setAlignment(int nAlignment)
	{
		m_lmLayout.setAlignment(nAlignment);
	}

	/********************************************************************************
	** Adds a button to the panel.
	*/

	public void add(Button oButton)
	{
		m_pnlButtons.add(oButton);
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	protected Panel			m_pnlButtons = new Panel(new GridLayout(1, 0, 5, 5));
	protected FlowLayout	m_lmLayout   = new FlowLayout(FlowLayout.CENTER, 5, 5);
}
