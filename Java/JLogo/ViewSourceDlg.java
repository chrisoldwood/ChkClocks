import java.awt.*;

/********************************************************************************
** This dialog is used to display the source code.
*/

public class ViewSourceDlg extends ModalDialog
{
	/********************************************************************************
	** Constructor.
	*/

	public ViewSourceDlg(Component oParent, SourceLines oLines)
	{
		super(oParent, "Source Code", CLOSE);

		// Create the dialog panel.
		m_pnlControls.add(BorderLayout.CENTER, m_ebSource);

		m_ebSource.setEditable(false);
		m_ebSource.setBackground(Color.white);

		setResizable(true);

		// Fetch the source code text and display.
		StringBuffer strSrc = new StringBuffer(256);

		for(int i = 0; i < oLines.count(); i++)
			strSrc.append(oLines.getIndentedLine(i) + "\n");

		m_ebSource.setText(strSrc.toString());
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
