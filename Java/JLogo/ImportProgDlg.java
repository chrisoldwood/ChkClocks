import java.awt.*;

/********************************************************************************
** This dialog is used to import the source code.
*/

public class ImportProgDlg extends ModalDialog
{
	/********************************************************************************
	** Constructor.
	*/

	public ImportProgDlg(Component oParent)
	{
		super(oParent, "Import Program", OK_CANCEL);

		// Create the dialog panel.
		m_pnlControls.add(BorderLayout.NORTH,  m_lblHelp);
		m_pnlControls.add(BorderLayout.CENTER, m_ebSource);

		setResizable(true);
	}

	/********************************************************************************
	** OK pressed.
	*/

	public void onOK()
	{
		m_strSource = m_ebSource.getText();

		super.onOK();
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// GUI members.
	private Label		m_lblHelp  = new Label("Paste the source code below", Label.CENTER);
	private TextArea	m_ebSource = new TextArea("", 25, 50, TextArea.SCROLLBARS_BOTH);

	// Data members.
	public String	m_strSource = "";
}
