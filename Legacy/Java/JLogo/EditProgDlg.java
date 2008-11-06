import java.awt.*;
import gort.ui.*;

/********************************************************************************
** This dialog is used to edit the source code.
*/

public class EditProgDlg extends ModalDialog
{
	/********************************************************************************
	** Constructor.
	*/

	public EditProgDlg(Component oParent, String strSource)
	{
		super(oParent, "Edit Program", OK_CANCEL);

		// Save parameters.
		m_strSource = strSource;

		// Create the dialog panel.
		m_pnlControls.add(BorderLayout.CENTER, m_ebSource);
		setResizable(true);

		// Load the existing source.
		m_ebSource.setText(m_strSource);
		m_ebSource.setFont(new Font("Monospaced", Font.PLAIN, 12));
	}

	/********************************************************************************
	** OK pressed.
	*/

	public void onOK()
	{
		String str = m_ebSource.getText();

		// Source code changed?
		if (!m_strSource.equals(str))
		{
			m_strSource = str;
			super.onOK();
		}
		else
		{
			super.onCancel();
		}	
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// GUI members.
	private TextArea m_ebSource = new TextArea("", 25, 50, TextArea.SCROLLBARS_BOTH);

	// Data members.
	public String	m_strSource = "";
}
