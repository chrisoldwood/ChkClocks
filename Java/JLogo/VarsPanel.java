import java.awt.*;
import java.awt.event.*;
import gort.ui.*;

/********************************************************************************
** This panel is used to host the variables controls.
*/

public class VarsPanel extends GroupPanel
	implements ActionListener, ItemListener
{
	/********************************************************************************
	** Constructor.
	*/

	public VarsPanel(Turtle oTurtle, Program oProgram, ExecContext oEditCtx)
	{
		super("Variables", new BorderLayout(5, 10));

		// Save parameters.
		m_oTurtle  = oTurtle;
		m_oProgram = oProgram;
		m_oEditCtx = oEditCtx;

		// Create this panel.
		setBackground(SystemColor.control);
		add(BorderLayout.NORTH,  m_pnlFields1);
		add(BorderLayout.CENTER, m_lbVars);
		add(BorderLayout.SOUTH,  m_pnlFields2);

		// Create the child panels.
		m_pnlFields1.add(m_btnAdd,    m_ebAdd,    m_lblEmpty1);
		m_pnlFields1.add(m_lblEmpty3, m_ebAddVal, m_btnAddValExpr);
		m_pnlFields2.add(m_lblCurVal, m_ebCurVal, m_lblEmpty2);
		m_pnlFields2.add(m_btnSet,    m_ebSetVal, m_btnSetValExpr);

		// Setup the event handlers.
		m_lbVars.addItemListener(this);
		m_btnAdd.addActionListener(this);
		m_btnAddValExpr.addActionListener(this);
		m_btnSet.addActionListener(this);
		m_btnSetValExpr.addActionListener(this);
		m_oProgram.addActionListener(this);

		// Initialise buttons.
		m_btnSet.setEnabled(false); 
		m_ebCurVal.setEditable(false);
	}

	/********************************************************************************
	** Listbox selection changed.
	*/

	public void itemStateChanged(ItemEvent eEvent)
	{
		if (eEvent.getSource() == m_lbVars)
			onSelectVar();
	}

	/********************************************************************************
	** Variables listbox selection changed.
	*/

	public void onSelectVar()
	{
		String strSel = m_lbVars.getSelectedItem();
		double dValue = m_oEditCtx.getVariables().get(strSel);
		
		m_ebCurVal.setText(String.valueOf(dValue));
		m_ebSetVal.setText("");
	}

	/********************************************************************************
	** Button pressed.
	*/

	public void actionPerformed(ActionEvent eEvent)
	{
		try
		{
			if (eEvent.getSource() == m_btnAdd)
				onAdd();
			else if (eEvent.getSource() == m_btnAddValExpr)
				onExpression(m_ebAddVal);
			else if (eEvent.getSource() == m_btnSet)
				onSet();
			else if (eEvent.getSource() == m_btnSetValExpr)
				onExpression(m_ebSetVal);
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
	** Add variable action.
	*/

	public void onAdd() throws InvalidParamException
	{
		if (m_ebAdd.getText().length() == 0)
			throw new InvalidParamException("Please specify the variables' name.", m_ebAdd);

		if (m_ebAddVal.getText().length() == 0)
			throw new InvalidParamException("Please specify the variables' initial value.", m_ebAddVal);

		try
		{
			String    strName = m_ebAdd.getText();
			String    strExpr = m_ebAddVal.getText();
			Variables oVars   = m_oEditCtx.getVariables();
			AddVarCmd oCmd    = new AddVarCmd(strName, strExpr);

			if (!oVars.isValidName(strName))
				throw new InvalidParamException("The variable name is invalid.", m_ebAdd);

			if (oVars.exists(strName))
				throw new InvalidParamException("The variable name is already in use.", m_ebAdd);

			oCmd.execute(m_oEditCtx);
			m_oProgram.add(oCmd);

			m_lbVars.add(strName);
			m_lbVars.select(m_lbVars.getItemCount()-1);

			m_btnSet.setEnabled(true);

			onSelectVar();
		}
		catch(ExpressionException e)
		{
			throw new InvalidParamException("The value is invalid.", m_ebAddVal);
		}
	}

	/********************************************************************************
	** Set variable action.
	*/

	public void onSet() throws InvalidParamException
	{
		if (m_ebSetVal.getText().length() == 0)
			throw new InvalidParamException("Please specify the variables' value.", m_ebSetVal);

		try
		{
			String    strExpr = m_ebSetVal.getText();
			String    strName = m_lbVars.getSelectedItem();
			SetVarCmd oCmd    = new SetVarCmd(strName, strExpr);

			oCmd.execute(m_oEditCtx);
			m_oProgram.add(oCmd);

			onSelectVar();
		}
		catch(ExpressionException e)
		{
			throw new InvalidParamException("The value is invalid.", m_ebSetVal);
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
	** Program event occurred.
	*/

	public void onProgramEvent(ActionEvent eEvent)
	{
		String strEvent = eEvent.getActionCommand();

		// Old program deleted?
		if (strEvent.equals(Program.CLEARED))
		{
			// Reset controls.
			m_ebAdd.setText("");
			m_ebAddVal.setText("");
			m_lbVars.removeAll();
			m_ebCurVal.setText("");
			m_ebSetVal.setText("");
			m_btnSet.setEnabled(false);
		}
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// Child panels.
	private TriColPanel m_pnlFields1 = new TriColPanel();
	private TriColPanel m_pnlFields2 = new TriColPanel();

	// Child controls.
	private Button		m_btnAdd     = new Button("  Add  ");
	private TextField	m_ebAdd      = new TextField("", 3);
	private Label 		m_lblEmpty1  = new Label();

	private Label 		m_lblEmpty3     = new Label(" = ");
	private TextField	m_ebAddVal      = new TextField("", 3);
	private Button		m_btnAddValExpr = new Button("...");

	private List m_lbVars = new List();

	private Label 		m_lblCurVal  = new Label("Value:", Label.CENTER);
	private TextField	m_ebCurVal   = new TextField("", 3);
	private Label 		m_lblEmpty2  = new Label();

	private Button		m_btnSet        = new Button("  Set  ");
	private TextField	m_ebSetVal      = new TextField("", 3);
	private Button		m_btnSetValExpr = new Button("...");

	// Data members.
	private Turtle		m_oTurtle;
	private Program		m_oProgram;
	private ExecContext	m_oEditCtx;
}
