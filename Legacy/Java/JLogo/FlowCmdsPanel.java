import java.awt.*;
import java.awt.event.*;
import java.util.*;
import gort.ui.*;

/********************************************************************************
** This panel is used to host the flow control commands controls.
*/

public class FlowCmdsPanel extends GroupPanel
	implements ActionListener
{
	/********************************************************************************
	** Constructor.
	*/

	public FlowCmdsPanel(Program oProgram, ExecContext oEditCtx)
	{
		super("Flow Commands", GroupPanel.CENTER);

		// Save parameters.
		m_oProgram = oProgram;
		m_oEditCtx = oEditCtx;

		// Create this panel.
		setBackground(SystemColor.control);
		add(m_pnlFields);

		// Create the child panels.
		m_pnlRptBtns.add(BorderLayout.WEST, m_btnRepeat);
		m_pnlRptBtns.add(BorderLayout.EAST, m_btnEndRepeat);

		m_pnlFields.add(m_pnlRptBtns, m_ebRepeat, m_btnReptExpr);

		// Setup the event handlers.
		m_btnRepeat.addActionListener(this);
		m_btnReptExpr.addActionListener(this);
		m_btnEndRepeat.addActionListener(this);
		m_oProgram.addActionListener(this);

		// Initialise buttons.
		m_btnEndRepeat.setEnabled(false); 
	}

	/********************************************************************************
	** Button pressed.
	*/

	public void actionPerformed(ActionEvent eEvent)
	{
		try
		{
			if (eEvent.getSource() == m_btnRepeat)
				onRepeat();
			else if (eEvent.getSource() == m_btnReptExpr)
				onExpression(m_ebRepeat);
			else if (eEvent.getSource() == m_btnEndRepeat)
				onEndRepeat();
			else if (eEvent.getSource() == m_oProgram)
				onProgramEvent(eEvent);
		}
		catch(InvalidParamException ipe)
		{
			MsgBox.alert(this, "JLogo", ipe.getMessage());
			ipe.setFocus();
		}
		catch(Exception e)
		{
			e.printStackTrace();

			MsgBox.alert(this, "JLogo", e.toString());
		}
	}

	/********************************************************************************
	** Begin loop.
	*/

	public void onRepeat() throws InvalidParamException
	{
		if (m_ebRepeat.getText().length() == 0)
			throw new InvalidParamException("Please specify the loop count.", m_ebRepeat);

		try
		{
			String    strExpr = m_ebRepeat.getText();
			int       nCount  = (int) m_oEditCtx.getExprParser().evaluate(strExpr);
			RepeatCmd oCmd    = new RepeatCmd(strExpr);

			if (nCount < 1)
				throw new InvalidParamException("The loop count must exceed 0.", m_ebRepeat);

			m_oProgram.add(oCmd);
			m_vLoops.addElement(oCmd);

			m_btnEndRepeat.setEnabled(true);
		}
		catch(ExpressionException e)
		{
			throw new InvalidParamException("The loop count is invalid.", m_ebRepeat);
		}
	}

	/********************************************************************************
	** End loop.
	*/

	public void onEndRepeat()
	{
		RepeatCmd oCmd = (RepeatCmd) m_vLoops.pop();

		// Complete the other loop iterations.
		oCmd.executeEndRepeat(m_oEditCtx);

		m_oProgram.add(new EndCmd());

		if (m_vLoops.size() == 0)
			m_btnEndRepeat.setEnabled(false);
	}

	/********************************************************************************
	** Program event occurred.
	*/

	public void onProgramEvent(ActionEvent eEvent)
	{
		String strEvent = eEvent.getActionCommand();

		// Old program deleted?
		if (strEvent.equals(Program.CLEARED))
		{
			// Reset controls.
			m_btnEndRepeat.setEnabled(false);

			m_vLoops.removeAllElements();
		}
	}

	/********************************************************************************
	** Show dialog to allow an expression to be entered.
	*/

	public void onExpression(TextField ebField)
	{
		ExpressionDlg Dlg = new ExpressionDlg(this, ebField.getText());

		if (Dlg.prompt() == Dlg.OK)
			ebField.setText(Dlg.m_strExpr);
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// Child panels.
	private Panel		m_pnlRptBtns = new Panel(new BorderLayout());
	private TriColPanel m_pnlFields  = new TriColPanel();

	// Child controls.
	private Button		m_btnRepeat    = new Button("Repeat");
	private Button		m_btnEndRepeat = new Button("End");
	private TextField	m_ebRepeat     = new TextField("4", 3);
	private Button		m_btnReptExpr  = new Button("...");

	// Data members.
	private Program		m_oProgram;
	private ExecContext	m_oEditCtx;
	private Stack		m_vLoops = new Stack();
}
