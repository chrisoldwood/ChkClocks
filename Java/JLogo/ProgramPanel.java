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
		m_pnlFields.add(m_btnClear, m_btnImport);
		m_pnlFields.add(m_btnExec,  m_cbSpeed);
		m_pnlFields.add(m_btnView,  m_btnEdit);

		// Add the event handlers.
		m_btnClear.addActionListener(this);
		m_btnImport.addActionListener(this);
		m_btnExec.addActionListener(this);
		m_btnView.addActionListener(this);
		m_btnEdit.addActionListener(this);

		// Add the speeds.
		m_cbSpeed.add("Fast");
		m_cbSpeed.add("Medium");
		m_cbSpeed.add("Slow");

		// Set defaults.
		m_cbSpeed.select(0);
	}

	/********************************************************************************
	** Button pressed.
	*/

	public void actionPerformed(ActionEvent eEvent)
	{
		if (eEvent.getSource() == m_btnClear)
			onClear();
		if (eEvent.getSource() == m_btnImport)
			onImport();
		else if (eEvent.getSource() == m_btnExec)
			onExec();
		else if (eEvent.getSource() == m_btnView)
			onView();
		else if (eEvent.getSource() == m_btnEdit)
			onEdit();
	}

	/********************************************************************************
	** Clear action.
	*/

	public void onClear()
	{
		m_oTurtle.reset(true);
		m_oProgram.clear();
	}

	/********************************************************************************
	** Import action.
	*/

	public void onImport()
	{
		ImportProgDlg Dlg = new ImportProgDlg(this);

		if (Dlg.prompt() == Dlg.OK)
		{
			CmdFactory oCmdFactory = new CmdFactory();

			// Delete the existing program.
			m_oTurtle.reset(true);
			m_oProgram.clear();

			// Break the source code into lines.
			StrTok   st = new StrTok(true, false);
			String[] astrSource = st.splitLines(Dlg.m_strSource);

			// Parse all lines.
			for (int i = 0; i < astrSource.length; i++)
			{
				try
				{
					m_oProgram.add(oCmdFactory.createCmd(astrSource[i]));
				}
				catch(InvalidCmdException e)
				{
					String strMsg = "Invalid command on line: " + i + "\n\n" + e.getMessage() + "\n\nContinue with import?";

					if (MsgBox.query(this, "JLogo Import", strMsg, MsgBox.YES_NO) == MsgBox.NO)
						break;
				}
			}
		}
	}

	/********************************************************************************
	** Execute action.
	*/

	public void onExec()
	{
		int nSpeed = m_cbSpeed.getSelectedIndex() * Program.SPEED_FACTOR;

		m_oProgram.execute(this, m_oTurtle, nSpeed);
	}

	/********************************************************************************
	** View source action.
	*/

	public void onView()
	{
		SourceLines oLines = m_oProgram.getSource();

		ViewSourceDlg Dlg = new ViewSourceDlg(this, oLines);
		Dlg.show();
	}

	/********************************************************************************
	** Edit source action.
	*/

	public void onEdit()
	{
		EditSourceDlg Dlg = new EditSourceDlg(this, m_oProgram);
		Dlg.show();

		// Notify, if source modified.
		if (Dlg.isSrcModified())
			m_oProgram.notifyListeners(Program.EDITED);
	}

	/********************************************************************************
	** Constants.
	*/

	/********************************************************************************
	** Members.
	*/

	// Child panels.
	private DualColPanel m_pnlFields = new DualColPanel();

	// Child controls.
	private Button		m_btnClear   = new Button("Clear");
	private Button 		m_btnImport  = new Button("Import");

	private Button		m_btnExec    = new Button("Run");
	private Choice		m_cbSpeed    = new Choice();

	private Button		m_btnView    = new Button("View Source");
	private Button		m_btnEdit    = new Button("Edit Source");

	// Data members.
	private Turtle		m_oTurtle;
	private Display		m_oDisplay;
	private Program		m_oProgram;
}
