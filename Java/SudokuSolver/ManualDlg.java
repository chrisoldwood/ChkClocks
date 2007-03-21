import java.awt.*;
import gort.ui.*;

/********************************************************************************
** This dialog is used to display the manual.
*/

public class ManualDlg extends ModalDialog
{
	/********************************************************************************
	** Constructor.
	*/

	public ManualDlg(Component oParent, String strManual)
	{
		super(oParent, "Sudoku Solver Manual", CLOSE);

		// Create the dialog panel.
		m_pnlControls.add(BorderLayout.CENTER, m_ebManual);
		setResizable(true);

		// Display the manual.
		m_ebManual.setFont(new Font("Monospaced", Font.PLAIN, 12));
		m_ebManual.setText(strManual);
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// GUI members.
	private TextArea	m_ebManual = new TextArea("", 20, 60, TextArea.SCROLLBARS_BOTH);
}
