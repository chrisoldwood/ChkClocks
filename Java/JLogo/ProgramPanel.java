import java.awt.*;
import java.awt.event.*;

/********************************************************************************
** This panel is used to host the program control options.
*/

public class ProgramPanel extends GroupPanel
	implements ActionListener
{
	/********************************************************************************
	** Constructor.
	*/

	public ProgramPanel(Turtle oTurtle, Display oDisplay, Program oProgram)
	{
		super("Program", Label.CENTER, new BorderLayout(5, 5));

		// Save parameters.
		m_oTurtle  = oTurtle;
		m_oDisplay = oDisplay;
		m_oProgram = oProgram;

		// Create this panel.
		setBackground(SystemColor.control);
		add(BorderLayout.CENTER, m_pnlFields);

		// Create the child panels.
		m_pnlFields.add(m_btnClear);
		m_pnlFields.add(m_btnExec);
		m_pnlFields.add(m_btnView);

		// Add the event handlers.
		m_btnClear.addActionListener(this);
		m_btnExec.addActionListener(this);
		m_btnView.addActionListener(this);
	}

	/********************************************************************************
	** Button pressed.
	*/

	public void actionPerformed(ActionEvent eEvent)
	{
		if (eEvent.getSource() == m_btnClear)
			onClear();
		else if (eEvent.getSource() == m_btnExec)
			onExec();
		else if (eEvent.getSource() == m_btnView)
			onView();
	}

	/********************************************************************************
	** Clear action.
	*/

	public void onClear()
	{
		m_oTurtle.reset();
		m_oProgram.clear();
	}

	/********************************************************************************
	** Execute action.
	*/

	public void onExec()
	{
		m_oProgram.execute(m_oTurtle, EXEC_INTERVAL);
	}

	/********************************************************************************
	** View source action.
	*/

	public void onView()
	{
		String str = m_oProgram.getSource();

		SourceDlg Dlg = new SourceDlg(this, str);
		Dlg.show();
	}

	/********************************************************************************
	** Constants.
	*/

	// Default execute interval.
	public static final int EXEC_INTERVAL = 500;

	/********************************************************************************
	** Members.
	*/

	// Child panels.
	private Panel		m_pnlFields  = new Panel(new GridLayout(0, 1, 5, 5));

	// Child controls.
	private Button		m_btnClear = new Button("Clear");
	private Button		m_btnExec  = new Button("Run");
	private Button		m_btnView  = new Button("View Source");

	// Data members.
	private Turtle		m_oTurtle;
	private Display		m_oDisplay;
	private Program		m_oProgram;
}
