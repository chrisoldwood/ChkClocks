import java.awt.*;
import java.awt.event.*;
import java.util.*;

/********************************************************************************
** This panel is used to host the commands controls.
*/

public class CmdsPanel extends GroupPanel
	implements ActionListener
{
	/********************************************************************************
	** Constructor.
	*/

	public CmdsPanel(Turtle oTurtle, Program oProgram, ExecContext oEditCtx)
	{
		super("Commands", Label.CENTER, new FlowLayout(FlowLayout.CENTER));

		// Save parameters.
		m_oTurtle  = oTurtle;
		m_oProgram = oProgram;
		m_oEditCtx = oEditCtx;

		// Create this panel.
		setBackground(SystemColor.control);
		add(m_pnlFields);

		// Create the child panels.
		m_pnlFields.add(m_btnPenState,  m_cbPenState, m_lblEmpty1);
		m_pnlFields.add(m_btnPenClr,    m_cbPenClr,   m_lblEmpty2);
		m_pnlFields.add(m_btnForward,   m_ebForward,  m_btnFwdExpr);
		m_pnlFields.add(m_btnRight,     m_ebRight,    m_btnRgtExpr);
		m_pnlFields.add(m_btnLeft,      m_ebLeft,     m_btnLftExpr);
		m_pnlFields.add(m_lblBlank1,    m_lblBlank2,  m_lblBlank3);
		m_pnlFields.add(m_btnRepeat,    m_ebRepeat,   m_btnEndRepeat);

		// Fill the pen combos.
		for (int i = 0; i < s_astrStates.length; i++)
			m_cbPenState.add(s_astrStates[i]);

		for (int i = 0; i < s_astrColours.length; i++)
			m_cbPenClr.add(s_astrColours[i]);

		// Setup the event handlers.
		m_btnPenState.addActionListener(this);
		m_btnPenClr.addActionListener(this);
		m_btnForward.addActionListener(this);
		m_btnFwdExpr.addActionListener(this);
		m_btnRight.addActionListener(this);
		m_btnRgtExpr.addActionListener(this);
		m_btnLeft.addActionListener(this);
		m_btnLftExpr.addActionListener(this);
		m_btnRepeat.addActionListener(this);
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
			if (eEvent.getSource() == m_btnPenState)
				onPenState();
			else if (eEvent.getSource() == m_btnPenClr)
				onPenClr();
			else if (eEvent.getSource() == m_btnForward)
				onForward();
			else if (eEvent.getSource() == m_btnFwdExpr)
				onExpression(m_ebForward);
			else if (eEvent.getSource() == m_btnRight)
				onRight();
			else if (eEvent.getSource() == m_btnRgtExpr)
				onExpression(m_ebRight);
			else if (eEvent.getSource() == m_btnLeft)
				onLeft();
			else if (eEvent.getSource() == m_btnLftExpr)
				onExpression(m_ebLeft);
			else if (eEvent.getSource() == m_btnRepeat)
				onRepeat();
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
	** Pen state action.
	*/

	public void onPenState() throws InvalidParamException
	{
		try
		{
			String      strExpr = m_cbPenState.getSelectedItem();
			PenStateCmd oCmd    = new PenStateCmd(strExpr);

			oCmd.execute(m_oEditCtx);
			m_oProgram.add(oCmd);
		}
		catch(ExpressionException e)
		{
			throw new InvalidParamException("The pen state is invalid.", m_cbPenState);
		}
	}

	/********************************************************************************
	** Pen Colour action.
	*/

	public void onPenClr() throws InvalidParamException
	{
		try
		{
			String      strExpr = m_cbPenClr.getSelectedItem();
			PenColorCmd oCmd    = new PenColorCmd(strExpr);

			oCmd.execute(m_oEditCtx);
			m_oProgram.add(oCmd);
		}
		catch(ExpressionException e)
		{
			throw new InvalidParamException("The colour is invalid.", m_cbPenClr);
		}
	}

	/********************************************************************************
	** Forward action.
	*/

	public void onForward() throws InvalidParamException
	{
		if (m_ebForward.getText().length() == 0)
			throw new InvalidParamException("Please specify the distance.", m_ebForward);

		try
		{
			String     strExpr = m_ebForward.getText();
			ForwardCmd oCmd    = new ForwardCmd(strExpr);

			oCmd.execute(m_oEditCtx);
			m_oProgram.add(oCmd);
		}
		catch(ExpressionException e)
		{
			throw new InvalidParamException("The distance is invalid.\n\n" + e.getMessage(), m_ebForward);
		}
	}

	/********************************************************************************
	** Turn right action.
	*/

	public void onRight() throws InvalidParamException
	{
		if (m_ebRight.getText().length() == 0)
			throw new InvalidParamException("Please specify the angle.", m_ebRight);

		try
		{
			String       strExpr = m_ebRight.getText();
			TurnRightCmd oCmd    = new TurnRightCmd(strExpr);

			oCmd.execute(m_oEditCtx);
			m_oProgram.add(oCmd);
		}
		catch(ExpressionException e)
		{
			throw new InvalidParamException("The angle is invalid.", m_ebRight);
		}
	}

	/********************************************************************************
	** Turn left action.
	*/

	public void onLeft() throws InvalidParamException
	{
		if (m_ebLeft.getText().length() == 0)
			throw new InvalidParamException("Please specify the angle.", m_ebLeft);

		try
		{
			String      strExpr = m_ebLeft.getText();
			TurnLeftCmd oCmd    = new TurnLeftCmd(strExpr);

			oCmd.execute(m_oEditCtx);
			m_oProgram.add(oCmd);
		}
		catch(ExpressionException e)
		{
			throw new InvalidParamException("The angle is invalid.", m_ebLeft);
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

		m_oProgram.add(new EndRepeatCmd());

		if (m_vLoops.size() == 0)
			m_btnEndRepeat.setEnabled(false);
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
			m_cbPenState.select(0);
			m_cbPenClr.select(0);
			m_ebForward.setText("50");
			m_ebRight.setText("90");
			m_ebLeft.setText("90");
			m_vLoops.removeAllElements();
			m_btnEndRepeat.setEnabled(false);
		}
	}

	/********************************************************************************
	** Constants.
	*/

	public static final String[] s_astrStates  = new String[]
	{ "Down", "Up" };
	public static final String[] s_astrColours = new String[]
	{ "Black", "Red", "Green", "Blue", "White", "Cyan", "Magenta", "Yellow"};

	/********************************************************************************
	** Members.
	*/

	// Child panels.
	private TriColPanel m_pnlFields  = new TriColPanel();

	// Child controls.
	private Button		m_btnPenState = new Button("Pen State");
	private Choice		m_cbPenState  = new Choice();
	private Label 		m_lblEmpty1   = new Label();

	private Button		m_btnPenClr   = new Button("Pen Colour");
	private Choice		m_cbPenClr    = new Choice();
	private Label 		m_lblEmpty2   = new Label();

	private Button		m_btnForward  = new Button("Forward");
	private TextField	m_ebForward   = new TextField("50", 3);
	private Button		m_btnFwdExpr  = new Button("...");

	private Button		m_btnRight    = new Button("Turn Right");
	private TextField	m_ebRight     = new TextField("90", 3);
	private Button		m_btnRgtExpr  = new Button("...");

	private Button		m_btnLeft     = new Button("Turn Left");
	private TextField	m_ebLeft      = new TextField("90", 3);
	private Button		m_btnLftExpr  = new Button("...");

	private Label 		m_lblBlank1   = new Label();
	private Label		m_lblBlank2   = new Label();
	private Label		m_lblBlank3   = new Label();

	private Button		m_btnRepeat    = new Button("Repeat");
	private TextField	m_ebRepeat     = new TextField("4", 3);
	private Button		m_btnEndRepeat = new Button("End");

	// Data members.
	private Turtle		m_oTurtle;
	private Program		m_oProgram;
	private ExecContext	m_oEditCtx;
	private Stack		m_vLoops = new Stack();
}
