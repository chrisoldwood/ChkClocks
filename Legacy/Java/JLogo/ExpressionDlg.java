import java.awt.*;
import gort.ui.*;

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
		this(oParent, "Enter An Expression", strExpr);
	}

	/********************************************************************************
	** Full Constructor.
	*/

	public ExpressionDlg(Component oParent, String strTitle, String strExpr)
	{
		super(oParent, strTitle, OK_CANCEL);

		// Create the dialog panel.
		m_pnlControls.add(BorderLayout.CENTER, m_ebExpr);

		// Display the source code.
		m_ebExpr.setText(strExpr);
		m_ebExpr.setFont(new Font("Monospaced", Font.PLAIN, 12));

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
