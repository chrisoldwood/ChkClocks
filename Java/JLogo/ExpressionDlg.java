import java.awt.*;

/********************************************************************************
** This dialog is used to enter an expression.
*/

public class ExpressionDlg extends ModalDialog
{
	/********************************************************************************
	** Constructor.
	*/

	public ExpressionDlg(Component oParent, String strExpr)
	{
		super(oParent, "Enter An Expression", OK_CANCEL);

		// Create the dialog panel.
		m_pnlControls.add(BorderLayout.CENTER, m_ebExpr);

		// Display the source code.
		m_ebExpr.setText(strExpr);

		setResizable(true);
	}

	/********************************************************************************
	** OK pressed.
	*/

	public void onOK()
	{
		m_strExpr = m_ebExpr.getText();

		// Validate expression, if one supplied.
		if (m_strExpr.length() > 0)
		{
		}

		super.onOK();
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// Data members.
	public	String m_strExpr = "";

	// GUI members.
	private TextField m_ebExpr = new TextField(50);
}
