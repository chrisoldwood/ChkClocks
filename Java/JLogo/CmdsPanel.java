import java.awt.*;
import java.awt.event.*;

/********************************************************************************
** This panel is used to host the commands options.
*/

public class CmdsPanel extends GroupPanel
	implements ActionListener
{
	/********************************************************************************
	** Constructor.
	*/

	public CmdsPanel(Turtle oTurtle, Program oProgram)
	{
		super("Commands", Label.CENTER, new FlowLayout(FlowLayout.CENTER));

		// Save parameters.
		m_oTurtle  = oTurtle;
		m_oProgram = oProgram;

		// Create this panel.
		setBackground(SystemColor.control);
		add(m_pnlFields);

		// Create the child panels.
		m_pnlFields.add(m_btnPenState,  m_cbPenState);
		m_pnlFields.add(m_btnPenClr,    m_cbPenClr);
		m_pnlFields.add(m_btnForward,   m_ebForward);
		m_pnlFields.add(m_btnRight,     m_ebRight);
		m_pnlFields.add(m_btnLeft,      m_ebLeft);
		m_pnlFields.add(m_lblBlank1,    m_lblBlank2);
		m_pnlFields.add(m_btnRepeat,    m_ebRepeat);
		m_pnlFields.add(m_btnEndRepeat, m_lblBlank3);

		// Fill the pen combos.
		for (int i = 0; i < s_astrStates.length; i++)
			m_cbPenState.add(s_astrStates[i]);

		for (int i = 0; i < s_astrColours.length; i++)
			m_cbPenClr.add(s_astrColours[i]);

		// Setup the event handlers.
		m_btnPenState.addActionListener(this);
		m_btnPenClr.addActionListener(this);
		m_btnForward.addActionListener(this);
		m_btnRight.addActionListener(this);
		m_btnLeft.addActionListener(this);
		m_btnRepeat.addActionListener(this);
		m_btnEndRepeat.addActionListener(this);

		// Set disabled buttons.
		m_btnEndRepeat.setEnabled(false); 
	}

	/********************************************************************************
	** Button pressed.
	*/

	public void actionPerformed(ActionEvent eEvent)
	{
		if (eEvent.getSource() == m_btnPenState)
			onPenState();
		else if (eEvent.getSource() == m_btnPenClr)
			onPenClr();
		else if (eEvent.getSource() == m_btnForward)
			onForward();
		else if (eEvent.getSource() == m_btnRight)
			onRight();
		else if (eEvent.getSource() == m_btnLeft)
			onLeft();
		else if (eEvent.getSource() == m_btnRepeat)
			onRepeat();
		else if (eEvent.getSource() == m_btnEndRepeat)
			onEndRepeat();
	}

	/********************************************************************************
	** Pen state action.
	*/

	public void onPenState()
	{
		boolean bDown = m_cbPenState.getSelectedIndex() == 0;

		m_oTurtle.setPenDown(bDown);
		m_oProgram.add(new PenStateCmd(bDown));
	}

	/********************************************************************************
	** Pen Colour action.
	*/

	public void onPenClr()
	{
		Color  clrPen = s_aclrColours[m_cbPenClr.getSelectedIndex()];
		String strClr = m_cbPenClr.getSelectedItem();

		m_oTurtle.setPenColour(clrPen, strClr);
		m_oProgram.add(new PenColorCmd(clrPen, strClr));
	}

	/********************************************************************************
	** Forward action.
	*/

	public void onForward()
	{
		if (m_ebForward.getText().length() == 0)
		{
			MsgBox.notify(this, "JLogo", "Please specify the distance.");
			m_ebForward.requestFocus();
			return;
		}

		try
		{
			int nDistance = Integer.parseInt(m_ebForward.getText());

			m_oTurtle.forward(nDistance);
			m_oProgram.add(new ForwardCmd(nDistance));
		}
		catch(NumberFormatException e)
		{
			MsgBox.alert(this, "JLogo", "The distance is invalid.");
			m_ebForward.requestFocus();
		}
	}

	/********************************************************************************
	** Turn right action.
	*/

	public void onRight()
	{
		if (m_ebRight.getText().length() == 0)
		{
			MsgBox.notify(this, "JLogo", "Please specify the angle.");
			m_ebRight.requestFocus();
			return;
		}

		try
		{
			int nDegrees = Integer.parseInt(m_ebRight.getText());

			m_oTurtle.turnRight(nDegrees);
			m_oProgram.add(new TurnRightCmd(nDegrees));
		}
		catch(NumberFormatException e)
		{
			MsgBox.alert(this, "JLogo", "The angle is invalid.");
			m_ebRight.requestFocus();
		}
	}

	/********************************************************************************
	** Turn left action.
	*/

	public void onLeft()
	{
		if (m_ebLeft.getText().length() == 0)
		{
			MsgBox.notify(this, "JLogo", "Please specify the angle.");
			m_ebLeft.requestFocus();
			return;
		}

		try
		{
			int nDegrees = Integer.parseInt(m_ebLeft.getText());

			m_oTurtle.turnLeft(nDegrees);
			m_oProgram.add(new TurnLeftCmd(nDegrees));
		}
		catch(NumberFormatException e)
		{
			MsgBox.alert(this, "JLogo", "The angle is invalid.");
			m_ebLeft.requestFocus();
		}
	}

	/********************************************************************************
	** Begin loop.
	*/

	public void onRepeat()
	{
		if (m_ebRepeat.getText().length() == 0)
		{
			MsgBox.notify(this, "JLogo", "Please specify the loop count.");
			m_ebRepeat.requestFocus();
			return;
		}

		try
		{
			int nCount = Integer.parseInt(m_ebRepeat.getText());

			m_oProgram.add(new RepeatCmd(nCount));

			m_nLoops++;
			m_btnEndRepeat.setEnabled(true);
		}
		catch(NumberFormatException e)
		{
			MsgBox.alert(this, "JLogo", "The loop count is invalid.");
			m_ebRepeat.requestFocus();
		}
	}

	/********************************************************************************
	** End loop.
	*/

	public void onEndRepeat()
	{
		m_oProgram.add(new EndRepeatCmd());

		if ( (--m_nLoops) == 0)
			m_btnEndRepeat.setEnabled(false);
	}

	/********************************************************************************
	** Constants.
	*/

	public static final String[] s_astrStates  = new String[] { "Down", "Up" };
	public static final String[] s_astrColours = new String[] { "Black",     "Red",     "Green" };
	public static final Color[]  s_aclrColours = new Color[]  { Color.black, Color.red, Color.green };

	/********************************************************************************
	** Members.
	*/

	// Child panels.
	private DualColPanel m_pnlFields  = new DualColPanel();

	// Child controls.
	private Button		m_btnPenState = new Button("Pen State");
	private Choice		m_cbPenState  = new Choice();
	private Button		m_btnPenClr   = new Button("Pen Colour");
	private Choice		m_cbPenClr    = new Choice();
	private Button		m_btnForward  = new Button("Forward");
	private TextField	m_ebForward   = new TextField("50", 3);
	private Button		m_btnRight    = new Button("Turn Right");
	private TextField	m_ebRight     = new TextField("90", 3);
	private Button		m_btnLeft     = new Button("Turn Left");
	private TextField	m_ebLeft      = new TextField("90", 3);

	private Label 		m_lblBlank1   = new Label();
	private Label		m_lblBlank2   = new Label();

	private Button		m_btnRepeat    = new Button("Repeat");
	private TextField	m_ebRepeat     = new TextField("4", 3);
	private Button		m_btnEndRepeat = new Button("End Repeat");
	private Label		m_lblBlank3    = new Label();

	// Data members.
	private Turtle		m_oTurtle;
	private Program		m_oProgram;
	private	int			m_nLoops;
}
