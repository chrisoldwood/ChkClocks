import java.awt.*;

/********************************************************************************
** This dialog is used to display the source code.
*/

public class SourceDlg extends ModalDialog
{
	/********************************************************************************
	** Constructor.
	*/

	public SourceDlg(Component oParent, String strSource)
	{
		super(oParent, "Source Code", CLOSE);

		// Create the dialog panel.
		m_pnlControls.add(BorderLayout.CENTER, m_ebSource);

		// Display the source code.
		m_ebSource.setText(strSource);
		m_ebSource.setEditable(false);
		m_ebSource.setBackground(Color.white);

		setResizable(true);
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// GUI members.
	private TextArea m_ebSource = new TextArea("", 25, 50, TextArea.SCROLLBARS_BOTH);
}
