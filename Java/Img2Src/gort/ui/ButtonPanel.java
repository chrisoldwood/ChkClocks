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
		super(new FlowLayout(FlowLayout.CENTER, 5, 5));

		// Create this panel.
		add(m_pnlButtons);
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

	protected Panel	m_pnlButtons = new Panel(new GridLayout(1, 0, 5, 5));
}
